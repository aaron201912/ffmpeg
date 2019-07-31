/*
 * playerWnd.c
 *
 *  Created on: 2019-7-3
 *      Author: koda.xu
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>

// START_OF_INCS
#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>

#include <mgncs/mgncs.h>
#include <mgeff/mgeff.h>
#include <mgncs4touch/mgncs4touch.h>
#include <mgncs/mdblist.h>
#include <minigui/ctrl/trackbar.h>

#include "mi_sys.h"
#include "mi_divp.h"
#include "mi_disp.h"
#include "mi_panel.h"
#include "mi_gfx.h"
#include "SAT070CP50_1024x600.h"
#include "usbdetect.h"
#include "demux.h"
#include "videostream.h"
#include "audiostream.h"
#include "player.h"
#include "playerWnd.h"

#define READ_INFO_FROM_XML      0
#define SHOW_PLAY_TOOL_BAR      1
#if READ_INFO_FROM_XML
#include "xmlparser.h"
#include "app_config.h"
#endif

// END_OF_INCS


#define MAKE_YUYV_VALUE(y,u,v)  ((y) << 24) | ((u) << 16) | ((y) << 8) | (v)
#define YUYV_BLACK              MAKE_YUYV_VALUE(0,128,128)
#define YUYV_WHITE              MAKE_YUYV_VALUE(255,128,128)
#define YUYV_RED                MAKE_YUYV_VALUE(76,84,255)
#define YUYV_GREEN              MAKE_YUYV_VALUE(149,43,21)
#define YUYV_BLUE               MAKE_YUYV_VALUE(29,225,107)

#define TABLESIZE(table)    (sizeof(table)/sizeof(table[0]))

#define DIR_TYPE        "文件夹"
#define FILE_TYPE       "文件"
#define COL_DIV         16
#define COL_GAP         2
#define ROW_HEIGHT      40
#define COL_NUM         TABLESIZE(g_lv_caption)

#define DIVP_CHN        0
#define DISP_DEV        0
#define DISP_LAYER      0
#define DISP_INPUTPORT  0
#define AUDIO_DEV       0
#define AUDIO_CHN       0
#define AUDIO_SAMPLE_PER_FRAME  1024

#define FILE_FILTER_NUM         6

typedef struct _FileTree_t
{
    char name[256];
    char time[32];
    int dirFlag;
    int depth;
    int childCnt;
    long size;
    list_t headNodeList;
    list_t childNodeList;
} FileTree_t;

typedef enum
{
    E_PLAY_FORWARD,
    E_PLAY_BACKWARD
}PlayDirection_e;

typedef enum
{
    E_PLAY_NORMAL_MODE,
    E_PLAY_FAST_MODE,
    E_PLAY_SLOW_MODE
}PlayMode_e;

typedef enum
{
    E_NORMAL_SPEED = 0,
    E_2X_SPEED,
    E_4X_SPEED,
    E_8X_SPEED,
    E_16X_SPEED,
    E_32X_SPEED
}PlaySpeedMode_e;

typedef struct
{
    MI_S32 s32X;
    MI_S32 s32Y;
    MI_S16 s16PicW;
    MI_S16 s16PicH;
} Rect_t;

static PLOGFONT g_font_fft_arial = NULL;
static PLOGFONT g_font_fft_fzcircle = NULL;
static PLOGFONT g_font_fft_times = NULL;
static PLOGFONT g_font_fft = NULL;


// file list page
static FileTree_t g_fileRoot;
static MI_S32 g_s32SelectIdx = 0;
static MI_U8 g_pu8RootPath[256] = "/customer";
static MI_U8 g_pu8FullPath[256] = {0};
static MI_U8 g_pu8SelectPath[256] = {0};            // save select file
static char *g_pFileFilter[FILE_FILTER_NUM] = {".mp4", ".mpg", ".avi", ".wav", ".flv", ".mkv"/*, ".rmvb"*/};

// playing page
static MI_BOOL g_bShowPlayToolBar = FALSE;          // select file list page or playing page
static MI_BOOL g_bPlaying = FALSE;
static MI_BOOL g_bPause = FALSE;
static MI_BOOL g_bMute = FALSE;
static MI_S32 g_s32VolValue = 0;
static MI_BOOL g_ePlayDirection = E_PLAY_FORWARD;
static PlayMode_e g_ePlayMode = E_PLAY_NORMAL_MODE;
static PlaySpeedMode_e g_eSpeedMode = E_NORMAL_SPEED;
static MI_U32 g_u32SpeedNumerator = 1;
static MI_U32 g_u32SpeedDenomonator = 1;
static MI_S32 g_s32Duration = 0;
static MI_S32 g_s32PlayPos = 0;
static player_stat_t *g_pstPlayStat = NULL;

static HWND hMainPlayFileWnd = HWND_INVALID;

// bmp
static BITMAP g_item_upfolder;
static BITMAP g_item_folder;
static BITMAP g_item_file;
static BITMAP g_playfile_updir;
static BITMAP g_play_btn;
static BITMAP g_pause_btn;
static BITMAP g_stop_btn;
static BITMAP g_slow_btn;
static BITMAP g_fast_btn;
static BITMAP g_muteOn_btn;
static BITMAP g_muteOff_btn;
static BITMAP g_playtoolbar;

// date
static int g_bigMonth[] = {1, 3, 5, 7, 8, 10, 12};

// listview items
static char * g_lv_caption[] =
{
    "名称", "类型", "大小", "时间"
};

static int g_colDiv[] =
{
    5, 2, 3, 6
};

// play time info
char time_info[] = "--:--:--/--:--:--";


#if READ_INFO_FROM_XML
static XmlRect_t g_stPlayToolBarItem[11] = {
    {900, 540, 124, 60}, {800, 100, 124, 60}, {800, 100, 124, 60}, {800, 200, 124, 60},
    {800, 200, 124, 60}, {800, 200, 124, 60}, {800, 300, 124, 60}, {800, 300, 124, 60},
    {800, 300, 124, 60}, {800, 300, 124, 60}, {800, 300, 124, 60}
};
#else
// static Rect_t g_stPlayToolBarItem[12] = {
// 	{  0,   0,  50,   50}, { 50,   0,  14,  50}, { 64,   0, 960,  50}, {  0,  50, 1024, 550},
//     {  0, 520, 1024,  80}, { 20, 520, 984,  30}, { 20, 550,  50,  50}, { 75, 550,   50,  50},
//     {130, 550,   50,  50}, {185, 550,  50,  50}, {240, 550,  80,  50}, {804, 560,  200,  50}
// };

static Rect_t g_stPlayToolBarItem[] = {
    {  0,   0,  50,   50}, { 50,   0,  14,  50}, { 64,   0, 960,  50}, {  0,  50, 1024, 550},
    {  0, 520, 1024,  80}, { 20, 520, 984,  30}, { 20, 550,  80,  48}, {105, 550,   80,  48},
    {190, 550,   80,  48}, {275, 550,  80,  48}, {360, 550,  80,  48}, {480, 559,   32,  32},
    {520, 550, 260, 50}, {804, 550,  200,  50}
};
#endif

// font
//static PLOGFONT g_pPlayFileFont = NULL;

static void lv_notify(mWidget *self, int id, int nc, DWORD add_data);
static void updir_btn_notify(mWidget *button, int id, int nc, DWORD add_data);
static void play_btn_notify(mWidget *button, int id, int nc, DWORD add_data);
static void stop_btn_notify(mWidget *button, int id, int nc, DWORD add_data);
static void playslow_btn_notify(mWidget *button, int id, int nc, DWORD add_data);
static void playfast_btn_notify(mWidget *button, int id, int nc, DWORD add_data);
static void mute_btn_notify(mWidget *button, int id, int nc, DWORD add_data);
static void play_trk_notify(mTrackBar* self, int id, int code, DWORD add_data);
static void voice_trk_notify(mTrackBar* self, int id, int code, DWORD add_data);
static void playFileWnd_notify(mWidget *self, int message, int code, DWORD usrData);
static BOOL onTimer(mWidget *widget, int message, int id, DWORD tick);


void StopPlayVideo();
void StopPlayAudio();
static void ResetPlayingStatus();

static NCS_RDR_INFO g_rdr_info[] = {
    { "fashion", "fashion", NULL },
};

static NCS_RDR_INFO g_rdr_info2[] = {
    { "classic", "classic", NULL },
};

static NCS_EVENT_HANDLER lv_handlers[] = {
    NCS_MAP_NOTIFY(NCSN_LISTV_SELCHANGED, lv_notify),
    NCS_MAP_NOTIFY(NCSN_WIDGET_CLICKED, lv_notify),
    {0, NULL}
};

static NCS_EVENT_HANDLER updir_btn_handlers [] = {
    NCS_MAP_NOTIFY(NCSN_WIDGET_CLICKED, updir_btn_notify),
    {0, NULL}
};

static NCS_EVENT_HANDLER play_btn_handlers [] = {
    NCS_MAP_NOTIFY(NCSN_WIDGET_CLICKED, play_btn_notify),
    {0, NULL}
};

static NCS_EVENT_HANDLER stop_btn_handlers [] = {
    NCS_MAP_NOTIFY(NCSN_WIDGET_CLICKED, stop_btn_notify),
    {0, NULL}
};

static NCS_EVENT_HANDLER playslow_btn_handlers [] = {
    NCS_MAP_NOTIFY(NCSN_WIDGET_CLICKED, playslow_btn_notify),
    {0, NULL}
};

static NCS_EVENT_HANDLER playfast_btn_handlers [] = {
    NCS_MAP_NOTIFY(NCSN_WIDGET_CLICKED, playfast_btn_notify),
    {0, NULL}
};

static NCS_EVENT_HANDLER mute_btn_handlers [] = {
    NCS_MAP_NOTIFY(NCSN_WIDGET_CLICKED, mute_btn_notify),
    {0, NULL}
};

static NCS_EVENT_HANDLER play_trk_handlers [] = {
    NCS_MAP_NOTIFY(NCSN_TRKBAR_CHANGED, play_trk_notify),
    {0, NULL}
};

static NCS_EVENT_HANDLER voice_trk_handlers [] = {
    NCS_MAP_NOTIFY(NCSN_TRKBAR_CHANGED, voice_trk_notify),
    {0, NULL}
};

static NCS_EVENT_HANDLER timer_handles[] = {
    {MSG_TIMER, onTimer},
    {0, NULL}
};

static NCS_PROP_ENTRY play_trk_props [] = {
    {NCSP_TRKBAR_MINPOS, 0},
    {NCSP_TRKBAR_MAXPOS, 100},
    {NCSP_TRKBAR_CURPOS, 0},
    {0, 0}
};

static NCS_PROP_ENTRY voice_trk_props [] = {
    {NCSP_TRKBAR_MINPOS, 0},
    {NCSP_TRKBAR_MAXPOS, 100},
    {NCSP_TRKBAR_CURPOS, 0},
    {0, 0}
};


