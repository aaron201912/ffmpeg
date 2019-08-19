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
#include <minigui/ctrl/button.h>

#include "SAT070CP50_1024x600.h"
#include "usbdetect.h"
#include "frame.h"
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

#define TABLESIZE(table)    (sizeof(table)/sizeof(table[0]))

#define DIR_TYPE        "文件夹"
#define FILE_TYPE       "文件"
#define COL_DIV         16
#define COL_GAP         2
#define ROW_HEIGHT      40
#define COL_NUM         TABLESIZE(g_lv_caption)
#define FILE_FILTER_NUM         7

#define BN_NONE          0
#define BN_LEFT_CLICKED  1
#define BN_RIGHT_CLICKED 2

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
;

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
static char *g_pFileFilter[FILE_FILTER_NUM] = {".mp4", ".mpg", ".avi", ".wav", ".flv", ".mkv", ".mp3"/*, ".rmvb"*/};

// playing page
static MI_BOOL g_bShowPlayToolBar = FALSE;          // select file list page or playing page
static MI_BOOL g_bPlaying = FALSE;
static MI_BOOL g_bPause = FALSE;
static MI_BOOL g_bMute = FALSE;
static MI_BOOL g_jumpflag = FALSE;
static MI_U8 g_button_status = BN_NONE;
static MI_U8 g_timer_task[2];
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
static BITMAP g_musiclogo;

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

static Rect_t g_stPlayToolBarItem[] = {
    {   0,   0,  50,  50}, { 50,   0,  14,  50}, { 64,   0, 960, 50}, {  0,  50, 1024, 550},
    {   0, 520,1024,  80}, { 20, 520, 984,  30}, { 20, 550,  80,  48}, {105, 550,   80,  48},
    { 190, 550,  80,  48}, {275, 550,  80,  48}, {360, 550,  80, 48}, {530, 559,  32,  32},
    { 570, 550, 260,  50}, {840, 550, 150,   20}, {830, 550, 20,  50 }, {  0,   0,1024, 520 },
    { 984,  0,   40,  40}
};
#endif

// font
//static PLOGFONT g_pPlayFileFont = NULL;
static PLOGFONT CreateArialFont(unsigned size);
static PLOGFONT createLocalFont(unsigned size);
static PLOGFONT CreateFzcircleFont(unsigned size);
static PLOGFONT CreateTimesFont(unsigned size);

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
static void display_notify(mWidget *button, int id, int nc, DWORD add_data);

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
//	{BN_CLICKED, playslow_btn_notify},
//	{BN_DBLCLK, playslow_btn_notify},
    {0, NULL}
};

static NCS_EVENT_HANDLER playfast_btn_handlers [] = {
	NCS_MAP_NOTIFY(NCSN_WIDGET_CLICKED, playfast_btn_notify),
//	{BN_CLICKED, playfast_btn_notify},
//	{BN_DBLCLK, playfast_btn_notify},
    {0, NULL}
};

static NCS_EVENT_HANDLER mute_btn_handlers [] = {
    NCS_MAP_NOTIFY(NCSN_WIDGET_CLICKED, mute_btn_notify),
    {0, NULL}
};

static NCS_EVENT_HANDLER play_trk_handlers [] = {
	NCS_MAP_NOTIFY(NCSN_TRKBAR_CHANGED, play_trk_notify),
//	{NCSN_TRKBAR_CHANGED, play_trk_notify},
    {0, NULL}
};

static NCS_EVENT_HANDLER voice_trk_handlers [] = {
    NCS_MAP_NOTIFY(NCSN_TRKBAR_CHANGED, voice_trk_notify),
    {0, NULL}
};

