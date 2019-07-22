/**
 * \file mgutils.h
 * \date 2007/12/26
 * 
 * \brief This file includes interfaces of mGUtils. 
 *
 \verbatim

    This file is part of mGUtils, a component for MiniGUI.

    Copyright (C) 2002~2018, Beijing FMSoft Technologies Co., Ltd.
    Copyright (C) 1998~2002, WEI Yongming

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

    Or,

    As this program is a library, any link to this program must follow
    GNU General Public License version 3 (GPLv3). If you cannot accept
    GPLv3, you need to be licensed from FMSoft.

    If you have got a commercial license of this program, please use it
    under the terms and conditions of the commercial license.

    For more information about the commercial license, please refer to
    <http://www.minigui.com/en/about/licensing-policy/>.

 \endverbatim
 */

/*
 * $Id: mgutils.h 231 2017-10-26 07:25:36Z weiym $
 *
 *      mGUtils: A MiniGUI component which contains miscellaneous 
 *      utilities like ColorSelectionDialogBox, FileOpenDialogBox, and so on. 
 *
 */

#ifndef _MGUTILS_H
#define _MGUTILS_H

#ifdef  __cplusplus
extern "C" {
#endif

#if !defined(__NODLL__) && (defined (WIN32) || defined (__NUCLEUS_MNT__))
  #ifdef __MGUTILS_LIB__
  #define MGUTILS_EXPORT       __declspec(dllexport)
  #else
  #define MGUTILS_EXPORT       __declspec(dllimport) 
  #endif
#elif defined(__NODLL__) && defined(WIN32)
  #define MGUTILS_EXPORT       __declspec(dllimport) 
#else
  #define MGUTILS_EXPORT       
#endif

#ifndef MGUTILS_MAJOR_VERSION
#   undef PACKAGE
#   undef VERSION
#   undef PACKAGE_BUGREPORT
#   undef PACKAGE_NAME
#   undef PACKAGE_STRING
#   undef PACKAGE_TARNAME
#   undef PACKAGE_VERSION
#   ifdef __MGUTILS_LIB__
#       include "../mgutilsconfig.h"
#   else
#       include "mgutilsconfig.h"
#   endif
#endif

#define MGUTILS_MSG_BASE            MSG_USER + 0x40
#define MSG_FILESELOK               (MGUTILS_MSG_BASE + 1)
#define MSG_FILESELCANCEL           (MGUTILS_MSG_BASE + 2)
#define MSG_COLORSELOK              (MGUTILS_MSG_BASE + 3)
#define MSG_COLORSELCANCEL          (MGUTILS_MSG_BASE + 4)
#define MSG_FONTSELOK               (MGUTILS_MSG_BASE + 5)
#define MSG_FONTSELCANCEL           (MGUTILS_MSG_BASE + 6)

#ifdef _MGUTILS_MYWINS

    /**
     * \addtogroup mywins_fns Interfaces of MyWins module of mGUtils library
     *
     * This module defines some useful helpers, such as tool-tip window,
     * progress window, and so on.
     *
     * @{
     */

/**
 * \fn int myMessageBox (HWND hwnd, DWORD dwStyle, const char* title,\
                const char* text, ...)
 * \brief Creates a message box.
 *
 * This function creates a message box calling \a MessageBox and passing
 * \a hwnd, \a dwStyle and \a title to it. This function also receives \a 
 * printf-like arguments to format a string.
 *
 * \param hwnd The hosting main window.
 * \param dwStyle The style of the message box.
 * \param title The title of the message box.
 * \param text The format string.
 *
 * \return Identifier of the button which closes the message box.
 *
 * \sa MessageBox, printf(3)
 */
MGUTILS_EXPORT int myMessageBox (HWND hwnd, DWORD dwStyle, const char* title,
                const char* text, ...);

/**
 * \fn int myWinMessage (HWND hwnd, const char* title, \
                const char* button1, const char* text, ...)
 * \brief Creates a message box within only one button.
 *
 * This function creates a message box hosted to the main window \a hwnd,
 * displays a message and an application icon in the message box, and creates
 * a button which can be used to close the box. This function also 
 * receives \a printf-like arguments to format a string.
 *
 * This function can be used to display a information for the user.
 *
 * \param hwnd The hosting main window.
 * \param title The title of the message box.
 * \param button1 The text in the button.
 * \param text The format string.
 *
 * \return 0 indicates the message box was closed by clicking the only button.
 *
 * \sa myWinChoice, printf(3)
 */
MGUTILS_EXPORT int myWinMessage (HWND hwnd, const char* title, 
                const char* button1, const char* text, ...);

/**
 * \fn int myWinChoice (HWND hwnd, const char* title, \
                const char* button1, const char* button2, \
                const char* text, ...)
 * \brief Creates a message box within two buttons.
 *
 * This function creates a message box hosted to the main window \a hwnd, 
 * displays a message and an application icon in the message box, and creates
 * two buttons in it. This function also receives \a printf-like arguments 
 * to format a string.
 *
 * This function can be used to prompt the user to choose one item 
 * between two.
 *
 * \param hwnd The hosting main window.
 * \param title The title of the message box.
 * \param button1 The title of the first button.
 * \param button2 The title of the second button.
 * \param text The format string.
 *
 * \return Either 0 or 1, indicates the message box was closed by 
 *         the first or second button.
 *
 * \sa myWinTernary, printf(3)
 */
MGUTILS_EXPORT int myWinChoice (HWND hwnd, const char* title, 
                const char* button1, const char* button2, 
                const char* text, ...);

/**
 * \fn int myWinTernary (HWND hwnd, const char* title, \
                const char* button1, const char* button2, const char* button3, \
                const char* text, ...)
 * \brief Creates a message box within three buttons.
 *
 * This function creates a message box hosted to the main window \a hwnd, 
 * displays a message and an application icon in the message box, and creates
 * three buttons in it. This function also receives \a printf-like arguments 
 * to format a string.
 *
 * This function can be used to prompt the user to choose one item among 
 * three choices.
 *
 * \param hwnd The hosting main window.
 * \param title The title of the message box.
 * \param button1 The title of the first button.
 * \param button2 The title of the second button.
 * \param button3 The title of the third button.
 * \param text The format string.
 *
 * \return 0, 1, or 2, indicates the message box was closed by 
 *         the first, the second, or the third button.
 *
 * \sa myWinChoice, printf(3)
 */
MGUTILS_EXPORT int myWinTernary (HWND hwnd, const char* title, 
                const char* button1, const char* button2, const char* button3,
                const char* text, ...);

/**
 * \fn void errorWindow (HWND hwnd, const char* str, const char* title)
 * \brief A MiniGUI edition of \a perror.
 *
 * This function creates a message box by using \a myMessageBox, and display the
 * current system error message. You can consider it as an alternative of 
 * \a perror.
 *
 * \param hwnd The hosting main window.
 * \param str The string will be appeared before the system error message.
 * \param title The title of the message box.
 *
 * \sa myMessageBox, perror(3)
 */
MGUTILS_EXPORT void errorWindow (HWND hwnd, const char* str, const char* title);

/**
 * \fn HWND createStatusWin (HWND hParentWnd, int width, int height,\
                const char* title, const char* text, ...)
 * \brief Creates a status main window.
 *
 * This function creates a status main window and returns the handle to it.
 * You can call \a destroyStatusWin to destroy it. This function also 
 * receives \a printf-like arguments to format a string.
 *
 * \param hParentWnd The hosting main window.
 * \param width The width of the status window. 
 * \param height The height of the status window. 
 * \param title The title of the status window.
 * \param text The format string.
 *
 * \return The handle to the status window on success, HWND_INVALID on error.
 *
 * \sa destroyStatusWin
 */
MGUTILS_EXPORT HWND createStatusWin (HWND hParentWnd, int width, int height, 
                const char* title, const char* text, ...);

/**
 * \fn void destroyStatusWin (HWND hwnd)
 * \brief Destroies a status window.
 *
 * This function destroies the specified status window \a hwnd, which 
 * is returned by \a createStatusWin.
 *
 * \param hwnd The handle to the status window.
 * 
 * \sa createStatusWin
 */
MGUTILS_EXPORT void destroyStatusWin (HWND hwnd);

/* back-compatibility definitions */
#define createToolTipWin CreateToolTipWin
#define resetToolTipWin ResetToolTipWin
#define destroyToolTipWin DestroyToolTipWin

#ifdef _MGCTRL_PROGRESSBAR

/**
 * \fn HWND createProgressWin (HWND hParentWnd, const char *title,\
                const char *label, int id, int range)
 * \brief Creates a main window within a progress bar.
 *
 * This function creates a main window within a progress bar and 
 * returns the handle. You can call \a destroyProgressWin to destroy it. 
 *
 * Note that you can use \a SendDlgItemMessage to send a message to the 
 * progress bar in the main window in order to update the progress bar.
 *
 * \param hParentWnd The hosting main window.
 * \param title The title of the progress window.
 * \param label The text in the label of the progress bar.
 * \param id The identifier of the progress bar.
 * \param range The maximal value of the progress bar (minimal value is 0).
 *
 * \return The handle to the progress window on success, HWND_INVALID on error.
 *
 * \sa destroyProgressWin
 */
MGUTILS_EXPORT HWND createProgressWin (HWND hParentWnd, const char* title, 
                const char* label, int id, int range);

/**
 * \fn void destroyProgressWin (HWND hwnd)
 * \brief Destroies progress window.
 *
 * This function destroies the specified progress window \a hwnd, which 
 * is returned by \a createProgressWin.
 *
 * \param hwnd The handle to the progress window.
 * 
 * \sa createProgressWin
 */
MGUTILS_EXPORT void destroyProgressWin (HWND hwnd);

#endif /* _MGCTRL_PROGRESSBAR */

/**
 * Button info structure used by \a myWinMenu and \a myWinEntries function.
 * \sa myWinMenu, myWinEntries
 */
typedef struct _myWinButton
{
    /** Text of the button. */
    char*   text;
    /** Identifier of the button. */
    int     id;
    /** Styles of the button. */
    DWORD   flags;
} myWINBUTTON;

/* This is an internal structure. */
typedef struct _myWinMenuItems
{
    /* The pointer to the array of the item strings. */
    char**      items;
    /* The identifier of the listbox display the menu items. */
    int         listboxid;
    /* The pointer to the array of the selection status of the items. */
    int*        selected;
    /* The minimal button identifier. */
    int         minbuttonid;
    /* The maximal button identifier. */
    int         maxbuttonid;
} myWINMENUITEMS;

/**
 * \fn int myWinMenu (HWND hParentWnd, const char* title,\
                const char *label, int width, int listboxheight,\
                char **items, int *listItem, myWINBUTTON* buttons)
 * \brief Creates a menu main window for the user to select an item.
 *
 * This function creates a menu main window including a few buttons, 
 * and a list box with checkable item.
 * 
 * When the user click one of the buttons, this function will return the 
 * identifier of the button which leads to close the menu window, and 
 * the selections of the items via \a listItem.
 *
 * \param hParentWnd The hosting main window.
 * \param title The title of the menu main window.
 * \param label The label of the list box.
 * \param width The width of the menu main window.
 * \param listboxheight The height of the list box.
 * \param items The pointer to the array of the item strings.
 * \param listItem The pointer to the array of the check status of the items, 
 *        initial and returned.
 * \param buttons The buttons will be created.
 *
 * \return Returns the identifier of the button leading to close 
 *         the menu window on success, else on errors.
 *
 * \sa myWINBUTTON
 */
MGUTILS_EXPORT int myWinMenu (HWND hParentWnd, const char* title, 
                const char* label, int width, int listboxheight, 
                char ** items, int * listItem, myWINBUTTON* buttons);

/**
 * Entry info structure used by \a myWinEntries function.
 * \sa myWinEntries
 */
typedef struct _myWinEntry
{
    /** The label of the entry. */
    char*   text;
    /** The pointer to the string of the entry. */
    char**  value;
    /** The maximal length of the entry in bytes. */
    int     maxlen;
    /** The styles of the entry. */
    DWORD   flags;
} myWINENTRY;

/* This is an internal structure. */
typedef struct _myWinEntryItems
{
    myWINENTRY* entries;
    int         entrycount;
    int         firstentryid;
    int         minbuttonid;
    int         maxbuttonid;
} myWINENTRYITEMS;

/**
 * \fn int myWinEntries (HWND hParentWnd, const char* title,\
                const char* label, int width, int editboxwidth,\
                BOOL fIME, myWINENTRY* items, myWINBUTTON* buttons)
 * \brief Creates a entry main window for the user to enter something.
 *
 * This function creates a entry main window including a few buttons
 * and a few entries.
 * 
 * When the user click one of the buttons, this function will return the 
 * identifier of the button which leads to close the menu window, and 
 * the entered strings.
 *
 * \param hParentWnd The hosting main window.
 * \param title The title of the menu main window.
 * \param label The label of the entries.
 * \param width The width of the menu main window.
 * \param editboxwidth The width of the edit boxes.
 * \param fIME Whether active the IME window (obsolete).
 * \param items The pointer to the array of the entries, initial and returned.
 * \param buttons The buttons will be created.
 *
 * \return Returns the identifier of the button leading to close the 
 *         menu window on success, else on errors.
 *
 * \sa myWINBUTTON, myWINENTRY
 */
MGUTILS_EXPORT int myWinEntries (HWND hParentWnd, const char* title, 
                const char* label, int width, int editboxwidth, 
                BOOL fIME, myWINENTRY* items, myWINBUTTON* buttons);

/**
 * \fn int myWinHelpMessage (HWND hwnd, int width, int height,\
                const char* help_title, const char* help_msg)
 * \brief Creates a help message window.
 *
 * This function creates a help message window including a scrollable help 
 * message and a spin box. When the user click the OK button, this function 
 * will return.
 *
 * \param hwnd The hosting main window.
 * \param width The width of the help message window.
 * \param height The height of the help message window.
 * \param help_title The title of the window.
 * \param help_msg The help message.
 *
 * \return 0 on success, -1 on error.
 */
MGUTILS_EXPORT int myWinHelpMessage (HWND hwnd, int width, int height,
                const char* help_title, const char* help_msg);

    /** @} end of mywins_fns */

#endif /* _MGUTILS_MYWINS */

#ifdef _MGUTILS_VCONGUI

    /**
     * \addtogroup vcongui_fns Interfaces of mGUtils library (libmgutils)
     *
     * mGUtils provides a virtual console in a main window of MiniGUI.
     *
     * @{
     */

#ifndef __cplusplus
#ifndef __ECOS__
typedef	enum {false, true} bool;
#endif
#endif

#define VCONGUI_VERSION "Version 0.8 (Feb. 2018)"

#define MIN_COLS    10
#define MAX_COLS    100
#define MIN_ROWS    10
#define MAX_ROWS    60

#define GetCharWidth GetSysCharWidth
#define GetCCharWidth GetSysCCharWidth
#define GetCharHeight GetSysCharHeight

/**
 * Information of child process created.
 * \sa VCOnMiniGUI
 */
typedef struct _CHILDINFO
{
    /** Whether display start up messages. */
    bool startupMessage;
    /** Customized string will be displayed as a startup message. */
    const char* startupStr;
    /**
     * The program should be executed when startup
     * (If it is NULL, MiniGUI will try to execute the default shell).
     */
    const char* execProg;
    /** The argument of the startup program if \a execProg is NULL. */
    const char* execArgs;
    
    /**
     * The customized default window procedure of vcongui main window
     * (If it is NULL, MiniGUI will call the default main window procedure).
     */
    WNDPROC     DefWinProc;
    /** Whether display a menu */
    bool        fMenu;
    /** The initial position of the vcongui main window. */
    int         left, top;
    /** The number of terminal rows and columns. */
    int         rows, cols;
}CHILDINFO;
/** Data type of pointer to a CHILDINFO */
typedef CHILDINFO* PCHILDINFO;

/**
 * \fn void* VCOnMiniGUI (void* data)
 * \brief Creates a vcongui main window.
 *
 * This function creates a vcongui main window, and enter a message loop.
 * 
 * \param data The pointer to a CHILDINFO structure.
 */
void* VCOnMiniGUI (void* data);

#ifdef _MGRM_THREADS
void* NewVirtualConsole (PCHILDINFO pChildInfo);
#endif

    /** @} end of vcongui_fns */

#endif /* _MGUTILS_VCONGUI */

    /**
     * \addtogroup templates_fns Interfaces of mGUtils library (libmgutils)
     *
     * mGUtils provides a dialog in a main window of MiniGUI.
     *
     * @{
     */

/**
* \fn int ShowCommonDialog (PDLGTEMPLATE dlg_template, HWND hwnd, \
        WNDPROC proc, void* private_data)
* \ brief Creates a modal common dialog box from a dialog box
*          template in memory and other information.
*
* This function can be used for file choosing dialog, color selecting
* dialog, font selecting dialog, and information dialog.
*
* \param dlg_template The pointer to a DLGTEMPLATE structure.
* \param hwnd The handle to the hosting main window.
* \param proc The window procedure of the common dialog box.
* \param private_data The parameter will be passed to the window procedure.
*
* \return If the user clicks OK or CLOSE button of the dialog box, the
*         return value is TRUE, otherwise return FALSE.
*/
MGUTILS_EXPORT BOOL ShowCommonDialog (PDLGTEMPLATE dlg_template, HWND hwnd, 
        WNDPROC proc, void* private_data);

#ifdef _MGUTILS_DLGOPENFILE

#ifndef WIN32
#include <sys/types.h>
#include <dirent.h>
#endif

/**
 * \def FILE_ERROR_OK
 * \brief Open file success.
 */
#define FILE_ERROR_OK           0

/**
 * \def FILE_ERROR_PARAMERR
 * \brief Wrong parameters.
 */
#define FILE_ERROR_PARAMERR     -1

/**
 * \def FILE_ERROR_PARAMERR
 * \brief Path doesn't exist.
 */
#define FILE_ERROR_PATHNOTEXIST -2

/**
 * \def MAX_FILTER_LEN
 * \brief The maximum length of filter string.
 */
#define MAX_FILTER_LEN          255

/**
 * \def MY_NAMEMAX
 * \brief The maximum length of name.
 */
#define MY_NAMEMAX      127

/**
 * \def MY_PATHMAX
 * \brief The maximum length of path.
 */
#define MY_PATHMAX      255
typedef struct _FILEDLGDATA
{
        /** Indicates to create a Save File or an Open File dialog box. */
        BOOL        is_save;
        
        /** Indicates the controls is transparent or no? */
        BOOL        is_trans;
        
        /** The full path name of the file returned. */
        char        filefullname[MY_NAMEMAX + MY_PATHMAX + 1];
        
        /** The name of the file to be opened. */
        char        filename[MY_NAMEMAX + 1];
        
        /** The initial path of the dialog box. */
        char        filepath[MY_PATHMAX + 1];
        
        /**
        * The filter string, for example: 
        * All file (*.*)|Text file (*.txt;*.TXT)
        */     
        char        filter[MAX_FILTER_LEN + 1];
        
        /** The initial index of the filter*/
        int         filterindex;
        
        WNDPROC     hook;
} FILEDLGDATA, *PFILEDLGDATA;

#define IDC_FOSD_BASE                   520
#define IDC_FOSD_PATH_NOTE              (IDC_FOSD_BASE + 1) 
#define IDC_FOSD_PATH                   (IDC_FOSD_BASE + 2) 
#define IDC_FOSD_UPPER                  (IDC_FOSD_BASE + 3) 
#define IDC_FOSD_FILELIST               (IDC_FOSD_BASE + 4) 
#define IDC_FOSD_FILENAME_NOTE          (IDC_FOSD_BASE + 5) 
#define IDC_FOSD_FILENAME               (IDC_FOSD_BASE + 6) 
#define IDC_FOSD_FILETYPE_NOTE          (IDC_FOSD_BASE + 7) 
#define IDC_FOSD_FILETYPE               (IDC_FOSD_BASE + 8) 
#define IDC_FOSD_ISHIDE                 (IDC_FOSD_BASE + 9) 
#define IDC_FOSD_OK                     (IDC_FOSD_BASE + 10)
#define IDC_FOSD_CANCEL                 (IDC_FOSD_BASE + 11)

/**
* \fn int FileOpenSaveDialog (PDLGTEMPLATE dlg_template, HWND hwnd, \
WNDPROC proc, PFILEDLGDATA pfdd)
* \ brief Creates a modal Open/Save File Dialog Box.
*
* This function creates an Open/Save File Dialog Box, and returns
* the full path name of the file selected by user.
*
* \param dlg_template The pointer to a DLGTEMPLATE structure.
* \param hwnd The handle to the hosting main window.
* \param proc The window procedure of the common dialog box.
* \param pfdd The pointer to the FILEDLGDATA structure.
*
* \return If the user clicks OK or CLOSE button of the dialog box, the
*         return value is TRUE, otherwise return FALSE.
*/
MGUTILS_EXPORT BOOL FileOpenSaveDialog  (PDLGTEMPLATE dlg_template, HWND hwnd, 
        WNDPROC proc, PFILEDLGDATA pfdd);

MGUTILS_EXPORT extern DLGTEMPLATE DefFileDlg;
MGUTILS_EXPORT extern DLGTEMPLATE DefSimpleFileDlg;

/** The default File Open/Save Dialog callback procedure. */
MGUTILS_EXPORT LRESULT DefFileDialogProc (HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

/**
 * The file dialog box structure used by \a ShowOpenDialog.
 * \sa ShowOpenDialog.
 */
typedef struct _NEWFILEDLGDATA
{
    /** Indicates to create a Save File or an Open File dialog box. */
    BOOL    IsSave;
    /** Indicates the controls is transparent or no?*/
    BOOL    IsTrans;
    /** The full path name of the file returned. */
    char    filefullname[MY_NAMEMAX + MY_PATHMAX + 1];
    /** The name of the file to be opened. */
    char    filename[MY_NAMEMAX + 1];
    /** The initial path of the dialog box. */
    char    filepath[MY_PATHMAX + 1];
    /**
     * The filter string, for example: 
     * All file (*.*)|Text file (*.txt;*.TXT)
     */
    char    filter[MAX_FILTER_LEN + 1];
    /** The initial index of the filter*/
    int     filterindex;
} NEWFILEDLGDATA;
/** Data type of pointer to a NEWFILEDLGDATA */
typedef NEWFILEDLGDATA* PNEWFILEDLGDATA;

/**
 * \fn int ShowOpenDialog (HWND hWnd, int lx, int ty,\
                int w, int h, PNEWFILEDLGDATA pnfdd)
 * \brief Creates an Open File Dialog Box.
 *
 * This function creates an Open File Dialog Box, and returns 
 * the full path name of the file selected by user.
 *
 * \param hWnd The hosting main window.
 * \param lx lx,ty,w,h: The default position and size of the dialog box.
 * \param ty lx,ty,w,h: The default position and size of the dialog box.
 * \param w  lx,ty,w,h: The default position and size of the dialog box.
 * \param h  lx,ty,w,h: The default position and size of the dialog box.
 * \param pnfdd The pointer to the NEWFILEDLGDATA structure.
 *
 * \retval IDOK The user choosed a file and OK button clicked.
 * \retval IDCANCLE CANCEL button clicked.
 *
 * \attention pnfdd->filename is an input & output parameter, you can set
 * your default filename by it. So before use this function, you must init 
 * pnfdd->filename.
 * 
 * \sa NEWFILEDLGDATA, ShowOpenDialog
 */
MGUTILS_EXPORT int ShowOpenDialog (HWND hWnd, int lx, int ty, 
                int w, int h, PNEWFILEDLGDATA pnfdd);

#endif /* _MGUTILS_DLGOPENFILE */

#ifdef _MGUTILS_DLGCOLORSEL

/**
 * \fn void RGB2HSV (Uint8 r, Uint8 g, Uint8 b, \
 *               Uint16 *hout, Uint8 *sout, Uint8 *vout)
 * \brief Converts r/g/b values of a color in RGB color space to h/s/v of 
 *        the color in HSV color space.
 *
 * This function converts r/g/b values of a color in RGB color space to 
 * h/s/v values of the color in HSV color space. 
 *
 * \param r     The red value of the color in RGB space to be converted.
 * \param g     The green value of the color in RGB space to be converted.
 * \param b     The blue value of the color in RGB space to be converted.
 * \param hout  The pointer to Uint16, returns the h value of the color
 *              in HSV color space.
 * \param sout  The pointer to Uint8, returns the s value of the color 
 *              in HSV color space returned.
 * \param vout  The pointer to Uint8, returns the v value of the color
 *              in HSV color space returned.
 *
 * \sa HSV2RGB, YUV2RGB, RGB2YUV 
 */

void RGB2HSV (Uint8 r, Uint8 g, Uint8 b, 
                Uint16 *hout, Uint8 *sout, Uint8 *vout);

/**
 * \fn void HSV2RGB (Uint16 hin, Uint8 sin, Uint8 vin, \
 *               Uint8 *rout, Uint8 *gout, Uint8 *bout)
 * \brief Converts h/s/v values of a color to r/g/b values of the color.
 *
 * This function converts h/s/v values of a color in HSV color space 
 * to r/g/b values of the color in RGB space. 
 *
 * \param hin   The h value of the HSV color space to be converted, 
 *              the range is between 0 to 359.
 * \param sin   The s value of the HSV color space to be converted, 
 *              the range is between 0 to 255.
 * \param vin   The v value of the HSV color space to be converted, 
 *              the range is between 0 to 255.
 * \param rout  The pointer to Uint8, returns the red value of the RGB 
 *              color space.
 * \param gout  The pointer to Uint8, returns the green value of the RGB 
 *              color space.
 * \param bout  The pointer to Uint8, returns the bout value of the RGB 
 *              color space.
 * 
 * \sa RGB2HSV, YUV2RGB, RGB2YUV 
 */

void HSV2RGB (Uint16 hin, Uint8 sin, Uint8 vin, 
                Uint8 *rout, Uint8 *gout, Uint8 *bout);

/**
 * \fn void YUV2RGB (int y, int u, int v, Uint8 *r, Uint8 *g, Uint8 *b)
 * \brief Converts y/u/v values of a color in YUV color space to 
 *        to r/g/b values of the color in RGB color space.
 *
 * This function converts y/u/v values of YUV color space to r/g/b values 
 * of RGB color space. 
 *
 * \param y     The y value of the YUV color space to be converted.
 * \param u     The u value of the YUV color space to be converted.
 * \param v     The v value of the YUV color space to be converted.
 * \param r     The pointer to Uint8, returns the red value of the 
 *              RGB color space.
 * \param g     The pointer to Uint8, returns the green value of the 
 *              RGB color space.
 * \param b     The pointer to Uint8, returns the bout value of the 
 *              RGB color space.
 * 
 * \sa RGB2HSV, HSV2RGB, RGB2YUV 
 */

void YUV2RGB (int y, int u, int v, Uint8 *r, Uint8 *g, Uint8 *b);

/**
 * \fn void RGB2YUV (Uint8 r, Uint8 g, Uint8 b, int *y, int *u, int *v)
 * \brief Converts r/g/b values of a color in RGB color space 
 *        to y/u/v values of the color in YUV color space.
 *
 * The function converts r/g/b values of a color in RGB color space 
 *        to y/u/v values of the color in YUV color space.
 *
 * \param r  The red value of the RGB color space to be converted.
 * \param g  The green value of the RGB color space to be converted.
 * \param b  The blue value of the RGB color space to be converted.
 * \param y  The pointer to int, returns the y value of the YUV color space.
 * \param u  The pointer to int, returns the u value of the YUV color space.
 * \param v  The pointer to int, returns the v value of the YUV color space.
 * 
 * \sa RGB2HSV, HSV2RGB, YUV2RGB 
 */
MG_EXPORT void RGB2YUV (Uint8 r, Uint8 g, Uint8 b, int *y, int *u, int *v);

typedef struct _COLORDLGDATA {
        /** The value of the color  returned. */
        gal_pixel       pixel;
        
        /** The R, G, B value of the color returned. */
        Uint8           r, g, b;
        
        /** The H value of the color returned. */
        Uint16          h;
        
        /** The S, V value of the color returned. */
        Uint8           s, v;

        /** Indicates the controls is transparent or no?*/
        BOOL            is_trans;
        
        WNDPROC         hook;
} COLORDLGDATA, *PCOLORDLGDATA;

#define IDC_CSD_BASE                600 
#define IDC_CSD_BASIC_COLOR_NOTE    (IDC_CSD_BASE + 1) 
#define IDC_CSD_BASIC_COLOR          (IDC_CSD_BASE + 2)
#define IDC_CSD_BASIC_COLOR_0       (IDC_CSD_BASE + 2)
#define IDC_CSD_BASIC_COLOR_1       (IDC_CSD_BASE + 3)
#define IDC_CSD_BASIC_COLOR_2       (IDC_CSD_BASE + 4)
#define IDC_CSD_BASIC_COLOR_3       (IDC_CSD_BASE + 5)
#define IDC_CSD_BASIC_COLOR_4       (IDC_CSD_BASE + 6)
#define IDC_CSD_BASIC_COLOR_5       (IDC_CSD_BASE + 7)
#define IDC_CSD_BASIC_COLOR_6       (IDC_CSD_BASE + 8)
#define IDC_CSD_BASIC_COLOR_7       (IDC_CSD_BASE + 9)
#define IDC_CSD_BASIC_COLOR_8       (IDC_CSD_BASE + 10)  
#define IDC_CSD_BASIC_COLOR_9       (IDC_CSD_BASE + 11)
#define IDC_CSD_BASIC_COLOR_10      (IDC_CSD_BASE + 12)
#define IDC_CSD_BASIC_COLOR_11      (IDC_CSD_BASE + 13)
#define IDC_CSD_BASIC_COLOR_12      (IDC_CSD_BASE + 14)
#define IDC_CSD_BASIC_COLOR_13      (IDC_CSD_BASE + 15)
#define IDC_CSD_BASIC_COLOR_14      (IDC_CSD_BASE + 16)
#define IDC_CSD_BASIC_COLOR_15      (IDC_CSD_BASE + 17)
#define IDC_CSD_BASIC_COLOR_16      (IDC_CSD_BASE + 18)
#define IDC_CSD_BASIC_COLOR_17      (IDC_CSD_BASE + 19)
#define IDC_CSD_BASIC_COLOR_18      (IDC_CSD_BASE + 20)
#define IDC_CSD_BASIC_COLOR_19      (IDC_CSD_BASE + 21)
#define IDC_CSD_BASIC_COLOR_20      (IDC_CSD_BASE + 22)
#define IDC_CSD_BASIC_COLOR_21      (IDC_CSD_BASE + 23)
#define IDC_CSD_BASIC_COLOR_22      (IDC_CSD_BASE + 24)
#define IDC_CSD_BASIC_COLOR_23      (IDC_CSD_BASE + 25)
#define IDC_CSD_BASIC_COLOR_24      (IDC_CSD_BASE + 26)
#define IDC_CSD_BASIC_COLOR_25      (IDC_CSD_BASE + 27)
#define IDC_CSD_BASIC_COLOR_26      (IDC_CSD_BASE + 28)
#define IDC_CSD_BASIC_COLOR_27      (IDC_CSD_BASE + 29)
#define IDC_CSD_BASIC_COLOR_28      (IDC_CSD_BASE + 30)
#define IDC_CSD_BASIC_COLOR_29      (IDC_CSD_BASE + 31)
#define IDC_CSD_CUSTOM_COLOR_NOTE   (IDC_CSD_BASE + 32)
#define IDC_CSD_CUSTOM_COLOR_0      (IDC_CSD_BASE + 33)
#define IDC_CSD_CUSTOM              (IDC_CSD_BASE + 33)
#define IDC_CSD_CUSTOM_COLOR_1      (IDC_CSD_BASE + 34)
#define IDC_CSD_CUSTOM_COLOR_2      (IDC_CSD_BASE + 35)
#define IDC_CSD_CUSTOM_COLOR_3      (IDC_CSD_BASE + 36)
#define IDC_CSD_CUSTOM_COLOR_4      (IDC_CSD_BASE + 37)
#define IDC_CSD_CUSTOM_COLOR_5      (IDC_CSD_BASE + 38)
#define IDC_CSD_SPACE               (IDC_CSD_BASE + 39)
#define IDC_CSD_YSPACE              (IDC_CSD_BASE + 40)
#define IDC_CSD_COLOR               (IDC_CSD_BASE + 41)
#define IDC_CSD_NOTE_H              (IDC_CSD_BASE + 42)
#define IDC_CSD_NOTE_S              (IDC_CSD_BASE + 43)
#define IDC_CSD_NOTE_V              (IDC_CSD_BASE + 44)
#define IDC_CSD_NOTE_R              (IDC_CSD_BASE + 45)
#define IDC_CSD_NOTE_G              (IDC_CSD_BASE + 46)
#define IDC_CSD_NOTE_B              (IDC_CSD_BASE + 47)
#define IDC_CSD_VALUE_H             (IDC_CSD_BASE + 48)
#define IDC_CSD_VALUE_S             (IDC_CSD_BASE + 49)
#define IDC_CSD_VALUE_V             (IDC_CSD_BASE + 50)
#define IDC_CSD_VALUE_R             (IDC_CSD_BASE + 51)
#define IDC_CSD_VALUE_G             (IDC_CSD_BASE + 52)
#define IDC_CSD_VALUE_B             (IDC_CSD_BASE + 53)
#define IDC_CSD_ADD                 (IDC_CSD_BASE + 54)
#define IDC_CSD_OK                  (IDC_CSD_BASE + 55)
#define IDC_CSD_CANCEL              (IDC_CSD_BASE + 56)

/**
* \fn int ColorSelectDialog (PDLGTEMPLATE dlg_template, HWND hwnd, \
WNDPROC proc, PCOLORDLGDATA pcdd)
* \ brief Creates a modal Color Selection Dialog Box.
*
* This function creates an Color Selection Dialog Box, and returns
* the values of the color selected by the user.
*
* \param dlg_template The pointer to a DLGTEMPLATE structure.
* \param hwnd The handle to the hosting main window.
* \param proc The window procedure of the color selection dialog box.
* \param pcdd The pointer to the COLORDLGDATA structure.
*
* \return If the user clicks OK or CLOSE button of the dialog box, the
*         return value is TRUE, otherwise return FALSE.
*/
MGUTILS_EXPORT BOOL ColorSelectDialog  (PDLGTEMPLATE dlg_template, HWND hwnd, 
        WNDPROC proc, PCOLORDLGDATA pcdd);

MGUTILS_EXPORT extern DLGTEMPLATE DefColorDlg;
MGUTILS_EXPORT extern DLGTEMPLATE DefSimpleColorDlg;

/** The default Color Selection Dialog callback procedure. */
MGUTILS_EXPORT LRESULT DefColorDialogProc (HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

/**
 * The color data structure used by \a ColorSelDialog.
 * \sa ColorSelDialog.
 */
typedef struct _COLORDATA {
    /** Reserves, not used now. */
    DWORD           style;
    /** The value of the color  returned. */
    gal_pixel       pixel;
    /** The R, G, B value of the color returned. */
    Uint8           r, g, b;
    /** The H value of the color returned. */
    Uint16          h;
    /** The S, V value of the color returned. */
    Uint8           s, v;
    /** Indicates the controls is transparent or no?*/
    BOOL            is_trans;
} COLORDATA, *PCOLORDATA;

/* Internal use */
#define SELCOLOR_OK     IDOK
/* Internal use */
#define SELCOLOR_CANCEL IDCANCEL

/**
 * \fn int ColorSelDialog (HWND hWnd, int x, int y, \
 *               int w, int h, PCOLORDATA pClrData)
 * \brief Creates a Color Selection Dialog Box.
 *
 * This function creates a Color Selection Dialog Box, and returns 
 * the values of the color selected by the user.
 *
 * \param hWnd The hosting main window.
 * \param x  x,y,w,h: The default position and size of the dialog box.
 * \param y  x,y,w,h: The default position and size of the dialog box.
 * \param w  x,y,w,h: The default position and size of the dialog box.
 * \param h  x,y,w,h: The default position and size of the dialog box.
 * \param pClrData The pointer to the COLORDATA structure.
 *
 * \retval IDOK     The user choosed a color and OK button clicked.
 * \retval IDCANCEL The user clicked the Cancel button.
 * 
 * \sa COLORDATA, ColorSelDialog
 */
MGUTILS_EXPORT int ColorSelDialog (HWND hWnd, int x, int y, 
                int w, int h, PCOLORDATA pClrData);

#endif /* _MGUTILS_DLGCOLORSEL */

#ifdef _MGUTILS_DLGFONTSEL

typedef struct _FONTDLGDATA {
        /* The font minimize size. */
        int             min_size;
        /* The font maximize size. */
        int             max_size;
        /* Indicates the controls is transparent or no?*/
        BOOL            is_trans;       
        
        /* The font color. */
        RGB             color;
        PLOGFONT        logfont;
        
        WNDPROC         hook;
} FONTDLGDATA, *PFONTDLGDATA;

#define IDC_FSD_BASE            540
#define IDC_FSD_FONT_NOTE       (IDC_FSD_BASE + 1) 
#define IDC_FSD_FONT            (IDC_FSD_BASE + 2)
#define IDC_FSD_STYLE_NOTE      (IDC_FSD_BASE + 3)
#define IDC_FSD_STYLE           (IDC_FSD_BASE + 4)
#define IDC_FSD_SIZE_NOTE       (IDC_FSD_BASE + 5)
#define IDC_FSD_SIZE            (IDC_FSD_BASE + 6)
#define IDC_FSD_EFFECTS_NOTE    (IDC_FSD_BASE + 7)
#define IDC_FSD_FLIP_NOTE       (IDC_FSD_BASE + 8)
#define IDC_FSD_FLIP            (IDC_FSD_BASE + 9)
#define IDC_FSD_COLOR_NOTE      (IDC_FSD_BASE + 10)  
#define IDC_FSD_COLOR           (IDC_FSD_BASE + 11)
#define IDC_FSD_ITALIC          (IDC_FSD_BASE + 12)
#define IDC_FSD_STRIKEOUT       (IDC_FSD_BASE + 13)
#define IDC_FSD_UNDERLINE       (IDC_FSD_BASE + 14)
#define IDC_FSD_SAMPLE_NOTE     (IDC_FSD_BASE + 15)
#define IDC_FSD_SAMPLE          (IDC_FSD_BASE + 16)
#define IDC_FSD_CHARSET_NOTE    (IDC_FSD_BASE + 17)
#define IDC_FSD_CHARSET         (IDC_FSD_BASE + 18)
#define IDC_FSD_OK              (IDC_FSD_BASE + 19)
#define IDC_FSD_CANCEL          (IDC_FSD_BASE + 20)

/**
* \fn int FontSelectDialog (PDLGTEMPLATE dlg_template, HWND hwnd, \
WNDPROC proc, PFONTDLGDATA pfsd)
* \ brief Creates a modal Font Selection Dialog Box.
*
* This function creates an Font Selection Dialog Box, and returns
* the pointer to the logical font selected by user.
*
* \param dlg_template The pointer to a DLGTEMPLATE structure.
* \param hwnd The handle to the hosting main window.
* \param proc The window procedure of the font selection dialog box.
* \param pfsd The pointer to the FONTDLGDATA structure.
*
* \return If the user clicks OK or CLOSE button of the dialog box, the
*         return value is TRUE, otherwise return FALSE.
*/
MGUTILS_EXPORT BOOL FontSelectDialog  (PDLGTEMPLATE dlg_template, HWND hwnd, 
        WNDPROC proc, PFONTDLGDATA pfsd);

MGUTILS_EXPORT extern DLGTEMPLATE DefSimpleFontDlg;
MGUTILS_EXPORT extern DLGTEMPLATE DefFontDlg;

/** The default Font Selection Dialog callback procedure. */
MGUTILS_EXPORT LRESULT DefFontDialogProc (HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

#endif /* _MGUTILS_DLGFONTSEL */

#ifdef _MGUTILS_DLGSHOWINFO

typedef struct _INFODLGDATA {
        const char*     msg;
        WNDPROC         hook;
        
        /* for internal usage.*/
        int             nr_lines;
        int             vis_lines;
        int             start_line;
        RECT            rc;
} INFODLGDATA, *PINFODLGDATA;

#define IDC_IFD_SPIN      580

/**
* \fn int InfoShowDialog (PDLGTEMPLATE dlg_template, HWND hwnd, \
        WNDPROC proc, PINFODLGDATA pidd)
* \ brief Creates a modal Information Dialog Box.
*
* This function creates a Information Dialog Box.
*
* \param dlg_template The pointer to a DLGTEMPLATE structure.
* \param hwnd The handle to the hosting main window.
* \param proc The window procedure of the information dialog box.
* \param pidd The pointer to the INFODLGDATA structure.
*
* \return If the user clicks OK or CLOSE button of the dialog box, the
*         return value is TRUE, otherwise return FALSE.
*/
MGUTILS_EXPORT BOOL InfoShowDialog  (PDLGTEMPLATE dlg_template, HWND hwnd, 
        WNDPROC proc, PINFODLGDATA pidd);

MGUTILS_EXPORT extern DLGTEMPLATE DefInfoDlg;
MGUTILS_EXPORT extern DLGTEMPLATE DefSimpleInfoDlg;

/** The default Information Dialog callback procedure. */
MGUTILS_EXPORT LRESULT DefInfoDialogProc (HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

#endif /* _MGUTILS_DLGSHOWINFO */

    /** @} end of templates_fns */

#ifdef  _MGUTILS_SKIN

    /**
     * \defgroup skin_fns Skin related routines
     *
     * @{
     */

/* skin messages (send to items) */
#define SKIN_MSG_LBUTTONDOWN    1
#define SKIN_MSG_LBUTTONUP      2
#define SKIN_MSG_CLICK          3
/*#define SKIN_MSG_DBCLICK      4 */
#define SKIN_MSG_MOUSEMOVE      5
#define SKIN_MSG_MOUSEDRAG      6
#define SKIN_MSG_SETFOCUS       7
#define SKIN_MSG_KILLFOCUS      8

/* type of item */

/**
 * \def SI_TYPE_NRMLABEL
 * \sa skin_item_s
*/
#define SI_TYPE_NRMLABEL        0x10000000

/**
 * \def SI_TYPE_BMPLABEL
 * \sa skin_item_s
*/
#define SI_TYPE_BMPLABEL        0x20000000

/**
 * \def SI_TYPE_CMDBUTTON
 * \sa skin_item_s
*/
#define SI_TYPE_CMDBUTTON       0x30000000

/**
 * \def SI_TYPE_CHKBUTTON
 * \sa skin_item_s
*/
#define SI_TYPE_CHKBUTTON       0x40000000

/**
 * \def SI_TYPE_NRMSLIDER
 * \sa skin_item_s
*/
#define SI_TYPE_NRMSLIDER       0x50000000

#ifdef _MGHAVE_FIXED_MATH
/**
 * \def SI_TYPE_ROTSLIDER
 * \sa skin_item_s
*/
#define SI_TYPE_ROTSLIDER       0x60000000
#endif

/**
 * \def SI_TYPE_CONTROL
 * \brief The item is a control.
*/
#define SI_TYPE_CONTROL         0x70000000

/**
 * \def SI_TYPE_MASK
 * \brief The item type mask.
*/
#define SI_TYPE_MASK            0xF0000000

/* type of hittest shape */

/**
 * \def SI_TEST_SHAPE_RECT
 * \sa skin_item_s
*/
#define SI_TEST_SHAPE_RECT      0x01000000

/**
 * \def SI_TEST_SHAPE_ELLIPSE
 * \sa skin_item_s
*/
#define SI_TEST_SHAPE_ELLIPSE   0x02000000

/**
 * \def SI_TEST_SHAPE_LOZENGE
 * \sa skin_item_s
*/
#define SI_TEST_SHAPE_LOZENGE   0x03000000

/**
 * \def SI_TEST_SHAPE_LTRIANGLE
 * \sa skin_item_s
*/
#define SI_TEST_SHAPE_LTRIANGLE 0x04000000

/**
 * \def SI_TEST_SHAPE_RTRIANGLE
 * \sa skin_item_s
*/
#define SI_TEST_SHAPE_RTRIANGLE 0x05000000

/**
 * \def SI_TEST_SHAPE_UTRIANGLE
 * \sa skin_item_s
*/
#define SI_TEST_SHAPE_UTRIANGLE 0x06000000

/**
 * \def SI_TEST_SHAPE_DTRIANGLE
 * \sa skin_item_s
*/
#define SI_TEST_SHAPE_DTRIANGLE 0x07000000

#define SI_TEST_SHAPE_MASK      0x0F000000

/* common status of item */

/**
 * \def SI_STATUS_OK
 * \sa skin_item_s
*/
#define SI_STATUS_OK            0x00000001

/**
 * \def SI_STATUS_VISIBLE
 * \sa skin_item_s
*/
#define SI_STATUS_VISIBLE       0x00000002

/**
 * \def SI_STATUS_DISABLED
 * \sa skin_item_s
*/
#define SI_STATUS_DISABLED      0x00000004

/**
 * \def SI_STATUS_HILIGHTED
 * \sa skin_item_s
*/
#define SI_STATUS_HILIGHTED     0x00000008

#define SI_STATUS_MASK          0x0000000F

/* status of button item */

/**
 * \def SI_BTNSTATUS_CLICKED
 * \sa skin_item_s
*/
#define SI_BTNSTATUS_CLICKED    0x00000010

/**
 * \def SI_BTNSTATUS_CHECKED
 * \sa skin_item_s
*/
#define SI_BTNSTATUS_CHECKED    SI_BTNSTATUS_CLICKED

#define SI_STATUS_CLICKED       SI_BTNSTATUS_CLICKED

#define SI_BTNSTATUS_MASK       0x000000F0

/** Style of command button item */
#define SI_CMDBUTTON_2STATE     0x00000100

/* style of normal slider item */

/**
 * \def SI_NRMSLIDER_HORZ
 * \sa skin_item_s
*/
#define SI_NRMSLIDER_HORZ       0x00000100

/**
 * \def SI_NRMSLIDER_VERT
 * \sa skin_item_s
*/
#define SI_NRMSLIDER_VERT       0x00000200

/**
 * \def SI_NRMSLIDER_STATIC
 * \sa skin_item_s
*/
#define SI_NRMSLIDER_STATIC     0x00000400

#ifdef _MGHAVE_FIXED_MATH

/* style of rotation slider item */

/**
 * \def SI_ROTSLIDER_CW
 * \sa skin_item_s
*/
#define SI_ROTSLIDER_CW         0x00000100

/**
 * \def SI_ROTSLIDER_ANTICW
 * \sa skin_item_s
*/
#define SI_ROTSLIDER_ANTICW     0x00000200

/**
 * \def SI_ROTSLIDER_STATIC
 * \sa skin_item_s
*/
#define SI_ROTSLIDER_STATIC     0x00000400

#endif /* _MGHAVE_FIXED_MATH */

#define SI_STYLE_MASK           0x0000FF00

/**
 * \def CTRL_SKIN
 * \brief The class name of skin control. 
 */
#define CTRL_SKIN               "skin"

/**
 * \var typedef struct skin_item_ops_s skin_item_ops_t
 * \sa skin_item_ops_s
 */
typedef struct skin_item_ops_s skin_item_ops_t;

/**
 * \var typedef struct skin_head_s skin_head_t
 * \sa skin_head_s
 */
typedef struct skin_head_s skin_head_t;

/** Skin item information structure */
typedef struct skin_item_s
{
    /** The identifier of the item. */
    int id;

    /**
     * The style of the item.
     *
     * The style of an item is composed of five classes of information:
     *
     *  - The type of an item.
     *  - The hit-test shape type.
     *  - The common status.
     *  - The item specific status.
     *  - The item specific style.
     *
     * The type of an item can be one of the following values:
     *
     *  - SI_TYPE_NRMLABEL\n
     *    The item is a normal label.
     *  - SI_TYPE_BMPLABEL\n
     *    The item is a bitmap label.
     *  - SI_TYPE_CMDBUTTON\n
     *    The item is a command button.
     *  - SI_TYPE_CHKBUTTON\n
     *    The item is a check button.
     *  - SI_TYPE_NRMSLIDER\n
     *    The item is a normal slider.
     *  - SI_TYPE_ROTSLIDER\n
     *    The item is a rotation slider.
     *
     * The hit-test shape type can be one of the following values:
     *
     *  - SI_TEST_SHAPE_RECT\n
     *    The hit-test shape is a rect.
     *  - SI_TEST_SHAPE_ELLIPSE\n
     *    The hit-test shape is an ellipse.
     *  - SI_TEST_SHAPE_LOZENGE\n
     *    The hit-test shape is a lozenge.
     *  - SI_TEST_SHAPE_LTRIANGLE\n
     *    The hit-test shape is a left triangle.
     *  - SI_TEST_SHAPE_RTRIANGLE\n
     *    The hit-test shape is a right triangle.
     *  - SI_TEST_SHAPE_UTRIANGLE\n
     *    The hit-test shape is a up triangle.
     *  - SI_TEST_SHAPE_DTRIANGLE\n
     *    The hit-test shape is a down triangle.
     *
     * The common status of an item can be OR'ed with the following values:
     *
     *  - SI_STATUS_OK\n
     *    The basic status of an item, always be there.
     *  - SI_STATUS_VISIBLE\n
     *    The item is visble.
     *  - SI_STATUS_DISABLED\n
     *    The item is disabled.
     *  - SI_STATUS_HILIGHTED\n
     *    The item is hilighted.
     *
     *  The item specific status can be one of the following values:
     *
     *  - SI_BTNSTATUS_CLICKED\n
     *    The three-state button is clicked.
     *  - SI_BTNSTATUS_CHECKED\n
     *    The check button is ckecked.
     *
     *  The item specific style can be a valued OR'ed with the following values:
     *
     *  - SI_NRMSLIDER_HORZ\n
     *    The normal slider is horizontal.
     *  - SI_NRMSLIDER_VERT\n
     *    The normal slider is vertical.
     *  - SI_NRMSLIDER_STATIC\n
     *    The normal slider is static, i.e., the slider acts like a 
     *    progress bar.
     *  - SI_ROTSLIDER_CW\n
     *    The rotation slider is clockwise.
     *  - SI_ROTSLIDER_ANTICW\n
     *    The rotation slider is anti-clockwise.
     *  - SI_ROTSLIDER_STATIC\n
     *    The rotation slider is static, i.e., the slider acts like a 
     *    progress bar.
     */
    DWORD style;

    /** X-coordinate of the item in the skin. */
    int x;
    /** Y-coordinate of the item in the skin. */
    int y;

    /** The hit-test rectangle of the item. */
    RECT item_rc;

    union {
        /** The index of the item bitmap in the skin bitmap array. */
        int bmp_index;

        /** The handle to the control if the type is SI_TYPE_CONTROL. */
        HWND hwnd;
    };

    /** The tip text. */
    char* tip;

    /** The attached private data with the item by application. */
    DWORD attached;

    /** 
     * The type-specific data of the item, 
     * used to define extra information of the item.
     */
    void* type_data;

    /******** start of internal fields ********/
    /* The bounding rectangle of the item. */
    RECT shape;
    /* The hit-test rectangle of the item. */
    RECT rc_hittest;
    /* The region for mouse hittest. */
    CLIPRGN region;
    /* The operation routines of the item. */
    skin_item_ops_t *ops;
    /* The parent skin. */
    skin_head_t *hostskin;
} skin_item_t;

/** Skin item operations */
struct skin_item_ops_s
{
    /** Operations for initialization. */
    int (*init) (skin_head_t*, skin_item_t*);
    /** Operations for deinitialization. */
    int (*deinit) (skin_item_t*);

    /** Operations when creating the skin window. */
    int (*on_create) (skin_item_t*);
    /** Operations when destroying the skin window. */
    int (*on_destroy) (skin_item_t*);

    /** Operations for item drawing. */
    void (*draw_bg) (HDC, skin_item_t*);
    /** Operations for item drawing. */
    void (*draw_attached) (HDC, skin_item_t*);

    /** 
     * Operations for getting item value.
     * For button item, value is the click status, non-zero for clicked 
     * or checked. For slider, value is the position.
     * For label item, value is the label text.
     * For control item, value is the handle to control.
     */
    DWORD (*get_value) (skin_item_t *item);
    /** 
     * Operations for setting item value.
     * For button item, value is the click status, non-zero for clicked 
     * or checked. For slider, value is the position.
     * For label item, value is the label text.
     * For control item, value is the handle to control.
     */
    DWORD (*set_value) (skin_item_t *item, DWORD status);
    
    /** Item msg proc */
    int (*item_msg_proc) (skin_item_t* item, int message, 
                    WPARAM wparam, LPARAM lparam);
};

/** Slider information structure */
typedef struct sie_slider_s
{
    /** The minimal position of the thumb. */
    int min_pos;
    /** The maximal position of the thumb. */
    int max_pos;
    /** The current position of the thumb. */
    int cur_pos;
} sie_slider_t;

/** Normal label item info structure */
typedef struct si_nrmlabel_s
{
    /** The label string. */
    char* label;        

    /** The normal text color. */
    DWORD color;        
    /** The focus text color. */
    DWORD color_focus;  
    /** The click text color. */
    DWORD color_click;  
    /** The index of the logical font in the skin fonts array. */
    int font_index;
} si_nrmlabel_t;

/** Bitmap label item info structure */
typedef struct si_bmplabel_s
{
    /** The label string. */
    char* label;
    /** All label characters in the bitmap. */
    const char* label_chars;
} si_bmplabel_t;

/** Normal slider item info structure */
typedef struct si_nrmslider_s
{
    /** The slider information */
    sie_slider_t    slider_info;

    /** The index of the thumb bitmap in the skin bitmaps array. */
    int thumb_bmp_index;
} si_nrmslider_t;

#ifdef _MGHAVE_FIXED_MATH

/** Rotation slider item info structure */
typedef struct si_rotslider_s
{
    /** The rotation radius. */
    int radius;
    /** The start degree. */
    int start_deg;
    /** The end degree. */
    int end_deg;
    /** The current degree. */
    int cur_pos;

    /** The index of the thumb bitmap in the skin bitmaps array. */
    int thumb_bmp_index;
} si_rotslider_t;

#endif /* _MGHAVE_FIXED_MATH */

/**
 * \def SIE_BUTTON_CLICKED
 * \sa skin_event_cb_t
 */
#define SIE_BUTTON_CLICKED      0x00000001

/**
 * \def SIE_SLIDER_CHANGED
 * \sa skin_event_cb_t
 */
#define SIE_SLIDER_CHANGED      0x00000002

/**
 * \def SIE_GAIN_FOCUS
 * \sa skin_event_cb_t
 */
#define SIE_GAIN_FOCUS          0x00010000

/**
 * \def SIE_LOST_FOCUS
 * \sa skin_event_cb_t
 */
#define SIE_LOST_FOCUS          0x00020000

/**
 * \var int (* skin_event_cb_t) (HWND hwnd, skin_item_t* item, 
                int event, void* data)
 *
 * Event callback of skin item.
 *
 * The event can be one of the following values:
 *
 *  - SIE_GAIN_FOCUS\n
 *    The item gained the focus.
 *  - SIE_LOST_FOCUS\n
 *    The item losted the focus.
 *  - SIE_BUTTON_CLICKED\n
 *    The button item has been clicked.
 *  - SIE_SLIDER_CHANGED\n
 *    The position of the slider item has changed.
 */
typedef int (* skin_event_cb_t) (HWND hwnd, skin_item_t* item, 
                int event, void* data);
/**
 * \def MSG_CB_GOON
 * \sa skin_msg_cb_t
 */
#define MSG_CB_GOON     0

/**
 * \def MSG_CB_DEF_GOON
 * \sa skin_msg_cb_t
 */
#define MSG_CB_DEF_GOON 1

/**
 * \def MSG_CB_STOP
 * \sa skin_msg_cb_t
 */
#define MSG_CB_STOP     2

/**
 * \var int (* skin_msg_cb_t) (HWND hwnd, int message, 
                WPARAM wparam, LPARAM lparam, int* result)
 *
 * This is the type of message callback function of a skin window. 
 *
 * Before a skin window processes a message, it will call the message 
 * callback function if the application defined it.
 *
 * The application defined message callback function can process the message 
 * as a normal window procedure and return the result through the parameter 
 * \a result. 
 * However, the return value of a message callback function can be used
 * to control the behavior of the skin window's message handling procedure. 
 * It can be one of the following values:
 *
 *  - MSG_CB_GOON
 *    The message should be processed by skin window procedure, and
 *    the result of the callback will be ignored.
 *  - MSG_CB_DEF_GOON
 *    The message should be processed by the default window procedure, and
 *    the result of the callback will be ignored.
 *  - MSG_CB_STOP
 *    The message should not be processed, and the result is valid.
 */
typedef int (* skin_msg_cb_t) (HWND hwnd, int message, 
                WPARAM wparam, LPARAM lparam, int* result);

/**
 * \def SKIN_STYLE_NONE
 * \brief The style of the skin is none.
 */
#define SKIN_STYLE_NONE     0x00000000

/**
 * \def SKIN_STYLE_TOOLTIP
 * The skin has tooltip window.
 */
#define SKIN_STYLE_TOOLTIP  0x00000001

/**
 * \def SKIN_STYLE_MODAL
 * \brief The skin has modal window.
 */
#define SKIN_STYLE_MODAL    0x00000002

/** Skin header information structure */
struct skin_head_s
{
    /** The name of the skin. */
    char* name;

    /**
     * The style of the skin, can be OR'ed by the following values:
     *  - SKIN_STYLE_TOOLTIP
     *    The skin has tooltip window.
     */
    DWORD style;

    /** The pointer to the array of the bitmaps used by the skin. */
    const BITMAP* bmps;
    /** The pointer to the array of the logical fonts used by the skin. */
    const LOGFONT* fonts;

    /** The index of the background bitmap in the bitmap array. */
    int bk_bmp_index;

    /** The number of the items in this skin. */
    int nr_items;
    /** The pointer to the array of skin items. */
    skin_item_t* items;

    /** The attached private data with the skin by application. */
    DWORD attached;

    /******** start of internal fields ********/

    /* The event callback function of the skin. */
    skin_event_cb_t event_cb;

    /* The window message procedure of the skin window. */
    skin_msg_cb_t msg_cb;

    /**** The fields initialized when skin_init called. ****/
    /* The rectangle heap for hit-test regions. */
    BLOCKHEAP rc_heap;

    /**** 
     * The fields initialized when create_skin_main_window or 
     * create_skin_control called.
     ****/
    /* The handle of window in which the skin locates. */
    HWND hwnd;
    /* The handle of tool tip window. */
    HWND tool_tip;

    /* The old x and y. */
    int oldx, oldy;
    /* The current hilighted item. */
    skin_item_t* hilighted;

    /* cached identifier. */
    int cached_id;
    /* the item whose identifier is cached_id. */
    skin_item_t* cached_item;
};

/**
 * \fn BOOL skin_init (skin_head_t* skin, \
 *               skin_event_cb_t event_cb, skin_msg_cb_t msg_cb)
 * \brief Initializes a skin.
 *
 * This function initializes a skin \a skin before it can be used.
 * The initialization of a skin includes skin items initialization, etc, 
 * such as creating the hit-test region, allocating space for label, and so on.
 *
 * \param skin The pointer to the skin object. The skin can be
 *        a object returned by a hard-coded
 *        skin_head_t structure.
 * \param event_cb The item event callback of the skin.
 * \param msg_cb The window message callback of the skin window.
 *
 * \return TRUE for success, otherwise FALSE.
 *
 */
MGUTILS_EXPORT BOOL skin_init (skin_head_t* skin, 
                skin_event_cb_t event_cb, skin_msg_cb_t msg_cb);

/**
 * \fn void skin_deinit (skin_head_t* skin)
 * \brief Deinitializes an initialized skin.
 *
 * This function deinitializes a skin, opposed to skin_init.
 *
 * \param skin The pointer to the skin object.
 *
 * \sa skin_init
 */
MGUTILS_EXPORT void skin_deinit (skin_head_t* skin);

/**
 * \fn HWND create_skin_main_window_ex (skin_head_t* skin, HWND hosting, \
 *               int lx, int ty, int rx, int by, DWORD dwExStyle, BOOL modal)
 * \brief Creates a main window for a skin.
 *
 * This function creates a main window for the skin pointed to by \a skin.
 * The main window will have no caption, no menu, and no frame. However,
 * the window's caption string will be the name of the skin object.
 *
 * \param skin The pointer to the initialized skin object.
 * \param hosting The the hosting main window.
 * \param lx X-coordinate of the upper-left corner of the expected main window.
 * \param ty Y-coordinate of the upper-left corner of the expected main window.
 * \param rx X-coordinate of the lower-right corner of the expected main window.
 * \param by Y-coordinate of the lower-right corner of the expected main window.
 * \param dwExStyle The extended style of the expected control.
 * \param modal Whether to be a modal or modeless main window.
 *
 * \return The handle to the main window.
 *
 * \note The skin will store itself as the main window's additional data,
 *       so application should not call SetWindowAdditionalData to store
 *       other value.
 *
 * \sa create_skin_control
 */

MGUTILS_EXPORT HWND create_skin_main_window_ex (skin_head_t* skin, HWND hosting, 
                int lx, int ty, int rx, int by, DWORD dwExStyle, BOOL modal);
/**
 * \def create_skin_main_window(skin, hosting, lx, ty, rx, by, modal)
 * \sa create_skin_main_window_ex (skin, hosting, lx, ty, rx, by, 0, modal)
 */
#define create_skin_main_window(skin, hosting, lx, ty, rx, by, modal) \
                create_skin_main_window_ex (skin, hosting, lx, ty, rx, by, 0, modal)
/**
 * \fn HWND create_skin_control (skin_head_t* skin, HWND parent, int id, \
 *               int x, int y, int w, int h)
 * \brief Creates a control for a skin.
 *
 * This function creates a control for the skin pointed to by \a skin.
 *
 * \param skin The pointer to the initialized skin object.
 * \param parent The handle to the parent window of the expected control.
 * \param id The identifier of the expected control.
 * \param x X-coordinate of the expected control in the parent window's 
 *        client area.
 * \param y Y-coordinate of the expected control in the parent window's 
 *        client area.
 * \param w The width of the expected control.
 * \param h The height of the expected control.
 *
 * \return The handle to the control.
 *
 * \note The skin will store itself as the control's additional data,
 *       so application should not call SetWindowAdditionalData to store 
 *       other value.
 *
 *       You can also create a skin control by calling CreateWindowEx in 
 *       the following manner:
 * \code
 *     CreateWindowEx (CTRL_SKIN, "", WS_VISIBLE | WS_CHILD, WS_EX_NONE, 
 *                     id, x, y, w, h, parent, (DWORD) skin);
 * \endcode
 *
 * \sa create_skin_main_window
 */
MGUTILS_EXPORT HWND create_skin_control (skin_head_t* skin, HWND parent, 
                int id, int x, int y, int w, int h);

/**
 * \fn BOOL is_skin_main_window (HWND hwnd)
 * \brief Determines whether a window is a skin main window or not.
 *
 * The function determines whether a window specified by \a hwnd is a 
 * main window created by skin_main_window or not.
 *
 * \param hwnd The handle to the skin window.
 *
 * \return TRUE if it is , and FALSE if it isn't.
 *
 * \sa create_skin_main_window
 */
MGUTILS_EXPORT BOOL is_skin_main_window (HWND hwnd);

/**
 * \fn void destroy_skin_window (HWND hwnd)
 * \brief Destroys a skin window.
 *
 * The function destroys the skin main window or control.
 *  
 * \param hwnd The handle to the skin window.
 *
 * \sa create_skin_main_window, create_skin_control
 */
MGUTILS_EXPORT void destroy_skin_window (HWND hwnd);

/**
 * \fn skin_head_t* set_window_skin (HWND hwnd, skin_head_t *new_skin)
 * \brief Sets new skin.
 *
 * The function sets a new skin pointed to by \a new_skin for
 * the skin window \a hwnd.
 *
 * \param hwnd The handle to the skin window.
 * \param new_skin The pointer to the new skin object.
 *
 * \return The pointer to the old skin object.
 *
 * \sa skin_set_event_cb, skin_set_msg_cb
 */
MGUTILS_EXPORT skin_head_t* set_window_skin (HWND hwnd, skin_head_t *new_skin);

/**
 * \fn skin_head_t* get_window_skin (HWND hwnd)
 * \brief Gets new skin.
 *
 * The function gets the skin of skin window \a hwnd.
 *
 * \param hwnd The handle to the skin window.
 *
 * \return The pointer to the skin object.
 *
 * \sa skin_set_event_cb, skin_set_msg_cb
 */
MGUTILS_EXPORT skin_head_t* get_window_skin (HWND hwnd);

/**
 * \fn skin_event_cb_t skin_set_event_cb (skin_head_t* skin, \
 *               skin_event_cb_t event_cb)
 * \brief Sets new event callback of a skin.
 *
 * This function sets \a event_cb as the event callback of the skin \a skin.
 *
 * \param skin The pointer to the skin object.
 * \param event_cb The new event callback.
 *
 * \return The old event callback.
 *
 * \sa skin_set_msg_cb
 */
static inline skin_event_cb_t skin_set_event_cb (skin_head_t* skin, 
                skin_event_cb_t event_cb)
{
    skin_event_cb_t old = skin->event_cb;
    skin->event_cb = event_cb;
    return old;
}

/**
 * \fn skin_msg_cb_t skin_set_msg_cb (skin_head_t* skin, \
 *               skin_msg_cb_t msg_cb)
 * \brief Sets new message callback of a skin window.
 *
 * This function sets \a msg_cb as the message callback of the skin \a skin.
 *
 * \param skin The pointer to the skin object.
 * \param msg_cb The new message callback.
 *
 * \return The old message callback.
 *
 * \sa skin_set_event_cb
 */
static inline skin_msg_cb_t skin_set_msg_cb (skin_head_t* skin, 
                skin_msg_cb_t msg_cb)
{
    skin_msg_cb_t old = skin->msg_cb;
    skin->msg_cb = msg_cb;
    return old;
}

/**
 * \fn skin_item_t* skin_get_item (skin_head_t* skin, int id)
 * \brief Retrives a skin item through its identifier.
 *
 * This function retrives the pointer to a skin item by its identifier.
 *
 * \param skin The pointer to the skin.
 * \param id The identifier of the skin item.
 *
 * \return The pointer to the skin item, NULL on error.
 *
 * \sa skin_item_s
 */
MGUTILS_EXPORT skin_item_t* skin_get_item (skin_head_t* skin, int id);

/**
 * \fn DWORD skin_get_item_status (skin_head_t* skin, int id)
 * \brief Retrives the common status of a skin item.
 *
 * This function retrives the common status of a skin item by its identifier.
 *
 * \param skin The pointer to the skin.
 * \param id The identifier of the skin item.
 *
 * \return The common status of the skin items, zero on error.
 *
 * \sa skin_item_s, skin_show_item, skin_enable_item, skin_set_hilited_item, skin_get_item_status
 */
MGUTILS_EXPORT DWORD skin_get_item_status (skin_head_t* skin, int id);

/**
 * \fn DWORD skin_set_item_status ( skin_head_t* skin, int id, DWORD status)
 * \brief Set the common status of a skin item.
 *
 * This function set the common status of a skin item by its identifier.
 *
 * \param skin The pointer to the skin.
 * \param id The identifier of the skin item.
 * \param status The common status of the skin item.
 *
 * \return The old common status of the skin items.
 *
 * \sa skin_get_item_status
 */
MGUTILS_EXPORT DWORD skin_set_item_status (skin_head_t* skin, int id, DWORD status);

/**
 * \fn skin_item_t* skin_get_hilited_item (skin_head_t* skin)
 * \brief Gets the current highlighted skin item.
 *
 * \param skin The pointer to the skin.
 *
 * \return The pointer to the current highlighted skin item.
 *
 * \sa skin_item_s, skin_set_hilited_item
 */
MGUTILS_EXPORT skin_item_t* skin_get_hilited_item (skin_head_t* skin);

/**
 * \fn DWORD skin_show_item (skin_head_t* skin, int id, BOOL show)
 * \brief Showes or hides a skin item.
 *
 * This function showes or hides a skin item whose identifier is \a id.
 *
 * \param skin The pointer to the skin.
 * \param id The identifier of the skin item.
 * \param show TRUE for show, FALSE for hide.
 *
 * \return The current common status of the skin items, zero when error.
 *
 * \sa skin_item_s, skin_get_item_status
 */
MGUTILS_EXPORT DWORD skin_show_item (skin_head_t* skin, int id, BOOL show);

/**
 * \fn BOOL skin_enable_item (skin_head_t* skin, int id, BOOL enable)
 * \brief Enables or disables a skin item.
 *
 * This function enables or disables a skin item whose identifier is \a id.
 *
 * \param skin The pointer to the skin.
 * \param id The identifier of the skin item.
 * \param enable TRUE for enable, FALSE for disable.
 *
 * \return The current common status of the skin items, zero when error.
 *
 */
MGUTILS_EXPORT DWORD skin_enable_item (skin_head_t* skin, int id, BOOL enable);

/**
 * \fn skin_item_t* skin_set_hilited_item (skin_head_t* skin, int id)
 * \brief Sets the current highlighted skin item.
 *
 * This function sets the current highlighted skin item.
 *
 * \param skin The pointer to the skin.
 * \param id The identifier of the skin item.
 *
 * \return The pointer to the old highlighted skin item.
 *
 * \sa skin_item_s, skin_get_hilited_item
 */
MGUTILS_EXPORT skin_item_t* skin_set_hilited_item (skin_head_t* skin, int id);

/**
 * \fn BOOL skin_get_check_status (skin_head_t* skin, int id)
 * \brief Gets the check status of a check button item.
 *
 * This function gets the check status of a check button item,
 * checked or unchecked.
 *
 * \param skin The pointer to the skin.
 * \param id The identifier of the skin item.
 *
 * \return TRUE for checked, FALSE for unchecked.
 *
 * \sa skin_item_s, skin_set_check_status
 */
MGUTILS_EXPORT BOOL skin_get_check_status (skin_head_t* skin, int id);

/**
 * \fn DWORD skin_set_check_status (skin_head_t* skin, \
 *               int id, BOOL check)
 * \brief Sets the check status of a check button item.
 *
 * This function sets the check status of a check button item.
 *
 * \param skin The pointer to the skin.
 * \param id The identifier of the skin item.
 * \param check Checks it or not.
 *
 * \return The current common status of the check button item.
 *
 * \sa skin_item_s, skin_get_check_status
 */
MGUTILS_EXPORT DWORD skin_set_check_status (skin_head_t* skin, 
                int id, BOOL check);

/**
 * \fn const char* skin_get_item_label (skin_head_t* skin, int id)
 * \brief Gets the label string of a label item.
 *
 * This function returns the label string of a label item whose
 * identifier is \a id.
 *
 * \param skin The pointer to the skin.
 * \param id The identifier of the skin item.
 *
 * \return The pointer to the label string. You should not
 *         modify the content of the string directly.
 *
 * \sa skin_item_s, skin_set_item_label
 */
MGUTILS_EXPORT const char* skin_get_item_label (skin_head_t* skin, int id);

/**
 * \fn BOOL skin_set_item_label (skin_head_t* skin, \
 *               int id, const char* label)
 * \brief Sets the label string of a label item.
 *
 * This function sets the label string of a label item and reflects
 * the appearance of the item if it is visible.
 *
 * \param skin The pointer to the skin.
 * \param id The identifier of the skin item.
 * \param label The pointer to the new label string.
 *
 * \return TRUE for success, FALSE on error.
 *
 * \sa skin_item_s, skin_get_item_label
 */
MGUTILS_EXPORT BOOL skin_set_item_label (skin_head_t* skin, 
                int id, const char* label);

/**
 * \fn HWND skin_get_control_hwnd (skin_head_t* skin, int id)
 * \brief Gets the handle to a skin control item in the skin.
 *
 * This function returns the handle to a skin control item whose
 * identifier is \a id.
 *
 * \param skin The pointer to the skin.
 * \param id The identifier of the skin control item.
 *
 * \return The handle to the control.
 *
 * \sa skin_item_s
 */
MGUTILS_EXPORT HWND skin_get_control_hwnd (skin_head_t* skin, int id);

/**
 * \fn int skin_get_thumb_pos (skin_head_t* skin, int id)
 * \brief Gets the thumb position of a slider item.
 *
 * This function gets the thumb position of a slider item.
 *
 * \param skin The pointer to the skin.
 * \param id The identifier of the skin item.
 *
 * \return The current thumb position.
 *
 * \sa skin_item_s, skin_set_thumb_pos
 */
MGUTILS_EXPORT int skin_get_thumb_pos (skin_head_t* skin, int id);

/**
 * \fn BOOL skin_set_thumb_pos (skin_head_t* skin, int id, int pos)
 * \brief Sets the thumb position of a slider item.
 *
 * This function sets the thumb position of a slider item whose identifier
 * is \a id to the value of \a pos. It will redraw the item if the position
 * changed.
 *
 * \param skin The pointer to the skin.
 * \param id The identifier of the skin item.
 * \param pos The new position value.
 *
 * \return TRUE for success, FALSE on error.
 *
 * \sa skin_item_s, skin_get_thumb_pos
 */
MGUTILS_EXPORT BOOL skin_set_thumb_pos (skin_head_t* skin, int id, int pos);

/**
 * \fn BOOL skin_get_slider_info (skin_head_t* skin, \
 *               int id, sie_slider_t* sie)
 * \brief Retrives the sliding information of a slider item.
 *
 * This function retrives the sliding information of a slider item whose
 * identifier is \a id.
 *
 * \param skin The pointer to the skin.
 * \param id The identifier of the skin item.
 * \param sie The pointer to a buffer which will be used to return the
 *        sliding information.
 *
 * \return TRUE for success, FALSE on error.
 *
 * \sa skin_item_s, skin_set_slider_info
 */
MGUTILS_EXPORT BOOL skin_get_slider_info (skin_head_t* skin, 
                int id, sie_slider_t* sie);

/**
 * \fn BOOL skin_set_slider_info (skin_head_t* skin,  \
 *               int id, const sie_slider_t* sie)
 * \brief Sets the sliding information of a slider item.
 *
 * This function sets the sliding information of a slider item whose
 * identifier is \a id.
 *
 * \param skin The pointer to the skin.
 * \param id The identifier of the skin item.
 * \param sie The pointer to a sliding information structure.
 *
 * \return TRUE for success, FALSE on error.
 *
 */
MGUTILS_EXPORT BOOL skin_set_slider_info (skin_head_t* skin, 
                int id, const sie_slider_t* sie);

/**
 * \fn int skin_scale_slider_pos (const sie_slider_t* org, \
 *               int new_min, int new_max)
 * \brief Scales the sliding position.
 *
 * This function is a helper function, it calculates and returns 
 * the new position value of a sliding information in the new sliding range
 * defined by \a new_max and \a new_min.
 * 
 * \param org The original sliding information, its range and current 
 *        position value.
 * \param new_min The new minimal value.
 * \param new_max The new maximal value.
 *
 * \return The new position value.
 *
 * \sa sie_slider_t
 */
MGUTILS_EXPORT int skin_scale_slider_pos (const sie_slider_t* org, 
                int new_min, int new_max);

    /** @} end of skin_fns */

#endif /*  _MGUTILS_SKIN */

#ifdef  __cplusplus
}
#endif

#endif /* _MGUTILS_H */