static NCS_WND_TEMPLATE _playFileWnd_ctrl_tmpl[] = {
    // updirBtn/returnBtn
    {
        NCSCTRL_BUTTON,
        IDC_PLAYFILE_BUTTON_UPPER_DIR,
        240, 255, 80, 30,
        WS_VISIBLE | NCSS_NOTIFY | NCSS_BUTTON_IMAGE,
        WS_EX_NONE,
        "updir",
        NULL,
        NULL,
        updir_btn_handlers,
        NULL,
        0,
        0
    },

    // gap
    {
        NCSCTRL_STATIC,
        IDC_PLAYFILE_STATIC_GAP,
        240, 255, 80, 30,
        WS_VISIBLE,
        WS_EX_NONE, //WS_EX_TRANSPARENT,
        "",
        NULL,
        NULL,
        NULL,
        NULL,
        0,
        0,
        MakeRGBA(0xFF, 0xFF, 0xFF, 0xFF)
    },

    // current dirPath label
    {
        NCSCTRL_STATIC,
        IDC_PLAYFILE_STATIC_DIR_NAME,
        240, 255, 80, 30,
        WS_VISIBLE,
        WS_EX_NONE, //WS_EX_TRANSPARENT,
        "",
        NULL,
        NULL,
        NULL,
        NULL,
        0,
        0,
        MakeRGBA(0xFF, 0xFF, 0xFF, 0xFF)
    },

    // play files listview
    {
        NCSCTRL_LISTVIEW,
        IDC_PLAYFILE_LISTVIEW_FILELIST,
        10, 10, 320, 220,
        WS_BORDER | WS_VISIBLE | NCSS_LISTV_SORT | NCSS_LISTV_NOTIFY,
        WS_EX_NONE,
        "play file list",
        NULL,
        g_rdr_info,
        lv_handlers,
        NULL,
        0,
        0
    },

    // playtoolbar container static
    {
        NCSCTRL_STATIC,
        IDC_PLAYFILE_STATIC_PLAYTOOLBAR,
        240, 255, 80, 30,
        WS_NONE,        //WS_VISIBLE,
        WS_EX_NONE,
        "",
        NULL,
        NULL,
        NULL,
        NULL,
        0,
        0
    },

    // play progress trackbar
    {
        NCSCTRL_TRACKBAR,
        IDC_PLAYFILE_TRACKBAR_PLAY_PROGRESS,
        10, 260, 240, 40,
        NCSS_TRKBAR_HORIZONTAL | NCSS_TRKBAR_NOTICK | NCSS_NOTIFY, // WS_VISIBLE
        WS_EX_TRANSPARENT,
        "",
        play_trk_props,
        g_rdr_info,
        play_trk_handlers,
        NULL,
        0,
        0,
    },

    // play/pause btn
    {
        NCSCTRL_BUTTON,
        IDC_PLAYFILE_BUTTON_PLAY_PAUSE,
        240, 255, 80, 30,
        NCSS_NOTIFY | NCSS_BUTTON_IMAGE,        //WS_VISIBLE
        WS_EX_NONE,
        "",
        NULL,
        NULL,
        play_btn_handlers,
        NULL,
        0,
        0
    },

    // stop btn
    {
        NCSCTRL_BUTTON,
        IDC_PLAYFILE_BUTTON_STOP,
        240, 255, 80, 30,
        NCSS_NOTIFY | NCSS_BUTTON_IMAGE,        //WS_VISIBLE
        WS_EX_NONE,
        "",
        NULL,
        NULL,
        stop_btn_handlers,
        NULL,
        0,
        0
    },

    // play slow btn
    {
        NCSCTRL_BUTTON,
        IDC_PLAYFILE_BUTTON_PLAY_SLOW,
        240, 255, 80, 30,
        NCSS_NOTIFY | NCSS_BUTTON_IMAGE,        //WS_VISIBLE
        WS_EX_NONE,
        "",
        NULL,
        NULL,
        playslow_btn_handlers,
        NULL,
        0,
        0
    },

    // play fast btn
    {
        NCSCTRL_BUTTON,
        IDC_PLAYFILE_BUTTON_PLAY_FAST,
        240, 255, 80, 30,
        NCSS_NOTIFY | NCSS_BUTTON_IMAGE,        //WS_VISIBLE
        WS_EX_NONE,
        "",
        NULL,
        NULL,
        playfast_btn_handlers,
        NULL,
        0,
        0
    },

    // play speed label
    {
        NCSCTRL_STATIC,
        IDC_PLAYFILE_STATIC_SPEED_MODE,
        240, 255, 80, 30,
        WS_NONE,        //WS_VISIBLE,
        WS_EX_TRANSPARENT,
        "",
        NULL,
        NULL,
        NULL,
        NULL,
        0,
        0
    },

    // voice mute btn
    {
        NCSCTRL_BUTTON,
        IDC_PLAYFILE_BUTTON_VOICE_MUTE,
        240, 255, 80, 30,
        NCSS_NOTIFY | NCSS_BUTTON_IMAGE,        //WS_VISIBLE
        WS_EX_NONE,
        "",
        NULL,
        NULL,
        mute_btn_handlers,
        NULL,
        0,
        0
    },

    // voice adjustment
    {
        NCSCTRL_TRACKBAR,
        IDC_PLAYFILE_TRACKBAR_VOICE,
        10, 260, 240, 40,
        NCSS_TRKBAR_HORIZONTAL | NCSS_TRKBAR_NOTICK | NCSS_NOTIFY, // WS_VISIBLE
        WS_EX_TRANSPARENT,
        "",
        voice_trk_props,
        g_rdr_info2,
        voice_trk_handlers,
        NULL,
        0,
        0,
    },

    // play time label
    {
        NCSCTRL_STATIC,
        IDC_PLAYFILE_STATIC_PLAY_TIME,
        240, 255, 80, 30,
//		WS_NONE,        
		WS_VISIBLE,
        WS_EX_TRANSPARENT,
        "",
        NULL,
        NULL,
//		NULL,
		timer_handles,
        NULL,
        0,
        0
    },
};

static NCS_EVENT_HANDLER playFileWnd_handlers[] = {
    {MSG_CLOSE, (void *)playFileWnd_notify},
    //{MSG_ERASEBKGND, (void *)playFileWnd_notify},
    {0, NULL}
};

static NCS_MNWND_TEMPLATE playFileWnd_tmpl = {
    NCSCTRL_MAINWND,
    IDC_PLAYFILEPAGE_WINNOW,
    0, 0, MAINWND_W, MAINWND_H,
    WS_VISIBLE,
    WS_EX_AUTOSECONDARYDC,
    "",
    NULL,
    g_rdr_info,
    playFileWnd_handlers,
    _playFileWnd_ctrl_tmpl,
    sizeof(_playFileWnd_ctrl_tmpl)/sizeof(NCS_WND_TEMPLATE),
    0,
    MakeRGBA(0x80, 0x80, 0x80, 0x80),        //0,
    0,
};

// date function
static char *getDayOfWeek(int day)
{
    switch (day)
    {
        case 0:
            return "周日";
        case 1:
            return "周一";
        case 2:
            return "周二";
        case 3:
            return "周三";
        case 4:
            return "周四";
        case 5:
            return "周五";
        case 6:
            return "周六";
    }

    printf("invalid day %d\n", day);
    return NULL;
}

// month: [1, 12]
static int getBigMonthCount(int month)
{
    int i = 0;
    int count = 0;

    for (i = 0; i < sizeof(g_bigMonth)/sizeof(int); i++)
    {
        if (month > g_bigMonth[i])
            count++;
        else
            break;
    }

    //printf("count is %d, curMonth is %d\n", count, month);
    return count;
}

// month: [1, 12]
static int getDateOfMonth(int year, int month, int date)
{
    int febDays = 28;
    int monthDays = 30;
    int curMonthDate = 0;

    if ((year%400 == 0) || (year%100 != 0 && year%4 == 0))
    {
        febDays = 29;
    }

    if (month > 2)
    {
        curMonthDate = date - ((month-2)*monthDays + febDays + getBigMonthCount(month));
    }
    else if (month > 1)
    {
        curMonthDate = date - ((month-1)*monthDays + getBigMonthCount(month));
    }
    else
    {
        curMonthDate = date;
    }

    //printf("date is %d, curMonthDate is %d, febDays is %d\n", date, curMonthDate, febDays);
    return curMonthDate;
}

static int FilterFiles(char *pFileName, char **pFilter, int filterCnt)
{
    int i = 0;
    int fileNameLen = strlen(pFileName);

    for(; i < filterCnt; i++)
    {
        int filterLen = strlen(pFilter[i]);

        if (fileNameLen > filterLen)
        {
            if (!strncmp(pFileName+(fileNameLen-filterLen), pFilter[i], filterLen))
                return 0;
        }
    }

    return -1;
}

// file list opts
static void InitFileTreeRoot(FileTree_t *root, char *pRootName)
{
    memset(root, 0, sizeof(FileTree_t));
    root->dirFlag = 1;
    strcpy(root->name, pRootName);
    INIT_LIST_HEAD(&root->headNodeList);
    INIT_LIST_HEAD(&root->childNodeList);
}

static int CreateFileTree(FileTree_t *root)
{
    DIR *pDir =  NULL;
    struct dirent *ent;
    struct stat statbuf;
    struct tm *modifytm;
    FileTree_t *child = NULL;
    int dirFlag = 0;

    pDir = opendir(root->name);
    if (!pDir)
    {
        //printf("%s directory is not exist or open failed\n", root->name);
        return -1;
    }

    lstat(root->name, &statbuf);
    modifytm = localtime(&(statbuf.st_mtime));
    root->size = statbuf.st_size;
    sprintf(root->time, "%d/%d/%d %d:%d:%d %s", modifytm->tm_year+1900, modifytm->tm_mon+1,
            getDateOfMonth(modifytm->tm_year+1900, modifytm->tm_mon+1, modifytm->tm_yday+1),
            modifytm->tm_hour, modifytm->tm_min, modifytm->tm_sec,
            getDayOfWeek(modifytm->tm_wday));

    while ((ent=readdir(pDir)) != NULL)
    {
        if (ent->d_type & DT_DIR)
        {
            if (!strcmp(ent->d_name, ".") || !strcmp(ent->d_name, ".."))
                continue;

            dirFlag = 1;
        }
        else
        {
            // file filter
            if (FilterFiles(ent->d_name, g_pFileFilter, FILE_FILTER_NUM))
                continue;

            dirFlag = 0;
        }

        child = (FileTree_t*)malloc(sizeof(FileTree_t));
        memset(child, 0, sizeof(FileTree_t));
        INIT_LIST_HEAD(&child->headNodeList);;
        INIT_LIST_HEAD(&child->childNodeList);
        if (!strcmp(root->name, "/"))
            sprintf(child->name, "%s%s", root->name, ent->d_name);
        else
            sprintf(child->name, "%s/%s", root->name, ent->d_name);
        child->depth = root->depth + 1;
        root->childCnt++;
        list_add_tail(&child->childNodeList, &root->headNodeList);

        if (dirFlag)
        {
            child->dirFlag = 1;
            CreateFileTree(child);
        }
        else
        {
            child->dirFlag = 0;
            lstat(child->name, &statbuf);
            modifytm = localtime(&(statbuf.st_mtime));
            child->size = statbuf.st_size;
            sprintf(child->time, "%d/%d/%d %d:%d:%d %s", modifytm->tm_year+1900, modifytm->tm_mon+1,
                    getDateOfMonth(modifytm->tm_year+1900, modifytm->tm_mon+1, modifytm->tm_yday+1),
                    modifytm->tm_hour, modifytm->tm_min, modifytm->tm_sec,
                    getDayOfWeek(modifytm->tm_wday));
        }
    }

    closedir(pDir);
    pDir = NULL;

    return 0;
}

static void BrowseFileTree(FileTree_t *root)
{
    FileTree_t *pos = NULL;

    if (!root)
    {
        printf("FileTree is not exist\n");
        return;
    }

    printf("name:%-50sdirFlag:%-5dsize:%-8ld KB time:%-32sdepth:%-5dchildCnt:%-5d\n", root->name, root->dirFlag, (root->size+1023)/1024,
            root->time, root->depth, root->childCnt);

    if (list_empty(&root->headNodeList))
    {
        return;
    }

    list_for_each_entry(pos, &root->headNodeList, childNodeList)
    {
        BrowseFileTree(pos);
    }
}

static FileTree_t *FindFileTreeNode(FileTree_t *root, char *name)
{
    FileTree_t *pos = NULL;
    FileTree_t *result = NULL;

    if (!root)
        return NULL;

    if (!strcmp(root->name, name))
        return root;

    if (list_empty(&root->headNodeList))
    {
        //printf("%s is not exist in %s\n", name, root->name);
        return NULL;
    }

    list_for_each_entry(pos, &(root->headNodeList), childNodeList)
    {
        if ((result = FindFileTreeNode(pos, name)) != NULL)
            return result;
    }

    return NULL;
}

static int InsertFileTreeNode(FileTree_t *father, FileTree_t *child)
{
    child->depth = father->depth + 1;
    father->childCnt++;
    INIT_LIST_HEAD(&child->childNodeList);
    list_add_tail(&child->childNodeList, &father->headNodeList);

    return 0;
}

static int DestroyFileTree(FileTree_t *root)
{
    FileTree_t *pos = NULL;
    FileTree_t *posN = NULL;

    if (!root)
    {
        printf("root is NULL\n");
        return 0;
    }

    if (list_empty(&root->headNodeList))
    {
    //    printf("FileTree is empty\n");
        free(root);
        root = NULL;
        return 0;
    }

    list_for_each_entry_safe(pos, posN, &root->headNodeList, childNodeList)
    {
        DestroyFileTree(pos);
    }

    return 0;
}