static NCS_EVENT_HANDLER display_handles[] = {
	NCS_MAP_NOTIFY(NCSN_WIDGET_CLICKED, display_notify),
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
    {NCSP_TRKBAR_MAXPOS, VOLUME_RANGE},
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
        0
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
		WS_VISIBLE,
        WS_EX_TRANSPARENT,
        "",
        NULL,
        NULL,
		timer_handles,
        NULL,
        0,
        0
    },

	// audio volume
    {
        NCSCTRL_STATIC,
        IDC_PLAYFILE_STATIC_VOLUME,
        780, 550, 50, 30,        
		WS_VISIBLE,
        WS_EX_TRANSPARENT,
        "",
        NULL,
        NULL,
		NULL,
        NULL,
        0,
        0
    },
    
    // music logo
    {
        NCSCTRL_BUTTON,
        IDC_PLAYFILE_BUTTON_DISPLAY,
        0, 0, 1020, 520,        
		NCSS_NOTIFY | NCSS_BUTTON_IMAGE,
        WS_EX_NONE,
        "",
        NULL,
        NULL,
		display_handles,
        NULL,
        0,
        0
    },
    // video fps label
    {
        NCSCTRL_STATIC,
        IDC_PLAYFILE_STATIC_FPS,
        984, 0, 40, 40,        
		WS_VISIBLE,
        WS_EX_TRANSPARENT,
        "",
        NULL,
        NULL,
		NULL,
        NULL,
        0,
        0
    }
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
    	//printf("FileTree is empty\n");
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