static int DeleteFileTreeNode(FileTree_t *father, FileTree_t *child)
{
    list_del(&child->childNodeList);
    child->depth = 0;
    father->childCnt--;

    DestroyFileTree(child);
    return 0;
}

static void SetBtnImg(mButton *pBtnObj, PBITMAP pBmp)
{
    if (pBtnObj)
    {
        mPushButtonPiece *body;
        mImagePiece *content;

        _c(pBtnObj)->setProperty(pBtnObj, NCSP_BUTTON_IMAGE, (DWORD)pBmp);
        body = (mPushButtonPiece*)((mWidget*)pBtnObj->body);
        content = (mImagePiece*)body->content;
        _c(content)->setProperty(content, NCSP_IMAGEPIECE_DRAWMODE, NCS_DM_SCALED);
    }
}

// duration, format, width, height, I-frame/P-frame, etc.
MI_S32 GetMediaInfo()
{
    return 0;
}

// current pos(play time) s
MI_S32 GetCurrentPlayPos(MI_S32 s32Duration, MI_S32 s32CurrentPos)
{
    char timeInfo[32];
    int trackPos = 0;
    mStatic *timeInfoObj = (mStatic *)ncsGetChildObj(hMainPlayFileWnd, IDC_PLAYFILE_STATIC_PLAY_TIME);
    mTrackBar *trkObj = (mTrackBar *)ncsGetChildObj(hMainPlayFileWnd, IDC_PLAYFILE_TRACKBAR_PLAY_PROGRESS);
    g_s32Duration = s32Duration;
    g_s32PlayPos = s32CurrentPos;

    // update playtime static
    memset(timeInfo, 0, sizeof(timeInfo));
    sprintf(timeInfo, "%02d:%02d:%02d / %02d:%02d:%02d", s32CurrentPos/3600, (s32CurrentPos%3600)/60, s32CurrentPos%60,
            s32Duration/3600, (s32Duration%3600)/60, s32Duration%60);
    _c(timeInfoObj)->setProperty(timeInfoObj, NCSP_WIDGET_TEXT, (DWORD)timeInfo);

    // update progress bar
    trackPos = (s32CurrentPos * 100) / s32Duration;
    _c(trkObj)->setProperty(trkObj, NCSP_TRKBAR_CURPOS, trackPos);

    return 0;
}

// MI display video
MI_S32 DisplayVideo(MI_S32 s32DispWidth, MI_S32 s32DispHeight, void *pYData, void *pUVData)
{
    MI_SYS_BUF_HANDLE hHandle;
    MI_SYS_ChnPort_t pstSysChnPort;
    MI_SYS_BufConf_t stBufConf;
    MI_SYS_BufInfo_t stBufInfo;

    pstSysChnPort.eModId    = E_MI_MODULE_ID_DISP;
    pstSysChnPort.u32ChnId  = 0;
    pstSysChnPort.u32DevId  = 0;
    pstSysChnPort.u32PortId = 0;

    memset(&stBufInfo , 0 , sizeof(MI_SYS_BufInfo_t));
    memset(&stBufConf , 0 , sizeof(MI_SYS_BufConf_t));

    stBufConf.eBufType = E_MI_SYS_BUFDATA_FRAME;
    stBufConf.u64TargetPts = 0;
    stBufConf.stFrameCfg.u16Width  = s32DispWidth;
    stBufConf.stFrameCfg.u16Height = s32DispHeight;
    stBufConf.stFrameCfg.eFormat = E_MI_SYS_PIXEL_FRAME_YUV_SEMIPLANAR_420;
    stBufConf.stFrameCfg.eFrameScanMode = E_MI_SYS_FRAME_SCAN_MODE_PROGRESSIVE;

    if(MI_SUCCESS == MI_SYS_ChnInputPortGetBuf(&pstSysChnPort,&stBufConf,&stBufInfo,&hHandle, -1))
    {
        stBufInfo.stFrameData.eCompressMode = E_MI_SYS_COMPRESS_MODE_NONE;
        stBufInfo.stFrameData.eFieldType = E_MI_SYS_FIELDTYPE_NONE;
        stBufInfo.stFrameData.eTileMode = E_MI_SYS_FRAME_TILE_MODE_NONE;
        stBufInfo.bEndOfStream = FALSE;

        memcpy(stBufInfo.stFrameData.pVirAddr[0], pYData, s32DispWidth*s32DispHeight);
        memcpy(stBufInfo.stFrameData.pVirAddr[1], pUVData, s32DispWidth*s32DispHeight/2);

//		FILE *fpread1 = fopen("pic_later.yuv", "a+");
//		fwrite(stBufInfo.stFrameData.pVirAddr[0], 1, s32DispWidth * s32DispHeight, fpread1);
//		fwrite(stBufInfo.stFrameData.pVirAddr[1], 1, s32DispWidth * s32DispHeight / 2, fpread1);
//		fclose(fpread1);

        MI_SYS_ChnInputPortPutBuf(hHandle ,&stBufInfo , FALSE);
    }
	else {
		printf("get MI_SYS_ChnInputPortGetBuf failed!\n");
	}

    return 0;
}

// MI play audio
MI_S32 PlayAudio(MI_U8 *pu8AudioData, MI_U32 u32DataLen)
{
    MI_AUDIO_Frame_t stAoSendFrame;
    MI_S32 s32RetSendStatus = 0;
    MI_AUDIO_DEV AoDevId = AUDIO_DEV;
    MI_AO_CHN AoChn = AUDIO_CHN;

    //read data and send to AO module
    stAoSendFrame.u32Len = u32DataLen;
    stAoSendFrame.apVirAddr[0] = pu8AudioData;
    stAoSendFrame.apVirAddr[1] = NULL;

    do{
        s32RetSendStatus = MI_AO_SendFrame(AoDevId, AoChn, &stAoSendFrame, 128);
    }while(s32RetSendStatus == MI_AO_ERR_NOBUF);

    if(s32RetSendStatus != MI_SUCCESS)
    {
        printf("[Warning]: MI_AO_SendFrame fail, error is 0x%x: \n",s32RetSendStatus);
    }

    return 0;
}

// pause audio
MI_S32 PauseAudio()
{
    MI_AO_PauseChn(AUDIO_DEV, AUDIO_CHN);
    return 0;
}

// resume audio
MI_S32 ResumeAudio()
{
    MI_AO_ResumeChn(AUDIO_DEV, AUDIO_CHN);
    return 0;
}

// stay in playing page, clear play status
MI_S32 PlayComplete()
{
    mButton *pPlayObj = (mButton*)ncsGetChildObj(hMainPlayFileWnd, IDC_PLAYFILE_BUTTON_PLAY_PAUSE);
    mStatic *pTimeObj = (mStatic*)ncsGetChildObj(hMainPlayFileWnd, IDC_PLAYFILE_STATIC_PLAY_TIME);
	mTrackBar *pProcessObj = (mStatic*)ncsGetChildObj(hMainPlayFileWnd, IDC_PLAYFILE_TRACKBAR_PLAY_PROGRESS);

	init_clock(&g_pstPlayStat->video_clk, &g_pstPlayStat->video_pkt_queue.serial);
    init_clock(&g_pstPlayStat->audio_clk, &g_pstPlayStat->audio_pkt_queue.serial);

   	toggle_pause(g_pstPlayStat);
    g_bPlaying = FALSE;
    g_bPause = TRUE;

	// 文件流回到开始位置,显示第一帧后暂定等待
	stream_seek(g_pstPlayStat, g_pstPlayStat->p_fmt_ctx->start_time, 0, 0);
	
    SetBtnImg(pPlayObj, &g_play_btn);
    ResetPlayingStatus();

	memcpy(time_info, "00:00:00", 8);
	_c(pTimeObj)->setProperty(pTimeObj, NCSP_STATIC_ALIGN, NCS_ALIGN_CENTER);
	_c(pTimeObj)->setProperty(pTimeObj, NCSP_WIDGET_TEXT, (DWORD)time_info);
	InvalidateRect(pTimeObj->hwnd, NULL, TRUE);

    _c(pProcessObj)->setProperty(pProcessObj, NCSP_TRKBAR_CURPOS, 0);
    InvalidateRect(pProcessObj->hwnd, NULL, TRUE);

    return 0;
}

// stay in playing page , clear play status,
MI_S32 PlayError()
{
    mButton *pPlayObj = (mButton*)ncsGetChildObj(hMainPlayFileWnd, IDC_PLAYFILE_BUTTON_PLAY_PAUSE);
    mStatic *pSpeedModeObj = (mStatic*)ncsGetChildObj(hMainPlayFileWnd, IDC_PLAYFILE_STATIC_SPEED_MODE);
    // stop play video & audio
    g_bPlaying = FALSE;
    g_bPause = FALSE;

    // sendmessage to stop playing
    player_deinit(g_pstPlayStat);
    StopPlayAudio();
    StopPlayVideo();

    SetBtnImg(pPlayObj, &g_play_btn);
    ResetPlayingStatus();

    _c(pSpeedModeObj)->setProperty(pSpeedModeObj, NCSP_WIDGET_TEXT, (DWORD)"");
    InvalidateRect(hMainPlayFileWnd, NULL, TRUE);

    return 0;
}

static void show_lv_page(MI_BOOL bShow)
{
    mStatic *gapObj = (mStatic *)ncsGetChildObj(hMainPlayFileWnd, IDC_PLAYFILE_STATIC_GAP);
    mStatic *pathObj = (mStatic *)ncsGetChildObj(hMainPlayFileWnd, IDC_PLAYFILE_STATIC_DIR_NAME);
    mButton *upDirObj = (mButton *)ncsGetChildObj(hMainPlayFileWnd, IDC_PLAYFILE_BUTTON_UPPER_DIR);
    mListView *lvObj = (mListView *)ncsGetChildObj(hMainPlayFileWnd, IDC_PLAYFILE_LISTVIEW_FILELIST);

    printf("show lv page\n");

    if (bShow)
    {
        IncludeWindowStyle(upDirObj->hwnd, WS_VISIBLE);
        IncludeWindowStyle(lvObj->hwnd, WS_VISIBLE);
        IncludeWindowStyle(pathObj->hwnd, WS_VISIBLE);
        IncludeWindowStyle(gapObj->hwnd, WS_VISIBLE);
    }
    else
    {
        ExcludeWindowStyle(upDirObj->hwnd, WS_VISIBLE);
        ExcludeWindowStyle(lvObj->hwnd, WS_VISIBLE);
        ExcludeWindowStyle(pathObj->hwnd, WS_VISIBLE);
        ExcludeWindowStyle(gapObj->hwnd, WS_VISIBLE);
    }
}

static void show_playfile_page(MI_BOOL bShow)
{
    mStatic *pToolBarObj = (mStatic*)ncsGetChildObj(hMainPlayFileWnd, IDC_PLAYFILE_STATIC_PLAYTOOLBAR);
    mStatic *pSpeedModeObj = (mStatic*)ncsGetChildObj(hMainPlayFileWnd, IDC_PLAYFILE_STATIC_SPEED_MODE);
    mStatic *pPlayTimeObj = (mStatic*)ncsGetChildObj(hMainPlayFileWnd, IDC_PLAYFILE_STATIC_PLAY_TIME);
    mTrackBar *pProgressObj = (mTrackBar*)ncsGetChildObj(hMainPlayFileWnd, IDC_PLAYFILE_TRACKBAR_PLAY_PROGRESS);
    mTrackBar *pVoiceObj = (mTrackBar*)ncsGetChildObj(hMainPlayFileWnd, IDC_PLAYFILE_TRACKBAR_VOICE);
    mButton *pPlayBtnObj = (mButton*)ncsGetChildObj(hMainPlayFileWnd, IDC_PLAYFILE_BUTTON_PLAY_PAUSE);
    mButton *pStopBtnObj = (mButton*)ncsGetChildObj(hMainPlayFileWnd, IDC_PLAYFILE_BUTTON_STOP);
    mButton *pSlowBtnObj = (mButton*)ncsGetChildObj(hMainPlayFileWnd, IDC_PLAYFILE_BUTTON_PLAY_SLOW);
    mButton *pFastBtnObj = (mButton*)ncsGetChildObj(hMainPlayFileWnd, IDC_PLAYFILE_BUTTON_PLAY_FAST);
    mButton *pMuteBtnObj = (mButton*)ncsGetChildObj(hMainPlayFileWnd, IDC_PLAYFILE_BUTTON_VOICE_MUTE);

    printf("show playfile page\n");

    if (bShow)
    {
        IncludeWindowStyle(pToolBarObj->hwnd, WS_VISIBLE);
        IncludeWindowStyle(pSpeedModeObj->hwnd, WS_VISIBLE);
        IncludeWindowStyle(pPlayTimeObj->hwnd, WS_VISIBLE);
        IncludeWindowStyle(pProgressObj->hwnd, WS_VISIBLE);
        IncludeWindowStyle(pVoiceObj->hwnd, WS_VISIBLE);
        IncludeWindowStyle(pPlayBtnObj->hwnd, WS_VISIBLE);
        IncludeWindowStyle(pStopBtnObj->hwnd, WS_VISIBLE);
        IncludeWindowStyle(pSlowBtnObj->hwnd, WS_VISIBLE);
        IncludeWindowStyle(pFastBtnObj->hwnd, WS_VISIBLE);
        IncludeWindowStyle(pMuteBtnObj->hwnd, WS_VISIBLE);
    }
    else
    {
        ExcludeWindowStyle(pToolBarObj->hwnd, WS_VISIBLE);
        ExcludeWindowStyle(pSpeedModeObj->hwnd, WS_VISIBLE);
        ExcludeWindowStyle(pPlayTimeObj->hwnd, WS_VISIBLE);
        ExcludeWindowStyle(pProgressObj->hwnd, WS_VISIBLE);
        ExcludeWindowStyle(pVoiceObj->hwnd, WS_VISIBLE);
        ExcludeWindowStyle(pPlayBtnObj->hwnd, WS_VISIBLE);
        ExcludeWindowStyle(pStopBtnObj->hwnd, WS_VISIBLE);
        ExcludeWindowStyle(pSlowBtnObj->hwnd, WS_VISIBLE);
        ExcludeWindowStyle(pFastBtnObj->hwnd, WS_VISIBLE);
        ExcludeWindowStyle(pMuteBtnObj->hwnd, WS_VISIBLE);
    }
}

static void ResetPlayingStatus()
{
    g_ePlayDirection = E_PLAY_FORWARD;
    g_ePlayMode = E_PLAY_NORMAL_MODE;
    g_eSpeedMode = E_NORMAL_SPEED;
    g_u32SpeedNumerator = 1;
    g_u32SpeedDenomonator = 1;
}

static void SetPlayerControlCallBack(player_stat_t *is)
{
    is->playerController.fpGetMediaInfo = GetMediaInfo;
    is->playerController.fpGetCurrentPlayPos = GetCurrentPlayPos;
    is->playerController.fpDisplayVideo = DisplayVideo;
    is->playerController.fpPlayAudio = PlayAudio;
    is->playerController.fpPauseAudio = PauseAudio;
    is->playerController.fpResumeAudio = ResumeAudio;
    is->playerController.fpPlayComplete = PlayComplete;
    is->playerController.fpPlayError = PlayError;
}

static void add_fileinfo_item(mListView *self, NCS_LISTV_ITEMINFO *info, FileTree_t *pFileNode)
{
    int i = 0, j = 0;
    NCS_LISTV_ITEMDATA subdata[COL_NUM];
    int color = 0xFFFFFF;
    //int color = 0x0;
    int nRowIdx = 0;
    FileTree_t *pos = NULL;

    // font
    HDC clientDc = NULL;
    PLOGFONT logfont;
    FONTMETRICS fm;

    if (!pFileNode)
        return;

    clientDc = GetClientDC(self->hwnd);
    logfont = GetWindowFont(self->hwnd);
    GetFontMetrics(logfont, &fm);
    printf("old font size is max_w:%d avg_w:%d h:%d\n", fm.max_width, fm.ave_width, fm.font_height);

//  SelectFont(clientDc, g_pPlayFileFont);
    logfont = GetWindowFont(self->hwnd);
    GetFontMetrics(logfont, &fm);
    printf("current font size is max_w:%d avg_w:%d h:%d\n", fm.max_width, fm.ave_width, fm.font_height);

    info->dataSize = COL_NUM;
    info->data = subdata;

    printf("filenode: depth:%d, childCnt:%d, name:%s\n", pFileNode->depth, pFileNode->childCnt, pFileNode->name);

    list_for_each_entry(pos, &pFileNode->headNodeList, childNodeList)
    {
        info->index = nRowIdx;
        for (j = 0; j < COL_NUM; j++)
        {
            subdata[j].row = info->index;
            subdata[j].col = j;
            subdata[j].textColor = 0;
            subdata[j].flags = 0;
            subdata[j].text = "";
            subdata[j].image = 0;

            switch (j)
            {
                case 0:
                    {
                        char dirName[256];
                        char *p = NULL;
                        memset(dirName, 0, sizeof(dirName));
                        memcpy(dirName, pos->name, strlen(pos->name));
                        p = strrchr(dirName, '/');
                        *p = 0;
                        subdata[j].flags = NCSF_ITEM_USEBITMAP;
                        subdata[j].text = pos->name+strlen(dirName)+1;
                        if (pos->dirFlag)
                            subdata[j].image = (DWORD)&g_item_folder;
                        else
                            subdata[j].image = (DWORD)&g_item_file;
                    }
                    break;
                case 1:
                    subdata[j].text = (char*)(pos->dirFlag ? DIR_TYPE:FILE_TYPE);
                    break;
                case 2:
                    {
                        char szSize[32];
                        memset(szSize, 0, sizeof(szSize));
                        sprintf(szSize, "%ld KB", (pos->size+1023)/1024);
                        subdata[j].text = szSize;
                    }
                    break;
                case 3:
                    subdata[j].text = pos->time;
                    break;
                default:
                    break;
            }
        }

        _c(self)->addItem(self, info);
        nRowIdx++;
    }

    for (i = 0; i < nRowIdx; i++)
    {
        for (j = 0; j < COL_NUM; j++)
        {
            _c(self)->setBackground(self, i, j, &color);
        }
    }

    ReleaseDC(clientDc);
}

static void lv_notify(mWidget *self, int id, int nc, DWORD add_data)
{
    mListView *lstvObj = (mListView*)self;

    if (!lstvObj)
        return;

    // event occur order: selchanged clicked itemdbcliked
    if (nc == NCSN_LISTV_SELCHANGED)
    {
        g_s32SelectIdx = _c(lstvObj)->indexOf(lstvObj, (HITEM)add_data);
        printf("select item index is %d\n", g_s32SelectIdx);
    }

    if (nc == NCSN_WIDGET_CLICKED)
    {
        const char *fileName = _c(lstvObj)->getItemText(lstvObj, g_s32SelectIdx, 0);
        const char *type = _c(lstvObj)->getItemText(lstvObj, g_s32SelectIdx, 1);
        mStatic *pathObj = (mStatic*)ncsGetChildObj(hMainPlayFileWnd, IDC_PLAYFILE_STATIC_DIR_NAME);
        mStatic *pSpeedModeObj = (mStatic*)ncsGetChildObj(hMainPlayFileWnd, IDC_PLAYFILE_STATIC_SPEED_MODE);
        mButton *pPlayObj = (mButton *)ncsGetChildObj(hMainPlayFileWnd, IDC_PLAYFILE_BUTTON_PLAY_PAUSE);
        mTrackBar *pVoiceObj = (mTrackBar*)ncsGetChildObj(hMainPlayFileWnd, IDC_PLAYFILE_TRACKBAR_VOICE);
        mTrackBar *pProgressObj = (mTrackBar *)ncsGetChildObj(hMainPlayFileWnd, IDC_PLAYFILE_TRACKBAR_PLAY_PROGRESS);
		mStatic *pTimeObj = (mStatic *)ncsGetChildObj(hMainPlayFileWnd, IDC_PLAYFILE_STATIC_PLAY_TIME);
		NCS_LISTV_ITEMINFO info;
        FileTree_t *curFileNode = NULL;

        if (!pathObj)
            return;

        printf("select item: name is %s, type is %s\n", fileName, type);

        // confirm btn is disable utill selected file
        if (!strcmp(type, FILE_TYPE))
        {
            // save selectpath & play file
            memset(g_pu8SelectPath, 0, sizeof(g_pu8SelectPath));
            sprintf((char*)g_pu8SelectPath, "%s/%s", (char*)g_pu8FullPath, fileName);

            // hide
            _c(pSpeedModeObj)->setProperty(pSpeedModeObj, NCSP_WIDGET_TEXT, (DWORD)"");
            g_bShowPlayToolBar = TRUE;
            show_lv_page(FALSE);
            show_playfile_page(TRUE);
            SetBtnImg(pPlayObj, &g_pause_btn);
            ResetPlayingStatus();
            _c(pVoiceObj)->setProperty(pVoiceObj, NCSP_TRKBAR_CURPOS, g_s32VolValue);
            //_c(pProgressObj)->setProperty(pProgressObj, NCSP_TRKBAR_CURPOS, g_s32VolValue);
            printf("current vol value is %d\n", g_s32VolValue);
            InvalidateRect(hMainPlayFileWnd, NULL, TRUE);

            // sendmessage to play file
            StartPlayVideo();
            StartPlayAudio();
            g_pstPlayStat = player_init((const char*)g_pu8SelectPath);
            if (!g_pstPlayStat)
            {
                StopPlayAudio();
                StopPlayVideo();
                SetBtnImg(pPlayObj, &g_play_btn);
                InvalidateRect(GetParent(lstvObj->hwnd), NULL, TRUE);
                return;
            }

            g_bPlaying = TRUE;
            g_bPause = FALSE;
			g_pstPlayStat->paused = false;
            g_pstPlayStat->out_width = MAINWND_W;
#if SHOW_PLAY_TOOL_BAR
            g_pstPlayStat->out_height = MAINWND_H - g_stPlayToolBarItem[4].s16PicH;
#else
            g_pstPlayStat->out_height = MAINWND_H;
#endif
			printf("dispalay windows height : %d.\n", g_pstPlayStat->out_height);
            SetPlayerControlCallBack(g_pstPlayStat);

            printf("fileName is %s\n", g_pstPlayStat->filename);
            open_demux(g_pstPlayStat);
            open_video(g_pstPlayStat);
            open_audio(g_pstPlayStat);	

			// printf file time info
			MI_U32 second, minute, hours;

			if (g_pstPlayStat->p_fmt_ctx->duration > 0)
			{
				second = g_pstPlayStat->p_fmt_ctx->duration / AV_TIME_BASE;
				minute = second / 60;
				hours  = minute / 60;
				sprintf(time_info, "--:--:--/%02d:%02d:%02d", (hours % 24), (minute % 60), (second % 60));
			}

			_c(pTimeObj)->setProperty(pTimeObj, NCSP_STATIC_ALIGN, NCS_ALIGN_CENTER);
			_c(pTimeObj)->setProperty(pTimeObj, NCSP_WIDGET_TEXT, (DWORD)time_info);
			InvalidateRect(GetParent(pTimeObj->hwnd), NULL, TRUE);
			
			SetTimer((mWidget *)pTimeObj->hwnd, IDC_PLAYFILE_STATIC_PLAY_TIME, 100);
			
		}
        else
        {
            info.height     = ROW_HEIGHT;
            info.flags      = 0;
            info.foldIcon   = 0;
            info.unfoldIcon = 0;
            info.parent = 0;

            if (!strcmp(type, DIR_TYPE))
            {
                if (!strcmp((char*)g_pu8FullPath, "/"))
                    sprintf((char*)g_pu8FullPath, "%s%s", (char*)g_pu8FullPath, fileName);
                else
                    sprintf((char*)g_pu8FullPath, "%s/%s", (char*)g_pu8FullPath, fileName);

                _c(pathObj)->setProperty(pathObj, NCSP_WIDGET_TEXT, (DWORD)g_pu8FullPath);
                curFileNode = FindFileTreeNode(&g_fileRoot, (char*)g_pu8FullPath);
                printf("notify, fullPath:%s pFileNode:%s\n", (char*)g_pu8FullPath, curFileNode?curFileNode->name:"null");

                if (curFileNode)
                {
                    _c(lstvObj)->freeze(lstvObj, TRUE);
                    _c(lstvObj)->removeAll(lstvObj);
                    add_fileinfo_item(lstvObj, &info, curFileNode);
                    _c(lstvObj)->freeze(lstvObj, FALSE);
                }
            }
        }
    }
}