// stay in playing page, clear play status
MI_S32 PlayComplete()
{
    mButton *pPlayObj = (mButton*)ncsGetChildObj(hMainPlayFileWnd, IDC_PLAYFILE_BUTTON_PLAY_PAUSE);
    mStatic *pTimeObj = (mStatic*)ncsGetChildObj(hMainPlayFileWnd, IDC_PLAYFILE_STATIC_PLAY_TIME);
	mTrackBar *pProcessObj = (mStatic*)ncsGetChildObj(hMainPlayFileWnd, IDC_PLAYFILE_TRACKBAR_PLAY_PROGRESS);

	printf("video or audio play complete!\n");
	printf("video pts : %.2f, audio pts : %.2f.\n", g_pstPlayStat->video_clk.pts, g_pstPlayStat->audio_clk.pts);

	init_clock(&g_pstPlayStat->video_clk, &g_pstPlayStat->video_pkt_queue.serial);
    init_clock(&g_pstPlayStat->audio_clk, &g_pstPlayStat->audio_pkt_queue.serial);
	g_pstPlayStat->eof = 0;
	
   	toggle_pause(g_pstPlayStat);
    g_bPlaying = FALSE;
    g_bPause   = TRUE;

	// 文件流回到开始位置,暂定等待
	g_pstPlayStat->seek_flags = AVSEEK_FLAG_FRAME;
//	printf("start_time of audio file : %lld.\n", g_pstPlayStat->p_fmt_ctx->start_time);
	stream_seek(g_pstPlayStat, g_pstPlayStat->p_fmt_ctx->start_time, 0, 0);
	
    SetBtnImg(pPlayObj, &g_play_btn);
    ResetPlayingStatus();

	memcpy(time_info, "00:00:00", 8);
	_c(pTimeObj)->setProperty(pTimeObj, NCSP_STATIC_ALIGN, NCS_ALIGN_RIGHT);
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
    // stop play video & audio
    g_bPlaying = FALSE;
    g_bPause = FALSE;
	printf("error in playing!\n");
    // sendmessage to stop playing
    player_deinit(g_pstPlayStat);
	sstar_ao_deinit();

    SetBtnImg(pPlayObj, &g_play_btn);
    ResetPlayingStatus();
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
	mStatic *pVolumeObj = (mStatic*)ncsGetChildObj(hMainPlayFileWnd, IDC_PLAYFILE_STATIC_VOLUME);
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
//	mButton *pDisplayObj = (mButton *)ncsGetChildObj(hMainPlayFileWnd, IDC_PLAYFILE_BUTTON_DISPLAY);
	mStatic *pFpsObj = (mStatic *)ncsGetChildObj(hMainPlayFileWnd, IDC_PLAYFILE_STATIC_FPS);				

	printf("show playfile page\n");

    if (bShow)
    {
    	IncludeWindowStyle(pVolumeObj->hwnd, WS_VISIBLE);
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
//		IncludeWindowStyle(pDisplayObj->hwnd, WS_VISIBLE);
		IncludeWindowStyle(pFpsObj->hwnd, WS_VISIBLE);
    }
    else
    {
    	ExcludeWindowStyle(pVolumeObj->hwnd, WS_VISIBLE);
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
//		ExcludeWindowStyle(pDisplayObj->hwnd, WS_VISIBLE);
		ExcludeWindowStyle(pFpsObj->hwnd, WS_VISIBLE);
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
    is->playerController.fpGetMediaInfo      = GetMediaInfo;
    is->playerController.fpGetCurrentPlayPos = GetCurrentPlayPos;
    is->playerController.fpPlayComplete      = PlayComplete;
    is->playerController.fpPlayError         = PlayError;
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
    logfont  = GetWindowFont(self->hwnd);
    GetFontMetrics(logfont, &fm);
    printf("old font size is max_w:%d avg_w:%d h:%d\n", fm.max_width, fm.ave_width, fm.font_height);

//	SelectFont(clientDc, g_pPlayFileFont);
    logfont  = GetWindowFont(self->hwnd);
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
        mButton *pPlayObj = (mButton *)ncsGetChildObj(hMainPlayFileWnd, IDC_PLAYFILE_BUTTON_PLAY_PAUSE);
        mTrackBar *pVoiceObj = (mTrackBar*)ncsGetChildObj(hMainPlayFileWnd, IDC_PLAYFILE_TRACKBAR_VOICE);
        mTrackBar *pProgressObj = (mTrackBar *)ncsGetChildObj(hMainPlayFileWnd, IDC_PLAYFILE_TRACKBAR_PLAY_PROGRESS);
		mStatic *pTimeObj = (mStatic *)ncsGetChildObj(hMainPlayFileWnd, IDC_PLAYFILE_STATIC_PLAY_TIME);
		mStatic *pVolumeObj = (mStatic *)ncsGetChildObj(hMainPlayFileWnd, IDC_PLAYFILE_STATIC_VOLUME);
		mButton *pDisplayObj = (mButton *)ncsGetChildObj(hMainPlayFileWnd, IDC_PLAYFILE_BUTTON_DISPLAY);
		mStatic *pFpsObj = (mStatic*)ncsGetChildObj(hMainPlayFileWnd, IDC_PLAYFILE_STATIC_FPS);
		NCS_LISTV_ITEMINFO info;
        FileTree_t *curFileNode = NULL;

        if (!pathObj) return;

        // confirm btn is disable utill selected file
        if (!strcmp(type, FILE_TYPE))
        {
            // save selectpath & play file
            memset(g_pu8SelectPath, 0, sizeof(g_pu8SelectPath));
            sprintf((char*)g_pu8SelectPath, "%s/%s", (char*)g_pu8FullPath, fileName);
			// malloc player context
            g_pstPlayStat = player_init((const char*)g_pu8SelectPath);
            if (!g_pstPlayStat)
            {
            	printf("[ERROR] Initilize player failed!\n");
                return;
            }
			printf("video file name is : %s\n", g_pstPlayStat->filename);
	
            // sendmessage to play file
            g_bPlaying = TRUE;
            g_bPause   = FALSE;
			g_pstPlayStat->paused = false;
			g_pstPlayStat->seek_flags = AVSEEK_FLAG_FRAME;
            g_pstPlayStat->out_width  = MAINWND_W;
#if SHOW_PLAY_TOOL_BAR
            g_pstPlayStat->out_height = MAINWND_H - g_stPlayToolBarItem[4].s16PicH;
#else
            g_pstPlayStat->out_height = MAINWND_H;
#endif
			ResetPlayingStatus();
			SetPlayerControlCallBack(g_pstPlayStat);

            // initilize ssplayer		
            if (0 > open_demux(g_pstPlayStat)) {		
				printf("open %s failed!\n", g_pstPlayStat->filename);
				return ;
            }
			if (g_pstPlayStat->video_idx >= 0) {
				//sstar_disp_init(g_pstPlayStat->out_width, g_pstPlayStat->out_height);
				MI_DISP_ShowInputPort(DISP_LAYER, DISP_INPUTPORT);
            	open_video(g_pstPlayStat);
			}
			if (g_pstPlayStat->audio_idx >= 0) {
				sstar_ao_init(g_s32VolValue);
            	open_audio(g_pstPlayStat);	
			}

			// redraw gui display
			MI_U32 second, minute, hours;

			// switch gui bmp
			if (g_pstPlayStat->audio_idx >= 0 && g_pstPlayStat->video_idx < 0)
			{
				IncludeWindowStyle(pDisplayObj->hwnd, WS_VISIBLE);
			}
			show_lv_page(FALSE);
            show_playfile_page(TRUE);
			g_bShowPlayToolBar = TRUE;
					
            SetBtnImg(pPlayObj, &g_pause_btn);
			
			PLOGFONT curFont = CreateLogFont("rbf", "fixed", "GB2312", 'm', 'r', 'n', 'c', 'n', 'n', 32, 0);
			SetWindowFont (pFpsObj->hwnd, curFont);
			
			char text[3];
			sprintf(text, "%d", g_s32VolValue);
			_c(pVolumeObj)->setProperty(pVolumeObj, NCSP_STATIC_ALIGN, NCS_ALIGN_CENTER);
			_c(pVolumeObj)->setProperty(pVolumeObj, NCSP_WIDGET_TEXT, (DWORD)text);
            _c(pVoiceObj)->setProperty(pVoiceObj, NCSP_TRKBAR_CURPOS, g_s32VolValue);			
			_c(pProgressObj)->setProperty(pProgressObj, NCSP_TRKBAR_CURPOS, 0);
			InvalidateRect(hMainPlayFileWnd, NULL, TRUE);
			
			if (g_pstPlayStat->p_fmt_ctx->duration > 0)
			{
				second = g_pstPlayStat->p_fmt_ctx->duration / AV_TIME_BASE;
				minute = second / 60;
				hours  = minute / 60;
				sprintf(time_info, "--:--:--/%02d:%02d:%02d", (hours % 24), (minute % 60), (second % 60));
			}

			_c(pTimeObj)->setProperty(pTimeObj, NCSP_STATIC_ALIGN, NCS_ALIGN_RIGHT);
			_c(pTimeObj)->setProperty(pTimeObj, NCSP_WIDGET_TEXT, (DWORD)time_info);
			InvalidateRect(pTimeObj->hwnd, NULL, TRUE);
			// 初始化1秒定时器
			//SetTimer((mWidget *)pTimeObj->hwnd, IDC_PLAYFILE_STATIC_PLAY_TIME, 100);
			SetTimer((mWidget *)pTimeObj->hwnd, IDC_PLAYFILE_STATIC_PLAY_TIME, 10);
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
	mListView *lvObj  = (mListView*)ncsGetChildObj(GetParent(button->hwnd), IDC_PLAYFILE_LISTVIEW_FILELIST);
	mStatic *pathObj  = (mStatic*)ncsGetChildObj(GetParent(button->hwnd), IDC_PLAYFILE_STATIC_DIR_NAME);
	mButton *pPlayObj = (mButton*)ncsGetChildObj(GetParent(button->hwnd), IDC_PLAYFILE_BUTTON_PLAY_PAUSE);
    NCS_LISTV_ITEMINFO info;
    int i = 0, color = 0x00FFFF;

    printf("Clicked updir btn\n");
    // do recollection stuff
    if (!button)
		return;

    if(nc == NCSN_WIDGET_CLICKED)
    {
        if (g_bShowPlayToolBar)
        {
            // sendmessage to stop playing file
            g_bShowPlayToolBar = FALSE;

            player_deinit(g_pstPlayStat);

			ResetPlayingStatus();

        	show_playfile_page(FALSE);
            show_lv_page(TRUE); 
			SetBtnImg(pPlayObj, &g_play_btn);   
			InvalidateRect(hMainPlayFileWnd, NULL, TRUE);
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
		if (g_bShowPlayToolBar)
		{
			toggle_pause(g_pstPlayStat);            // 判断并设置暂停/播放状态
			printf("video pause status : %d.\n", g_pstPlayStat->paused);
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
}

static void stop_btn_notify(mWidget *button, int id, int nc, DWORD add_data)
{
    mButton *pPlayObj = (mButton*)ncsGetChildObj(GetParent(button->hwnd), IDC_PLAYFILE_BUTTON_PLAY_PAUSE);
	mStatic *pTimeObj = (mStatic*)ncsGetChildObj(GetParent(button->hwnd), IDC_PLAYFILE_STATIC_PLAY_TIME);
	mButton *pDisplayObj = (mButton *)ncsGetChildObj(GetParent(button->hwnd), IDC_PLAYFILE_BUTTON_DISPLAY);

	printf("Clicked stop btn\n");

    if (!button)
		return;

    if(nc == NCSN_WIDGET_CLICKED)
    {
    	if (g_bShowPlayToolBar)
    	{
	    	// sendmessage to stop playing
	        g_bPlaying = FALSE;
	        g_bPause   = FALSE;
			g_bShowPlayToolBar = FALSE;
			// 注销1秒定时器
			KillTimer(pTimeObj->hwnd, IDC_PLAYFILE_STATIC_PLAY_TIME);

			if (g_pstPlayStat->audio_idx >= 0 && g_pstPlayStat->video_idx < 0)
			{
				ExcludeWindowStyle(pDisplayObj->hwnd, WS_VISIBLE);
			}

			player_deinit(g_pstPlayStat);

			ResetPlayingStatus();

			show_playfile_page(FALSE);
	        show_lv_page(TRUE);
			SetBtnImg(pPlayObj, &g_play_btn);
			InvalidateRect(hMainPlayFileWnd, NULL, TRUE);
    	}
    }
}

static void playslow_btn_notify(mWidget *button, int id, int nc, DWORD add_data)
{
    mStatic *pSpeedModeObj = (mStatic*)ncsGetChildObj(GetParent(button->hwnd), IDC_PLAYFILE_STATIC_SPEED_MODE);
//	char speedMode[16] = {0};
//	double incr = -5.0, pos;

    if (!button || !pSpeedModeObj)
		return;

    if (!g_bPlaying)
		return;

	if (nc = NCSN_WIDGET_CLICKED)
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

//		if (g_bShowPlayToolBar)
//		{
//			pos = get_master_clock(g_pstPlayStat);
//	        if (isnan(pos))
//	            pos = (double)g_pstPlayStat->seek_pos / AV_TIME_BASE;
//	        pos += incr;
//	        if (g_pstPlayStat->p_fmt_ctx->start_time != AV_NOPTS_VALUE && pos < g_pstPlayStat->p_fmt_ctx->start_time / (double)AV_TIME_BASE)
//	            pos = g_pstPlayStat->p_fmt_ctx->start_time / (double)AV_TIME_BASE;
//			g_pstPlayStat->seek_flags = AVSEEK_FLAG_FRAME;
//			stream_seek(g_pstPlayStat, (int64_t)(pos * AV_TIME_BASE), (int64_t)(incr * AV_TIME_BASE), 0);
//		}

		if (g_button_status == BN_NONE)
		{
			g_button_status = BN_LEFT_CLICKED;
			g_timer_task[0] = 0;
		}
		else if (g_button_status == BN_LEFT_CLICKED)
		{
			g_button_status = BN_NONE;
			printf("double click playslow button!\n");
		}
		else ;
	}
}

static void playfast_btn_notify(mWidget *button, int id, int nc, DWORD add_data)
{
	mStatic *pSpeedModeObj = (mStatic*)ncsGetChildObj(GetParent(button->hwnd), IDC_PLAYFILE_STATIC_SPEED_MODE);
//	char speedMode[16] = {0};
//	double incr = 5.0, pos;

    // do recollection stuff
    if (!button || !pSpeedModeObj)
		return;

    if (!g_bPlaying)
		return;
	
	if (nc == NCSN_WIDGET_CLICKED)
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
			
//		PLOGFONT curFont = GetCurFont(HDC_SCREEN);
//		printf("curFont: %s-%s-%s-size%d\n", curFont->type, curFont->family, curFont->charset, curFont->size);
//		SelectFont(HDC_SCREEN, g_font_fft);
//		SetBkMode (HDC_SCREEN, BM_TRANSPARENT);
//		TextOut (HDC_SCREEN, 600, 520, "hahaha,哈哈哈");
//		curFont = GetCurFont(HDC_SCREEN);
//		printf("select Font: %s-%s-%s-size%d-regular\n", g_font_fft->type, g_font_fft->family, g_font_fft->charset, g_font_fft->size);
//		printf("curFont: %s-%s-%s-size%d\n", curFont->type, curFont->family, curFont->charset, curFont->size);

//		if (g_bShowPlayToolBar)
//		{
//			pos = get_master_clock(g_pstPlayStat);

//			if (isnan(pos))
//				pos = (double)g_pstPlayStat->seek_pos / AV_TIME_BASE;
//			pos += incr;   // 每次快进5秒
//			if (g_pstPlayStat->p_fmt_ctx->start_time != AV_NOPTS_VALUE && pos > g_pstPlayStat->p_fmt_ctx->duration / (double)AV_TIME_BASE)
//				pos = g_pstPlayStat->p_fmt_ctx->duration / (double)AV_TIME_BASE;
//			g_pstPlayStat->seek_flags = AVSEEK_FLAG_FRAME;
//			stream_seek(g_pstPlayStat, (int64_t)(pos * AV_TIME_BASE), (int64_t)(incr * AV_TIME_BASE), 0);
//		}
		if (g_button_status == BN_NONE)
		{
			g_button_status = BN_RIGHT_CLICKED;
			g_timer_task[0] = 0;
			//printf("sigle click playfast button!\n")
		}
		else if (g_button_status == BN_RIGHT_CLICKED) 
		{
			g_button_status = BN_NONE;
			printf("double click playfast button!\n");
		}
		else ;
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
    	if (g_bShowPlayToolBar)
    	{
	        if (g_s32VolValue)
	            vol = g_s32VolValue * (MAX_AO_VOLUME - MIN_AO_VOLUME) / VOLUME_RANGE + MIN_AO_VOLUME;
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
}

static void play_trk_notify(mTrackBar* self, int id, int code, DWORD add_data)
{
	mTrackBar *pVideoObj = (mTrackBar*)self;
	MI_S32 t_value;
	int64_t position, offset;
	double s_time;

	if (!pVideoObj)
		return;

	switch (code)
	{
		case NCSN_TRKBAR_CHANGED:
			if (g_bShowPlayToolBar) {
				t_value  = _c(pVideoObj)->getProperty(pVideoObj, NCSP_TRKBAR_CURPOS);
				position = g_pstPlayStat->p_fmt_ctx->duration / play_trk_props[1].value * t_value;
				if (g_pstPlayStat->video_idx >= 0)
					s_time = g_pstPlayStat->video_clk.pts;      // 获取当前时间戳
				else if (g_pstPlayStat->audio_idx >= 0)
					s_time = g_pstPlayStat->audio_clk.pts;
				if (position >= 0 && position <= g_pstPlayStat->p_fmt_ctx->duration) 
				{
					position += g_pstPlayStat->p_fmt_ctx->start_time;				
					offset = position - s_time * AV_TIME_BASE;
					// 当跳转进度大于3秒时才执行跳转,防止破坏正常进度进行
					if (offset > 3000000 || offset < -3000000)
					{
						g_jumpflag = TRUE;
						g_pstPlayStat->seek_flags = AVSEEK_FLAG_FRAME;
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
	mStatic *pVolumeObj = (mStatic *)ncsGetChildObj(hMainPlayFileWnd, IDC_PLAYFILE_STATIC_VOLUME);	
    MI_S32 vol = 0;
    MI_AO_ChnState_t stAoState;
	
	//printf("voice trackbar notify\n");

    if (!pVoiceObj)
		return;

	switch (code)
	{
    	case NCSN_TRKBAR_CHANGED :
			if (g_bShowPlayToolBar) {
				char text[3];	
		        g_s32VolValue = _c(pVoiceObj)->getProperty(pVoiceObj, NCSP_TRKBAR_CURPOS);
				sprintf(text, "%d", g_s32VolValue);
				_c(pVolumeObj)->setProperty(pVolumeObj, NCSP_STATIC_ALIGN, NCS_ALIGN_CENTER);
				_c(pVolumeObj)->setProperty(pVolumeObj, NCSP_WIDGET_TEXT, (DWORD)text);
				InvalidateRect(pVolumeObj->hwnd, NULL, FALSE);			

				printf("audio track value : %d.\n", g_s32VolValue);

				if (g_s32VolValue)
		            vol = g_s32VolValue * (MAX_AO_VOLUME - MIN_AO_VOLUME) / VOLUME_RANGE + MIN_AO_VOLUME;
		        else
		            vol = MIN_AO_VOLUME;

		        memset(&stAoState, 0, sizeof(MI_AO_ChnState_t));
		        if (MI_SUCCESS == MI_AO_QueryChnStat(AUDIO_DEV, AUDIO_CHN, &stAoState))
		        {
		            MI_AO_SetVolume(AUDIO_DEV, vol);
		            MI_AO_SetMute(AUDIO_DEV, g_bMute);
		        }
			}
		break;
			
		default : break;
    }
}

static BOOL onTimer(mWidget *widget, int message, int id, DWORD tick)
{
	g_timer_task[0] ++;
	g_timer_task[1] ++;

	if (g_timer_task[0] >= 4)             // 双击间隔时间设置为400ms
	{
		double incr, pos;
		
		g_timer_task[0] = 0;
		switch (g_button_status)
		{
			case BN_NONE : break;
			
			case BN_LEFT_CLICKED :
				if (g_bShowPlayToolBar && !g_pstPlayStat->paused)
				{
					incr = -5.0;
					pos = get_master_clock(g_pstPlayStat);
					if (isnan(pos))
						pos = (double)g_pstPlayStat->seek_pos / AV_TIME_BASE;
					pos += incr;
					if (g_pstPlayStat->p_fmt_ctx->start_time != AV_NOPTS_VALUE && pos < g_pstPlayStat->p_fmt_ctx->start_time / (double)AV_TIME_BASE)
						pos = g_pstPlayStat->p_fmt_ctx->start_time / (double)AV_TIME_BASE;
					g_pstPlayStat->seek_flags = AVSEEK_FLAG_FRAME;
					stream_seek(g_pstPlayStat, (int64_t)(pos * AV_TIME_BASE), (int64_t)(incr * AV_TIME_BASE), 0);
				}
			break;

			case BN_RIGHT_CLICKED :			
				if (g_bShowPlayToolBar && !g_pstPlayStat->paused)
				{
					incr = 5.0;
					pos = get_master_clock(g_pstPlayStat);

					if (isnan(pos))
						pos = (double)g_pstPlayStat->seek_pos / AV_TIME_BASE;
					pos += incr;   // 每次快进5秒
					if (g_pstPlayStat->p_fmt_ctx->start_time != AV_NOPTS_VALUE && pos > g_pstPlayStat->p_fmt_ctx->duration / (double)AV_TIME_BASE)
						pos = g_pstPlayStat->p_fmt_ctx->duration / (double)AV_TIME_BASE;
					g_pstPlayStat->seek_flags = AVSEEK_FLAG_FRAME;
					stream_seek(g_pstPlayStat, (int64_t)(pos * AV_TIME_BASE), (int64_t)(incr * AV_TIME_BASE), 0);
				}
			break;

			default : break;
		}
		g_button_status = BN_NONE;
	}

	if (g_timer_task[1] >= 10)             // 1s定时任务
	{
		double time;
		char tmpstr[] = "--:--:--";
		char fpsstr[] = "00.0";
		uint32_t second, minute, hours;
		mStatic *pTimeObj = (mStatic *)ncsGetChildObj(hMainPlayFileWnd, IDC_PLAYFILE_STATIC_PLAY_TIME);
		mTrackBar *pProgressObj = (mTrackBar*)ncsGetChildObj(hMainPlayFileWnd, IDC_PLAYFILE_TRACKBAR_PLAY_PROGRESS);
		mStatic *pFpsObj = (mStatic*)ncsGetChildObj(hMainPlayFileWnd, IDC_PLAYFILE_STATIC_FPS);
	
		g_timer_task[1] = 0;
		if (g_pstPlayStat->video_idx >= 0)
			time = g_pstPlayStat->video_clk.pts;
		else if (g_pstPlayStat->audio_idx >= 0)
			time = g_pstPlayStat->audio_clk.pts;
		second = (uint32_t)time;
		minute = second / 60;
		hours  = minute / 60;
		sprintf(tmpstr, "%02d:%02d:%02d", (hours % 24), (minute % 60), (second % 60));
		memcpy(time_info, tmpstr, 8);

		// 重绘GUI
		_c(pTimeObj)->setProperty(pTimeObj, NCSP_STATIC_ALIGN, NCS_ALIGN_RIGHT);
		_c(pTimeObj)->setProperty(pTimeObj, NCSP_WIDGET_TEXT, (DWORD)time_info);

		uint32_t total_time = g_pstPlayStat->p_fmt_ctx->duration / AV_TIME_BASE;
		uint32_t t_value = (WPARAM)second * play_trk_props[1].value / total_time;
		uint32_t tmp_value = _c(pProgressObj)->getProperty(pProgressObj, NCSP_TRKBAR_CURPOS);

		if (g_jumpflag == TRUE) {
			g_jumpflag = FALSE;
			_c(pProgressObj)->setProperty(pProgressObj, NCSP_TRKBAR_CURPOS, tmp_value);
		}
		else
			_c(pProgressObj)->setProperty(pProgressObj, NCSP_TRKBAR_CURPOS, t_value);

		if (g_pstPlayStat->paused) 
			sprintf(fpsstr, "%.1f", 0.0);
		else
			sprintf(fpsstr, "%.1f", g_pstPlayStat->fps);
			
		_c(pFpsObj)->setProperty(pFpsObj, NCSP_STATIC_ALIGN, NCS_ALIGN_CENTER);
		_c(pFpsObj)->setProperty(pFpsObj, NCSP_WIDGET_TEXT, (DWORD)fpsstr);
		InvalidateRect(hMainPlayFileWnd, NULL, FALSE);
	}

	return TRUE;
}

static void display_notify(mWidget *button, int id, int nc, DWORD add_data)
{
	mWidget *pDiaplayObj = (mWidget*)button;

	if (!pDiaplayObj)
		return;
	
	switch (nc)
	{
		case NCSN_WIDGET_CLICKED :
			printf("click display windows!\n");
		break;

		default : break;
	}
}

static void playFileWnd_notify(mWidget *self, int message, int code, DWORD usrData)
{
    if (message == MSG_CLOSE)
    {
        printf("playFileWnd close...\n");
//		DestroyLogFont(g_pPlayFileFont);
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
	mStatic *pFpsObj = (mStatic *)ncsGetChildObj(self->hwnd, IDC_PLAYFILE_STATIC_FPS);
    mTrackBar *pVoiceObj = (mTrackBar *)ncsGetChildObj(self->hwnd, IDC_PLAYFILE_TRACKBAR_VOICE);
	mTrackBar *pProgressObj = (mTrackBar *)ncsGetChildObj(self->hwnd, IDC_PLAYFILE_TRACKBAR_PLAY_PROGRESS);
    mStatic *pVolumeObj = (mStatic *)ncsGetChildObj(self->hwnd, IDC_PLAYFILE_STATIC_VOLUME);
	mButton *pDisplayObj = (mButton *)ncsGetChildObj(self->hwnd, IDC_PLAYFILE_BUTTON_DISPLAY);

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
	SetBtnImg(pDisplayObj, &g_musiclogo);

	_c(pSpeedModeObj)->setProperty(pSpeedModeObj, NCSP_STATIC_ALIGN, NCS_ALIGN_CENTER);
	_c(pFpsObj)->setProperty(pFpsObj, NCSP_STATIC_ALIGN, NCS_ALIGN_CENTER);
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
	if (LoadBitmap (HDC_SCREEN, &g_musiclogo, "appres/music_logo.png"))
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
	UnloadBitmap(&g_musiclogo);
}

static PLOGFONT createLocalFont(unsigned size)
{
    return CreateLogFont("ttf", "helvetica", "GB2312",
                         FONT_WEIGHT_BOOK, FONT_SLANT_ROMAN,
                         FONT_SETWIDTH_NORMAL, FONT_OTHER_AUTOSCALE,
                         FONT_UNDERLINE_NONE, FONT_STRUCKOUT_NONE,
                         size, 0);
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

	printf("get in MiniGUI Main!\n");
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

    g_font_fft = CreateArialFont(16);
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
    playFileWnd_tmpl.ctrls   = _playFileWnd_ctrl_tmpl;
    g_bShowPlayToolBar       = FALSE;

	mainWnd = ncsCreateMainWindowIndirect(&playFileWnd_tmpl, HWND_DESKTOP);

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
	printf("Welcome To MiniGUI!\n");
    sstar_enable_display(&stPanelParam, MAINWND_W, MAINWND_H - g_stPlayToolBarItem[4].s16PicH);
    main_entry(argc, argv);
    sstar_disable_display();
	printf("Exit MiniGUI!\n");
    return 0;
}
#endif