static void updir_btn_notify(mWidget *button, int id, int nc, DWORD add_data)
{
    mListView *lvObj = NULL;
    mStatic *pathObj = NULL;
    NCS_LISTV_ITEMINFO info;
    int i  = 0;
    int color = 0x00FFFF;

    printf("Clicked updir btn\n");
    // do recollection stuff
    if (!button)
        return;

    lvObj = (mListView*)ncsGetChildObj(GetParent(button->hwnd), IDC_PLAYFILE_LISTVIEW_FILELIST);
    pathObj = (mStatic*)ncsGetChildObj(GetParent(button->hwnd), IDC_PLAYFILE_STATIC_DIR_NAME);
    mButton *pPlayObj = (mButton*)ncsGetChildObj(GetParent(button->hwnd), IDC_PLAYFILE_BUTTON_PLAY_PAUSE);
    mStatic *pSpeedModeObj = (mStatic*)ncsGetChildObj(GetParent(button->hwnd), IDC_PLAYFILE_STATIC_SPEED_MODE);

    if(nc == NCSN_WIDGET_CLICKED)
    {
        ResetPlayingStatus();

        if (g_bShowPlayToolBar)
        {
            // sendmessage to stop playing file
            player_deinit(g_pstPlayStat);
            StopPlayAudio();
            StopPlayVideo();
            SetBtnImg(pPlayObj, &g_play_btn);
            ResetPlayingStatus();

            _c(pSpeedModeObj)->setProperty(pSpeedModeObj, NCSP_WIDGET_TEXT, (DWORD)"");
            g_bShowPlayToolBar = FALSE;
            show_playfile_page(FALSE);
            show_lv_page(TRUE);
            InvalidateRect(GetParent(lvObj->hwnd), NULL, TRUE);
        }
        else
        {
            if (!strcmp((char*)g_pu8FullPath, (char*)g_pu8RootPath))
            {
                //PostMessage(hMainPlayFileWnd, MSG_CLOSE, 0, 0);
            }
            else
            {
                FileTree_t *curFileNode = NULL;
                char *p = strrchr((char*)g_pu8FullPath, '/');
                *p = 0;

                _c(pathObj)->setProperty(pathObj, NCSP_WIDGET_TEXT, (DWORD)g_pu8FullPath);
                curFileNode = FindFileTreeNode(&g_fileRoot, (char*)g_pu8FullPath);
                printf("notify, fullPath:%s pFileNode:%s\n", (char*)g_pu8FullPath, curFileNode?curFileNode->name:"null");

                if (curFileNode)
                {

                    for (i = 0; i < COL_NUM; i++)
                    {
                        _c(lvObj)->setBackground(lvObj, g_s32SelectIdx, i, &color);
                    }

                    info.height     = ROW_HEIGHT;
                    info.flags      = 0;
                    info.foldIcon   = 0;
                    info.unfoldIcon = 0;
                    info.parent = 0;

                    _c(lvObj)->freeze(lvObj, TRUE);
                    _c(lvObj)->removeAll(lvObj);
                    add_fileinfo_item(lvObj, &info, curFileNode);
                    _c(lvObj)->freeze(lvObj, FALSE);
                }
            }
        }
    }
}

static void play_btn_notify(mWidget *button, int id, int nc, DWORD add_data)
{
    mButton *pBtnObj = (mButton*)button;
    printf("Clicked play/pause btn\n");

    if (!button)
        return;

    if(nc == NCSN_WIDGET_CLICKED)
    {
//		if (g_bPlaying)
//		{
//			g_bPause = !g_bPause;
//			// sendmessage to pause/resume playing
//			toggle_pause(g_pstPlayStat);
//		}
//		else
//		{
//		    g_bPlaying = TRUE;
//		    g_bPause = FALSE;

            // sendmessage to start playing
//			StartPlayVideo();
//			StartPlayAudio();

//			g_pstPlayStat = player_init((const char*)g_pu8SelectPath);
//			if (!g_pstPlayStat)
//			{
//			    StopPlayAudio();
//			    StopPlayVideo();
//			    return;
//			}

//			open_demux(g_pstPlayStat);
//			open_video(g_pstPlayStat);
//			open_audio(g_pstPlayStat);
//        }

		toggle_pause(g_pstPlayStat);            // 判断并设置暂停/播放状态
		
		if (g_pstPlayStat->paused) {
		    SetBtnImg(pBtnObj, &g_play_btn);
			g_bPause   = TRUE;
			g_bPlaying = FALSE;
		}else {
		    SetBtnImg(pBtnObj, &g_pause_btn);
			g_bPause   = FALSE;
			g_bPlaying = TRUE;
		}

		InvalidateRect(pBtnObj->hwnd, NULL, TRUE);
    }
}

static void stop_btn_notify(mWidget *button, int id, int nc, DWORD add_data)
{
    mButton *pPlayObj = (mButton*)ncsGetChildObj(GetParent(button->hwnd), IDC_PLAYFILE_BUTTON_PLAY_PAUSE);
    mStatic *pSpeedModeObj = (mStatic*)ncsGetChildObj(GetParent(button->hwnd), IDC_PLAYFILE_STATIC_SPEED_MODE);
	mStatic *pTimeObj = (mStatic*)ncsGetChildObj(GetParent(button->hwnd), IDC_PLAYFILE_STATIC_PLAY_TIME);
    printf("Clicked stop btn\n");

    if (!button)
        return;

    if(nc == NCSN_WIDGET_CLICKED)
    {
        g_bPlaying = FALSE;
        g_bPause = FALSE;

		KillTimer(pTimeObj->hwnd, IDC_PLAYFILE_STATIC_PLAY_TIME);

        // sendmessage to stop playing
        player_deinit(g_pstPlayStat);
        StopPlayAudio();
        StopPlayVideo();

        SetBtnImg(pPlayObj, &g_play_btn);
        ResetPlayingStatus();

        _c(pSpeedModeObj)->setProperty(pSpeedModeObj, NCSP_WIDGET_TEXT, (DWORD)"");
        g_bShowPlayToolBar = FALSE;
        show_playfile_page(FALSE);
        show_lv_page(TRUE);
        InvalidateRect(hMainPlayFileWnd, NULL, TRUE);

        //PostMessage(GetParent(button->hwnd), MSG_CLOSE, 0, 0);
    }
}

static void playslow_btn_notify(mWidget *button, int id, int nc, DWORD add_data)
{
    mStatic *pSpeedModeObj = (mStatic*)ncsGetChildObj(GetParent(button->hwnd), IDC_PLAYFILE_STATIC_SPEED_MODE);
//	char speedMode[16] = {0};
	double incr, pos;

    printf("Clicked slow btn\n");

    if (!button || !pSpeedModeObj)
        return;

    if (!g_bPlaying)
        return;

    if(nc == NCSN_WIDGET_CLICKED)
    {
//		if (g_ePlayDirection == E_PLAY_FORWARD)
//		{
//		    // slow down
//		    if (g_ePlayMode ==  E_PLAY_FAST_MODE)
//		    {
//		        g_eSpeedMode = (PlaySpeedMode_e)((int)g_eSpeedMode - 1);
//		        g_u32SpeedNumerator = 1 << (int)g_eSpeedMode;
//		        g_u32SpeedDenomonator = 1;

//		        if (g_eSpeedMode == E_NORMAL_SPEED)
//		            g_ePlayMode = E_PLAY_NORMAL_MODE;
//		    }
//		    else
//		    {
//		        if (g_eSpeedMode < E_32X_SPEED)
//		        {
//		            g_ePlayMode = E_PLAY_SLOW_MODE;
//		            g_eSpeedMode = (PlaySpeedMode_e)((int)g_eSpeedMode + 1);
//		        }
//		        else    // turn to play backward
//		        {
//		            g_ePlayDirection = E_PLAY_BACKWARD;
//		            g_ePlayMode = E_PLAY_NORMAL_MODE;
//		            g_eSpeedMode = E_NORMAL_SPEED;
//		        }

//		        g_u32SpeedNumerator = 1;
//		        g_u32SpeedDenomonator = 1 << (int)g_eSpeedMode;
//		    }
//		}
//		else
//		{
//		    // speed up
//		    if (g_ePlayMode == E_PLAY_SLOW_MODE)
//		    {
//		        g_eSpeedMode = (PlaySpeedMode_e)((int)g_eSpeedMode - 1);
//		        g_u32SpeedNumerator = 1;
//		        g_u32SpeedDenomonator = 1 << (int)g_eSpeedMode;

//		        if (g_eSpeedMode == E_NORMAL_SPEED)
//		        {
//		            g_ePlayMode = E_PLAY_NORMAL_MODE;
//		        }
//		    }
//		    else
//		    {
//		        if (g_eSpeedMode < E_32X_SPEED)
//		        {
//		            g_ePlayMode = E_PLAY_FAST_MODE;
//		            g_eSpeedMode = (PlaySpeedMode_e)((int)g_eSpeedMode + 1);
//		            g_u32SpeedNumerator = 1 << (int)g_eSpeedMode;
//		            g_u32SpeedDenomonator = 1;
//		        }
//		    }
//		}

//		memset(speedMode, 0, sizeof(speedMode));
//		if (g_u32SpeedNumerator == g_u32SpeedDenomonator)
//		    sprintf(speedMode, "", g_u32SpeedNumerator);
//		else if (g_u32SpeedNumerator > g_u32SpeedDenomonator)
//		    sprintf(speedMode, "%dX %s", g_u32SpeedNumerator, ((g_ePlayDirection == E_PLAY_FORWARD) ? ">>" : "<<"));
//		else
//		    sprintf(speedMode, "1/%dX %s", g_u32SpeedDenomonator, ((g_ePlayDirection == E_PLAY_FORWARD) ? ">>" : "<<"));
//		_c(pSpeedModeObj)->setProperty(pSpeedModeObj, NCSP_WIDGET_TEXT, (DWORD)speedMode);

		pos = get_master_clock(g_pstPlayStat);
        if (isnan(pos))
            pos = (double)g_pstPlayStat->seek_pos / AV_TIME_BASE;
        pos -= 5.0;
        if (g_pstPlayStat->p_fmt_ctx->start_time != AV_NOPTS_VALUE && pos < g_pstPlayStat->p_fmt_ctx->start_time / (double)AV_TIME_BASE)
            pos = g_pstPlayStat->p_fmt_ctx->start_time / (double)AV_TIME_BASE;
        stream_seek(g_pstPlayStat, (int64_t)(pos * AV_TIME_BASE), (int64_t)(incr * AV_TIME_BASE), 0);
    }
}

static void playfast_btn_notify(mWidget *button, int id, int nc, DWORD add_data)
{
    mStatic *pSpeedModeObj = (mStatic*)ncsGetChildObj(GetParent(button->hwnd), IDC_PLAYFILE_STATIC_SPEED_MODE);
//	char speedMode[16] = {0};
	double incr, pos;

    printf("Clicked fast btn\n");

    // do recollection stuff
    if (!button || !pSpeedModeObj)
        return;

    if (!g_bPlaying)
        return;

    if(nc == NCSN_WIDGET_CLICKED)
    {
//		if (g_ePlayDirection == E_PLAY_FORWARD)
//		{
//		    // speed up
//		    if (g_ePlayMode == E_PLAY_SLOW_MODE)
//		    {
//		        g_eSpeedMode = (PlaySpeedMode_e)((int)g_eSpeedMode - 1);
//		        g_u32SpeedNumerator = 1;
//		        g_u32SpeedDenomonator = 1 << (int)g_eSpeedMode;

//		        if (g_eSpeedMode == E_NORMAL_SPEED)
//		            g_ePlayMode = E_PLAY_NORMAL_MODE;
//		    }
//		    else
//		    {
//		        if (g_eSpeedMode < E_32X_SPEED)
//		        {
//		            g_ePlayMode = E_PLAY_FAST_MODE;
//		            g_eSpeedMode = (PlaySpeedMode_e)((int)g_eSpeedMode + 1);
//		            g_u32SpeedNumerator = 1 << (int)g_eSpeedMode;
//		            g_u32SpeedDenomonator = 1;
//		        }
//		    }
//		}
//		else
//		{
//		    // slow down
//		    if (g_ePlayMode == E_PLAY_FAST_MODE)
//		    {
//		        g_eSpeedMode = (PlaySpeedMode_e)((int)g_eSpeedMode - 1);
//		        g_u32SpeedNumerator = 1 << (int)g_eSpeedMode;
//		        g_u32SpeedDenomonator = 1;

//		        if (g_eSpeedMode == E_NORMAL_SPEED)
//		            g_ePlayMode = E_PLAY_NORMAL_MODE;
//		    }
//		    else
//		    {
//		        // 1/32X speed backward to normal speed forward
//		        if (g_eSpeedMode == E_32X_SPEED)
//		        {
//		            g_eSpeedMode = E_NORMAL_SPEED;
//		            g_ePlayMode = E_PLAY_NORMAL_MODE;
//		            g_ePlayDirection = E_PLAY_FORWARD;
//		        }
//		        else
//		        {
//		            g_ePlayMode = E_PLAY_SLOW_MODE;
//		            g_eSpeedMode = (PlaySpeedMode_e)((int)g_eSpeedMode + 1);
//		        }

//		        g_u32SpeedNumerator = 1;
//		        g_u32SpeedDenomonator = 1 << (int)g_eSpeedMode;
//		    }
//		}

//		memset(speedMode, 0, sizeof(speedMode));
//		if (g_u32SpeedNumerator == g_u32SpeedDenomonator)
//		    memset(speedMode, 0, sizeof(speedMode));
//		else if (g_u32SpeedNumerator > g_u32SpeedDenomonator)
//		    sprintf(speedMode, "%dX %s", g_u32SpeedNumerator, ((g_ePlayDirection == E_PLAY_FORWARD) ? ">>" : "<<"));
//		else
//		    sprintf(speedMode, "1/%dX %s", g_u32SpeedDenomonator, ((g_ePlayDirection == E_PLAY_FORWARD) ? ">>" : "<<"));
//		_c(pSpeedModeObj)->setProperty(pSpeedModeObj, NCSP_WIDGET_TEXT, (DWORD)speedMode);

        // sendmessage to adjust speed
        //SelectFont(HDC_SCREEN, g_font_fft_fzcircle);

//		PLOGFONT curFont = GetCurFont(HDC_SCREEN);
//		printf("curFont: %s-%s-%s-size%d\n", curFont->type, curFont->family, curFont->charset, curFont->size);
//		SelectFont(HDC_SCREEN, g_font_fft);
//		SetBkMode (HDC_SCREEN, BM_TRANSPARENT);
//		TextOut (HDC_SCREEN, 600, 520, "hahaha,哈哈哈");
//		curFont = GetCurFont(HDC_SCREEN);
//		printf("select Font: %s-%s-%s-size%d-regular\n", g_font_fft->type, g_font_fft->family, g_font_fft->charset, g_font_fft->size);
//		printf("curFont: %s-%s-%s-size%d\n", curFont->type, curFont->family, curFont->charset, curFont->size);

		pos = get_master_clock(g_pstPlayStat);

		if (isnan(pos))
			pos = (double)g_pstPlayStat->seek_pos / AV_TIME_BASE;
		pos += 5.0;   // 每次快进5秒
		
		if (g_pstPlayStat->p_fmt_ctx->start_time != AV_NOPTS_VALUE && pos > g_pstPlayStat->p_fmt_ctx->duration / (double)AV_TIME_BASE)
			pos = g_pstPlayStat->p_fmt_ctx->duration / (double)AV_TIME_BASE;
		stream_seek(g_pstPlayStat, (int64_t)(pos * AV_TIME_BASE), (int64_t)(incr * AV_TIME_BASE), 0);
	}
}

static void mute_btn_notify(mWidget *button, int id, int nc, DWORD add_data)
{
    mButton *pMuteObj = (mButton*)button;
    MI_S32 vol = 0;
    printf("Clicked mute btn\n");

    if (!button)
        return;

    if(nc == NCSN_WIDGET_CLICKED)
    {
        if (g_s32VolValue)
            vol = g_s32VolValue * (MAX_ADJUST_AO_VOLUME - MIN_ADJUST_AO_VOLUME) / 100 + MIN_ADJUST_AO_VOLUME;
        else
            vol = MIN_AO_VOLUME;
        g_bMute = !g_bMute;

        MI_AO_SetVolume(AUDIO_DEV, vol);
        MI_AO_SetMute(AUDIO_DEV, g_bMute);

        if (g_bMute)
        {
            SetBtnImg(pMuteObj, &g_muteOn_btn);
        }
        else
        {
            SetBtnImg(pMuteObj, &g_muteOff_btn);
        }

        InvalidateRect(pMuteObj->hwnd, NULL, TRUE);
    }
}

static void play_trk_notify(mTrackBar* self, int id, int code, DWORD add_data)
{
	mTrackBar *pVideoObj = (mTrackBar*)self;
	MI_S32 t_value;
	int64_t position, offset;
	double s_time;

//	printf("play trackbar notify\n");

	if (!pVideoObj) return;

	switch (code)
	{
		case NCSN_TRKBAR_CHANGED:
			if (g_pstPlayStat) {
				t_value  = _c(pVideoObj)->getProperty(pVideoObj, NCSP_TRKBAR_CURPOS);
				position = g_pstPlayStat->p_fmt_ctx->duration / play_trk_props[1].value * t_value;
				s_time   = g_pstPlayStat->video_clk.pts;      // 获取当前时间戳
				if (position >= 0 && position <= g_pstPlayStat->p_fmt_ctx->duration) 
				{
					position += g_pstPlayStat->p_fmt_ctx->start_time;				
					offset = position - s_time * AV_TIME_BASE;
					// 仅当进度变化大于2秒时执行跳转
					if (offset > 2000000 || offset < -2000000)
					{
						stream_seek(g_pstPlayStat, position, offset, 0);
					}
				}
			}
			break;

		default : break;
	}
}

static void voice_trk_notify(mTrackBar* self, int id, int code, DWORD add_data)
{
    mTrackBar *pVoiceObj = (mTrackBar*)self;
    MI_S32 vol = 0;
    MI_AO_ChnState_t stAoState;
    printf("voice trackbar notify\n");

    if (!pVoiceObj)
        return;

    if (NCSN_TRKBAR_CHANGED == code)
    {
        g_s32VolValue = _c(pVoiceObj)->getProperty(pVoiceObj, NCSP_TRKBAR_CURPOS);

		printf("audio track value : %d.\n", g_s32VolValue);

		if (g_s32VolValue)
            vol = g_s32VolValue * (MAX_ADJUST_AO_VOLUME - MIN_ADJUST_AO_VOLUME) / 100 + MIN_ADJUST_AO_VOLUME;
        else
            vol = MIN_AO_VOLUME;

        memset(&stAoState, 0, sizeof(MI_AO_ChnState_t));
        if (MI_SUCCESS == MI_AO_QueryChnStat(AUDIO_DEV, AUDIO_CHN, &stAoState))
        {
            MI_AO_SetVolume(AUDIO_DEV, vol);
            MI_AO_SetMute(AUDIO_DEV, g_bMute);
        }
    }
}

static BOOL onTimer(mWidget *widget, int message, int id, DWORD tick)
{
	double time;
	char tmpstr[] = "--:--:--";
	char fpsstr[] = "00.00";
	uint32_t second, minute, hours;
	mTrackBar *pProgressObj = (mTrackBar*)ncsGetChildObj(hMainPlayFileWnd, IDC_PLAYFILE_TRACKBAR_PLAY_PROGRESS);
	mStatic *pFpsObj = (mStatic*)ncsGetChildObj(hMainPlayFileWnd, IDC_PLAYFILE_STATIC_SPEED_MODE);

	time = g_pstPlayStat->video_clk.pts;
	second = (uint32_t)time;
//	printf("video clock pts : %.2f. second : %ld.\n", time, second);
	minute = second / 60;
	hours  = minute / 60;
	sprintf(tmpstr, "%02d:%02d:%02d", (hours % 24), (minute % 60), (second % 60));
	memcpy(time_info, tmpstr, 8);

	_c(widget)->setProperty(widget, NCSP_STATIC_ALIGN, NCS_ALIGN_CENTER);
	_c(widget)->setProperty(widget, NCSP_WIDGET_TEXT, (DWORD)time_info);
	InvalidateRect(widget->hwnd, NULL, TRUE);

	uint32_t total_time = g_pstPlayStat->p_fmt_ctx->duration / AV_TIME_BASE;
	WPARAM t_value = (WPARAM)second * play_trk_props[1].value / total_time;
//	printf("video trackbar value : %ld\n", t_value);
	_c(pProgressObj)->setProperty(pProgressObj, NCSP_TRKBAR_THUMB_WIDTH, t_value);
	_c(pProgressObj)->setProperty(pProgressObj, NCSP_TRKBAR_CURPOS, t_value);
	InvalidateRect(pProgressObj->hwnd, NULL, TRUE);

	if (g_pstPlayStat->paused) 
		sprintf(fpsstr, "%.2f", 00.00);
	else
		sprintf(fpsstr, "%.2f", g_pstPlayStat->fps);
	_c(pFpsObj)->setProperty(pFpsObj, NCSP_STATIC_ALIGN, NCS_ALIGN_CENTER);
	_c(pFpsObj)->setProperty(pFpsObj, NCSP_WIDGET_TEXT, (DWORD)fpsstr);
	InvalidateRect(pFpsObj->hwnd, NULL, TRUE);

	return TRUE;
}


static void playFileWnd_notify(mWidget *self, int message, int code, DWORD usrData)
{
    if (message == MSG_CLOSE)
    {
        printf("playFileWnd close...\n");
//        DestroyLogFont(g_pPlayFileFont);
        ncsDestroyWindow(self, 0);
        hMainPlayFileWnd = HWND_INVALID;
    }

    if (message == MSG_ERASEBKGND)
    {

    }
}

static BOOL lstv_init(mWidget* self)
{
    int i;
    FileTree_t *pCurFile = NULL;
    NCS_LISTV_ITEMINFO info;
    NCS_LISTV_CLMINFO lstv_clminfo;
    mListView *lstvObj = (mListView *)ncsGetChildObj(self->hwnd, IDC_PLAYFILE_LISTVIEW_FILELIST);
    mStatic *pPathObj = (mStatic *)ncsGetChildObj(self->hwnd, IDC_PLAYFILE_STATIC_DIR_NAME);
    mButton *pUpDirObj = (mButton *)ncsGetChildObj(self->hwnd, IDC_PLAYFILE_BUTTON_UPPER_DIR);
    mStatic *pToolBarObj = (mStatic *)ncsGetChildObj(self->hwnd, IDC_PLAYFILE_STATIC_PLAYTOOLBAR);
    mButton *pPlayObj = (mButton *)ncsGetChildObj(self->hwnd, IDC_PLAYFILE_BUTTON_PLAY_PAUSE);
    mButton *pStopObj = (mButton *)ncsGetChildObj(self->hwnd, IDC_PLAYFILE_BUTTON_STOP);
    mButton *pSlowObj = (mButton *)ncsGetChildObj(self->hwnd, IDC_PLAYFILE_BUTTON_PLAY_SLOW);
    mButton *pFastObj = (mButton *)ncsGetChildObj(self->hwnd, IDC_PLAYFILE_BUTTON_PLAY_FAST);
    mButton *pMuteObj = (mButton *)ncsGetChildObj(self->hwnd, IDC_PLAYFILE_BUTTON_VOICE_MUTE);
    mStatic *pTimeObj = (mStatic *)ncsGetChildObj(self->hwnd, IDC_PLAYFILE_STATIC_PLAY_TIME);
    mStatic *pSpeedModeObj = (mStatic *)ncsGetChildObj(self->hwnd, IDC_PLAYFILE_STATIC_SPEED_MODE);
    mTrackBar *pVoiceObj = (mTrackBar *)ncsGetChildObj(self->hwnd, IDC_PLAYFILE_TRACKBAR_VOICE);
    mTrackBar *pProgressObj = (mTrackBar *)ncsGetChildObj(self->hwnd, IDC_PLAYFILE_TRACKBAR_PLAY_PROGRESS);

    if (!lstvObj || !pPathObj || !pUpDirObj)
        return FALSE;

    _c(pPathObj)->setProperty(pPathObj, NCSP_STATIC_ALIGN, NCS_ALIGN_LEFT);
    _c(pPathObj)->setProperty(pPathObj, NCSP_WIDGET_TEXT, (DWORD)g_pu8FullPath);
    _c(pToolBarObj)->setProperty(pToolBarObj, NCSP_WIDGET_BKIMAGE, (DWORD)&g_playtoolbar);
    _c(pToolBarObj)->setProperty(pToolBarObj, NCSP_WIDGET_BKIMAGE_MODE, NCS_DM_SCALED);
    SetBtnImg(pUpDirObj, &g_playfile_updir);
    SetBtnImg(pPlayObj, &g_play_btn);           // if be paused, use g_pause_btn image
    SetBtnImg(pStopObj, &g_stop_btn);
    SetBtnImg(pSlowObj, &g_slow_btn);
    SetBtnImg(pFastObj, &g_fast_btn);
    SetBtnImg(pMuteObj, &g_muteOff_btn);

	_c(pSpeedModeObj)->setProperty(pSpeedModeObj, NCSP_STATIC_ALIGN, NCS_ALIGN_CENTER);
	_c(pVoiceObj)->setProperty(pVoiceObj, NCSP_TRKBAR_CURPOS, g_s32VolValue);
	_c(pProgressObj)->setProperty(pProgressObj, NCSP_TRKBAR_CURPOS, 0);

    _c(lstvObj)->freeze(lstvObj, TRUE);
    for (i = 0; i < COL_NUM; i++)
    {
        lstv_clminfo.index  = i;
        lstv_clminfo.text   = g_lv_caption[i];
        lstv_clminfo.width  = g_stPlayToolBarItem[2].s16PicW * g_colDiv[i] / COL_DIV - COL_GAP;
        lstv_clminfo.pfnCmp = NULL;
        lstv_clminfo.sort = NCSID_LSTCLM_LOSORTED;
        lstv_clminfo.flags  = NCSF_LSTCLM_LEFTALIGN | NCSF_LSTCLM_VCENTERALIGN | NCSF_LSTHDR_LEFTALIGN;
        _c(lstvObj)->addColumn(lstvObj, &lstv_clminfo);
    }

    info.height     = ROW_HEIGHT;
    info.flags      = 0;
    info.foldIcon   = 0;
    info.unfoldIcon = 0;
    info.parent = 0;

    pCurFile = FindFileTreeNode(&g_fileRoot, (char*)g_pu8FullPath);
    add_fileinfo_item(lstvObj, &info, pCurFile);
    _c(lstvObj)->freeze(lstvObj, FALSE);

    return TRUE;
}

static PLOGFONT createLocalFont(unsigned size)
{
    return CreateLogFont("ttf", "helvetica", "GB2312",
                         FONT_WEIGHT_BOOK, FONT_SLANT_ROMAN,
                         FONT_SETWIDTH_NORMAL, FONT_OTHER_AUTOSCALE,
                         FONT_UNDERLINE_NONE, FONT_STRUCKOUT_NONE,
                         size, 0);
}

static int LoadProjectPicture()
{
	if (LoadBitmap (HDC_SCREEN, &g_item_folder, "appres/folder.png"))
    {
        return -1;
    }
    if (LoadBitmap (HDC_SCREEN, &g_item_file, "appres/file.png"))
    {
        return -1;
    }
    if (LoadBitmap (HDC_SCREEN, &g_playfile_updir, "appres/upDir.png"))
    {
        return -1;
    }
    // if (LoadBitmap (HDC_SCREEN, &g_play_btn, "appres/play.png"))
    // {
    //     return -1;
    // }
    // if (LoadBitmap (HDC_SCREEN, &g_pause_btn, "appres/pause.png"))
    // {
    //     return -1;
    // }
    // if (LoadBitmap (HDC_SCREEN, &g_stop_btn, "appres/stop.png"))
    // {
    //     return -1;
    // }
    // if (LoadBitmap (HDC_SCREEN, &g_slow_btn, "appres/slow.png"))
    // {
    //     return -1;
    // }
    // if (LoadBitmap (HDC_SCREEN, &g_fast_btn, "appres/fast.png"))
    // {
    //     return -1;
    // }

    if (LoadBitmap (HDC_SCREEN, &g_play_btn, "appres/play_40x24.png"))
    {
        return -1;
    }
    if (LoadBitmap (HDC_SCREEN, &g_pause_btn, "appres/pause_40x24.png"))
    {
        return -1;
    }
    if (LoadBitmap (HDC_SCREEN, &g_stop_btn, "appres/stop_40x24.png"))
    {
        return -1;
    }
    if (LoadBitmap (HDC_SCREEN, &g_slow_btn, "appres/slow_40x24.png"))
    {
        return -1;
    }
    if (LoadBitmap (HDC_SCREEN, &g_fast_btn, "appres/fast_40x24.png"))
    {
        return -1;
    }
    if (LoadBitmap (HDC_SCREEN, &g_muteOn_btn, "appres/muteon.png"))
    {
        return -1;
    }
    if (LoadBitmap (HDC_SCREEN, &g_muteOff_btn, "appres/muteoff.png"))
    {
        return -1;
    }
    if (LoadBitmap (HDC_SCREEN, &g_playtoolbar, "appres/playtoolbar2.png"))
    {
        return -1;
    }

    return 0;
}

static void UnloadProjectPicture()
{
    UnloadBitmap(&g_item_folder);
    UnloadBitmap(&g_item_file);
    UnloadBitmap(&g_playfile_updir);
    UnloadBitmap(&g_play_btn);
    UnloadBitmap(&g_pause_btn);
    UnloadBitmap(&g_stop_btn);
    UnloadBitmap(&g_slow_btn);
    UnloadBitmap(&g_fast_btn);
    UnloadBitmap(&g_muteOn_btn);
    UnloadBitmap(&g_muteOff_btn);
    UnloadBitmap(&g_playtoolbar);
}

MI_S32 CreatePlayerDev()
{
    MI_SYS_Version_t stVersion;
    MI_U64 u64Pts = 0;
    MI_DISP_PubAttr_t stPubAttr;
    MI_DISP_VideoLayerAttr_t stLayerAttr;
//	MI_SYS_ChnPort_t stDivpChnPort;
//	MI_DIVP_ChnAttr_t stDivpChnAttr;
//	MI_DIVP_OutputPortAttr_t stOutputPortAttr;
    MI_DISP_DEV dispDev = DISP_DEV;
    MI_DISP_LAYER dispLayer = DISP_LAYER;
    MI_U32 u32InputPort = DISP_INPUTPORT;
    MI_SYS_ChnPort_t stDispChnPort;
    MI_DISP_InputPortAttr_t stInputPortAttr;

    MI_SYS_Init();
    memset(&stVersion, 0x0, sizeof(MI_SYS_Version_t));
    MI_SYS_GetVersion(&stVersion);
    MI_SYS_GetCurPts(&u64Pts);
    u64Pts = 0xF1237890F1237890;
    MI_SYS_InitPtsBase(u64Pts);
    u64Pts = 0xE1237890E1237890;
    MI_SYS_SyncPts(u64Pts);

//	memset(&stDivpChnPort, 0, sizeof(MI_SYS_ChnPort_t));
//	memset(&stDivpChnAttr, 0, sizeof(MI_DIVP_ChnAttr_t));
//	memset(&stDivpChnAttr, 0, sizeof(MI_DIVP_ChnAttr_t));
//	memset(&stOutputPortAttr, 0, sizeof(stOutputPortAttr));
//	stDivpChnPort.eModId = E_MI_MODULE_ID_DIVP;
//	stDivpChnPort.u32DevId = 0;
//	stDivpChnPort.u32ChnId = DIVP_CHN;
//	stDivpChnPort.u32PortId = 0;
//	stDivpChnAttr.bHorMirror            = FALSE;
//	stDivpChnAttr.bVerMirror            = FALSE;
//	stDivpChnAttr.eDiType               = E_MI_DIVP_DI_TYPE_OFF;
//	stDivpChnAttr.eRotateType           = E_MI_SYS_ROTATE_NONE;
//	stDivpChnAttr.eTnrLevel             = E_MI_DIVP_TNR_LEVEL_OFF;
//	stDivpChnAttr.stCropRect.u16X       = 0;
//	stDivpChnAttr.stCropRect.u16Y       = 0;
//	stDivpChnAttr.stCropRect.u16Width   = 0;
//	stDivpChnAttr.stCropRect.u16Height  = 0;
//	stDivpChnAttr.u32MaxWidth           = MAINWND_W;
//	stOutputPortAttr.eCompMode          = E_MI_SYS_COMPRESS_MODE_NONE;
//	stOutputPortAttr.ePixelFormat       = E_MI_SYS_PIXEL_FRAME_YUV_SEMIPLANAR_420;
//	stOutputPortAttr.u32Width           = MAINWND_W;
//#if SHOW_PLAY_TOOL_BAR
//	stDivpChnAttr.u32MaxHeight          = MAINWND_H - g_stPlayToolBarItem[4].s16PicH;
//	stOutputPortAttr.u32Height          = MAINWND_H - g_stPlayToolBarItem[4].s16PicH;
//#else
//	stDivpChnAttr.u32MaxHeight          = MAINWND_H;
//	stOutputPortAttr.u32Height          = MAINWND_H;
//#endif
//	MI_DIVP_CreateChn(DIVP_CHN, &stDivpChnAttr);
//	MI_DIVP_StartChn(DIVP_CHN);
//	MI_DIVP_SetOutputPortAttr(DIVP_CHN, &stOutputPortAttr);
//	MI_SYS_SetChnOutputPortDepth(&stDivpChnPort, 0, 3);

	memset(&stPubAttr, 0, sizeof(MI_DISP_PubAttr_t));
    stPubAttr.stSyncInfo.u16Vact = stPanelParam.u16Height;
    stPubAttr.stSyncInfo.u16Vbb  = stPanelParam.u16VSyncBackPorch;
    stPubAttr.stSyncInfo.u16Vfb  = stPanelParam.u16VTotal - (stPanelParam.u16VSyncWidth + stPanelParam.u16Height + stPanelParam.u16VSyncBackPorch);
    stPubAttr.stSyncInfo.u16Hact = stPanelParam.u16Width;
    stPubAttr.stSyncInfo.u16Hbb  = stPanelParam.u16HSyncBackPorch;
    stPubAttr.stSyncInfo.u16Hfb  = stPanelParam.u16HTotal - (stPanelParam.u16HSyncWidth + stPanelParam.u16Width + stPanelParam.u16HSyncBackPorch);
    stPubAttr.stSyncInfo.u16Bvact= 0;
    stPubAttr.stSyncInfo.u16Bvbb = 0;
    stPubAttr.stSyncInfo.u16Bvfb = 0;
    stPubAttr.stSyncInfo.u16Hpw  = stPanelParam.u16HSyncWidth;
    stPubAttr.stSyncInfo.u16Vpw  = stPanelParam.u16VSyncWidth;
    stPubAttr.stSyncInfo.u32FrameRate = stPanelParam.u16DCLK*1000000/(stPanelParam.u16HTotal*stPanelParam.u16VTotal);
    stPubAttr.eIntfSync = E_MI_DISP_OUTPUT_USER;
    stPubAttr.eIntfType = E_MI_DISP_INTF_LCD;
    stPubAttr.u32BgColor = YUYV_BLACK;
    MI_DISP_SetPubAttr(dispDev, &stPubAttr);
    MI_DISP_Enable(dispDev);

    stLayerAttr.stVidLayerSize.u16Width  = stPanelParam.u16Width;
    stLayerAttr.stVidLayerSize.u16Height = stPanelParam.u16Height;
    stLayerAttr.ePixFormat = E_MI_SYS_PIXEL_FRAME_YUV_SEMIPLANAR_420;
    stLayerAttr.stVidLayerDispWin.u16X      = 0;
    stLayerAttr.stVidLayerDispWin.u16Y      = 0;
    stLayerAttr.stVidLayerDispWin.u16Width  = stPanelParam.u16Width;
    stLayerAttr.stVidLayerDispWin.u16Height = stPanelParam.u16Height;
    MI_DISP_SetVideoLayerAttr(dispLayer, &stLayerAttr);
    MI_DISP_BindVideoLayer(dispLayer, dispDev);
    MI_DISP_EnableVideoLayer(dispLayer);

//	memset(&stDispChnPort, 0, sizeof(MI_SYS_ChnPort_t));
//	stDispChnPort.eModId    = E_MI_MODULE_ID_DISP;
//	stDispChnPort.u32DevId  = DISP_DEV;
//	stDispChnPort.u32ChnId  = 0;
//	stDispChnPort.u32PortId = DISP_INPUTPORT;

	memset(&stInputPortAttr, 0, sizeof(MI_DISP_InputPortAttr_t));
    MI_DISP_GetInputPortAttr(dispLayer, u32InputPort, &stInputPortAttr);
    stInputPortAttr.stDispWin.u16X      = 0;
    stInputPortAttr.stDispWin.u16Y      = 0;
    stInputPortAttr.stDispWin.u16Width  = MAINWND_W;
    stInputPortAttr.u16SrcWidth = MAINWND_W;
#if SHOW_PLAY_TOOL_BAR
    stInputPortAttr.stDispWin.u16Height = MAINWND_H - g_stPlayToolBarItem[4].s16PicH;
    stInputPortAttr.u16SrcHeight = MAINWND_H - g_stPlayToolBarItem[4].s16PicH;
#else
    stInputPortAttr.stDispWin.u16Height = MAINWND_H;
    stInputPortAttr.u16SrcHeight = MAINWND_H;
#endif

    MI_DISP_SetInputPortAttr(dispLayer, u32InputPort, &stInputPortAttr);
    MI_DISP_GetInputPortAttr(dispLayer, u32InputPort, &stInputPortAttr);
    MI_DISP_EnableInputPort(dispLayer, u32InputPort);
    MI_DISP_SetInputPortSyncMode(dispLayer, u32InputPort, E_MI_DISP_SYNC_MODE_FREE_RUN);

    MI_PANEL_Init(stPanelParam.eLinkType);
    MI_PANEL_SetPanelParam(&stPanelParam);
    MI_GFX_Open();

//	MI_SYS_BindChnPort(&stDivpChnPort, &stDispChnPort, 30, 30);

    return MI_SUCCESS;
}

void DestroyPlayerDev()
{
    MI_DISP_DEV dispDev = DISP_DEV;
    MI_DISP_LAYER dispLayer = DISP_LAYER;
    MI_U32 u32InputPort = DISP_INPUTPORT;
//	MI_SYS_ChnPort_t stDivpChnPort;
//	MI_SYS_ChnPort_t stDispChnPort;

//	memset(&stDivpChnPort, 0, sizeof(MI_SYS_ChnPort_t));
//	memset(&stDispChnPort, 0, sizeof(MI_SYS_ChnPort_t));
//	stDivpChnPort.eModId = E_MI_MODULE_ID_DIVP;
//	stDivpChnPort.u32DevId = 0;
//	stDivpChnPort.u32ChnId = DIVP_CHN;
//	stDivpChnPort.u32PortId = 0;
//	stDispChnPort.eModId = E_MI_MODULE_ID_DISP;
//	stDispChnPort.u32DevId = DISP_DEV;
//	stDispChnPort.u32ChnId = 0;
//	stDispChnPort.u32PortId = DISP_INPUTPORT;
//	MI_SYS_UnBindChnPort(&stDivpChnPort, &stDispChnPort);

    MI_GFX_Close();
    MI_PANEL_DeInit();
    MI_DISP_DisableInputPort(dispLayer, u32InputPort);
    MI_DISP_DisableVideoLayer(dispLayer);
    MI_DISP_UnBindVideoLayer(dispLayer, dispDev);
    MI_DISP_Disable(dispDev);

//	MI_DIVP_StopChn(DIVP_CHN);
//	MI_DIVP_DestroyChn(DIVP_CHN);

    MI_SYS_Exit();
}

MI_S32 StartPlayVideo()
{
    MI_DISP_ShowInputPort(DISP_LAYER, DISP_INPUTPORT);
	return 0;
}

void StopPlayVideo()
{
    MI_DISP_HideInputPort(DISP_LAYER, DISP_INPUTPORT);
}

MI_S32 StartPlayAudio()
{
    MI_AUDIO_Attr_t stSetAttr;
    MI_AUDIO_Attr_t stGetAttr;
    MI_AUDIO_DEV AoDevId = AUDIO_DEV;
    MI_AO_CHN AoChn = AUDIO_CHN;

    MI_S32 s32SetVolumeDb;
    MI_S32 s32GetVolumeDb;

    //set Ao Attr struct
    memset(&stSetAttr, 0, sizeof(MI_AUDIO_Attr_t));
    stSetAttr.eBitwidth = E_MI_AUDIO_BIT_WIDTH_16;
    stSetAttr.eWorkmode = E_MI_AUDIO_MODE_I2S_MASTER;
    stSetAttr.u32FrmNum = 6;
    stSetAttr.u32PtNumPerFrm = AUDIO_SAMPLE_PER_FRAME;
    stSetAttr.u32ChnCnt = 1;

    if(stSetAttr.u32ChnCnt == 2)
    {
        stSetAttr.eSoundmode = E_MI_AUDIO_SOUND_MODE_STEREO;
    }
    else if(stSetAttr.u32ChnCnt == 1)
    {
        stSetAttr.eSoundmode = E_MI_AUDIO_SOUND_MODE_MONO;
    }

    stSetAttr.eSamplerate = E_MI_AUDIO_SAMPLE_RATE_48000;

    /* set ao public attr*/
    MI_AO_SetPubAttr(AoDevId, &stSetAttr);

    /* get ao device*/
    MI_AO_GetPubAttr(AoDevId, &stGetAttr);

    /* enable ao device */
    MI_AO_Enable(AoDevId);

    /* enable ao channel of device*/
    MI_AO_EnableChn(AoDevId, AoChn);


    /* if test AO Volume */
    s32SetVolumeDb = g_s32VolValue * (MAX_ADJUST_AO_VOLUME-MIN_ADJUST_AO_VOLUME) / 100 + MIN_ADJUST_AO_VOLUME;
    MI_AO_SetVolume(AoDevId, s32SetVolumeDb);
    MI_AO_SetMute(AoDevId, g_bMute);
    /* get AO volume */
    MI_AO_GetVolume(AoDevId, &s32GetVolumeDb);

	return 0;
}

void StopPlayAudio()
{
    MI_AUDIO_DEV AoDevId = AUDIO_DEV;
    MI_AO_CHN AoChn = AUDIO_CHN;

    /* disable ao channel of */
    MI_AO_DisableChn(AoDevId, AoChn);

    /* disable ao device */
    MI_AO_Disable(AoDevId);
}

static PLOGFONT CreateArialFont(unsigned size)
{
    return CreateLogFont(FONT_TYPE_NAME_SCALE_TTF, "arial", "ISO8859-1",
                         FONT_WEIGHT_REGULAR, FONT_SLANT_ROMAN,
                         FONT_FLIP_NIL, FONT_OTHER_NIL,
                         FONT_UNDERLINE_NONE, FONT_STRUCKOUT_NONE,
                         size, 0);
}


static PLOGFONT CreateFzcircleFont(unsigned size)
{
    return CreateLogFont(FONT_TYPE_NAME_SCALE_TTF, "fzcircle", "GB2312-0",
                         FONT_WEIGHT_REGULAR, FONT_SLANT_ROMAN,
                         FONT_FLIP_NIL, FONT_OTHER_NIL,
                         FONT_UNDERLINE_NONE, FONT_STRUCKOUT_NONE,
                         size, 0);
}

static PLOGFONT CreateTimesFont(unsigned size)
{
    return CreateLogFont(FONT_TYPE_NAME_SCALE_TTF, "times", "ISO8859-1",
                         FONT_WEIGHT_REGULAR, FONT_SLANT_ROMAN,
                         FONT_FLIP_NIL, FONT_OTHER_NIL,
                         FONT_UNDERLINE_NONE, FONT_STRUCKOUT_NONE,
                         size, 0);
}

int MiniGUIMain(int argc, const char* argv[])
{
    int not_used = 0;
    int i = 0;
    MSG Msg;
    mWidget* mainWnd;

    if (InitGUI (argc ,argv) != 0) {
        return 1;
    }

    ncsInitialize();
    ncs4TouchInitialize();
    mGEffInit();

    if (0 != LoadProjectPicture())
    {
        printf("open res file fail\n");
        goto deinit;
    }

    // detect usb device
    memset(g_pu8RootPath, 0, sizeof(g_pu8RootPath));
    if (MI_SUCCESS != ST_DetectUsbDev((char*)g_pu8RootPath, sizeof(g_pu8RootPath)))
    {
        printf("Please insert u disk first\n");
        goto unloadres;
    }

    // create filetree according to rootpath
    InitFileTreeRoot(&g_fileRoot, (char*)g_pu8RootPath);
    CreateFileTree(&g_fileRoot);
    //BrowseFileTree(&g_fileRoot);

#if 0
    // get ui layout from xml
    if (((access(UI_SURFACE_CFG_FILE, F_OK))!=-1) && 0 == ST_XmlPraseUiCfg((MI_U8*)UI_SURFACE_CFG_FILE, (MI_U8*)"PlayToolBar_LAYOUT", g_stPlayToolBarItem))
    {
        for (int i = 0; i < sizeof(g_stPlayToolBarItem)/sizeof(XmlRect_t); i++)
        {
            printf("index=%d (%d-%d-%d-%d)...\n", i,
                g_stPlayToolBarItem[i].s32X, g_stPlayToolBarItem[i].s32Y, g_stPlayToolBarItem[i].s16PicW, g_stPlayToolBarItem[i].s16PicH);
        }
    }
    else
        goto unloadres;
#endif

    memset(g_pu8FullPath, 0, sizeof(g_pu8FullPath));
    printf("root path is %s\n", (char*)g_pu8RootPath);

#if READ_INFO_FROM_XML
    // get last file path from xml
    if( (access((char*)g_pu8SelectPath, F_OK)) != -1 && strstr((char*)g_pu8SelectPath, (char*)g_pu8RootPath))
    {
        char *p = NULL;
        memcpy(g_pu8FullPath, g_pu8SelectPath, strlen((char*)g_pu8SelectPath));
        p = strrchr((char*)g_pu8FullPath, '/');
        *p = 0;
    }
    else
    {
        strcpy((char*)g_pu8FullPath, (char*)g_pu8RootPath);
    }
#else
    strcpy((char*)g_pu8FullPath, (char*)g_pu8RootPath);
#endif

    printf("init full path:%s\n", g_pu8FullPath);

    g_font_fft = CreateArialFont(12);
    //ft2SetLcdFilter(g_font_fft, MG_SMOOTH_DEFAULT);

    // init controller pos & size
    for (i = 0; i < sizeof(_playFileWnd_ctrl_tmpl)/sizeof(NCS_WND_TEMPLATE); i++)
    {
        _playFileWnd_ctrl_tmpl[i].x = g_stPlayToolBarItem[i].s32X;
        _playFileWnd_ctrl_tmpl[i].y = g_stPlayToolBarItem[i].s32Y;
        _playFileWnd_ctrl_tmpl[i].w = g_stPlayToolBarItem[i].s16PicW;
        _playFileWnd_ctrl_tmpl[i].h = g_stPlayToolBarItem[i].s16PicH;
    }

    voice_trk_props[2].value = g_s32VolValue;
    playFileWnd_tmpl.ctrls = _playFileWnd_ctrl_tmpl;
//    g_pPlayFileFont = createLocalFont(36);
    g_bShowPlayToolBar = FALSE;

	mainWnd = ncsCreateMainWindowIndirect(&playFileWnd_tmpl, HWND_DESKTOP);
    //mGEffInit();
    lstv_init(mainWnd);
    hMainPlayFileWnd = mainWnd->hwnd;
    SetMemDCColorKey((HDC)HDC_SCREEN, MEMDC_FLAG_SRCCOLORKEY, MakeRGBA(0x80, 0x80, 0x80, 0x80));
 
    while (GetMessage (&Msg, mainWnd->hwnd)) {
        TranslateMessage (&Msg);
        DispatchMessage (&Msg);
    }

    MainWindowThreadCleanup (mainWnd->hwnd);
    DestroyLogFont(g_font_fft);

unloadres:
    UnloadProjectPicture();

deinit:
    mGEffDeinit();
    ncs4TouchUninitialize();
    ncsUninitialize();
    TerminateGUI(not_used);

    return 0;
}

#ifdef _USE_MINIGUIENTRY
int main(int argc, const char *argv[])
{
    // init sdk
    CreatePlayerDev();
	printf("Welcome To MiniGUI!\n");
    main_entry(argc, argv);
    DestroyPlayerDev();

    return 0;
}
#endif
