/*
 * \file mmainwnd.h
 * \author dongjunjie
 * \date 2009/01/01
 *
 * This file include the main window defines
 *
 \verbatim

    This file is part of mGNCS, a component for MiniGUI.

    Copyright (C) 2008~2018, Beijing FMSoft Technologies Co., Ltd.

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

#ifndef _MGUI_CTRL_MNWND_H
#define _MGUI_CTRL_MNWND_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \defgroup MainWnd mMainWnd
 * @{
 */

/**
 * \def NCSCTRL_MAINWND
 * \brief the name of mMainWnd control
 */
#define NCSCTRL_MAINWND NCSCLASSNAME("mainwnd")

typedef struct _mMainWnd mMainWnd;
typedef struct _mMainWndClass mMainWndClass;
typedef struct _mMainWndRenderer mMainWndRenderer;

/*define mMainWndRender */
#define mMainWndRendererHeader(clsName, parentClass) \
	mPanelRendererHeader(clsName, parentClass)

/**
 * \struct mMainWndRenderer
 * \brief the \ref mMainWnd renderer interface, inherit from mPanelRenderer
 *
 * \sa mMainWnd, mMainWndClass
 */

struct _mMainWndRenderer
{
	mMainWndRendererHeader(mMainWnd, mPanel)
};

/* define mMainWnd */
#define mMainWndHeader(className) \
	mPanelHeader(className) \
	mComponent* parentCmp; \
	mComponent* nextCmp, *prevCmp;

/**
 * \struct mMainWnd
 * \brief define the memders of mMainWnd, inherit from mPanel
 *
 * \sa mMainWndClass
 */
struct _mMainWnd {
	mMainWndHeader(mMainWnd)
};

#define MSG_MNWND_ENDDIALOG   MSG_LASTUSERMSG + 4
//lParam is exit code
//wParam is the special flag, used by interal

/* define mMainWndClass */
#define mMainWndClassHeader(clsName, parentClass) \
	mPanelClassHeader(clsName, parentClass) \
	DWORD (*doModal)(clsName*,BOOL AutoDestroy);    \
	BOOL (*endDialog)(clsName*, DWORD code);

/**
 * \struct mMainWndClass
 * \brief the VTable of mMainWnd, inherit from mPanelClass
 *
 * - DWORD (*\b doModal)(mMainWnd *self, BOOL bAutoDestroy)
 *   Show a Model MainWindow.
 *   \param bAutoDestroy auto destroy the main window when mode exit.
 *   \return DWORD the value send by endDialog.
 *
 * - BOOL (*\b endDialog)(mMainWnd *self, DWORD code)
 *   End the model window.
 *   \param code the End code send to main window, would be returned by doModal.
 *   \return BOOL Sucessed or Failed.
 */
struct _mMainWndClass
{
	mMainWndClassHeader(mMainWnd, mPanel)
};

#define DECLARE_MAINWND_CLASS(clss, clssSuper)      \
	typedef mMainWndRenderer clss##Renderer;        \
	DECLARE_CLASS(clss, clssSuper)

#define NCS_MAINWND_ISMODE(obj)  (INSTANCEOF(obj, mMainWnd) && (GetWindowStyle(obj->hwnd) & NCSS_MNWND_MODE))
/**
 * \var g_stmMainWndCls
 * \brief the singleton of mMainWndClass
 */
MGNCS_EXPORT extern mMainWndClass g_stmMainWndCls; //Class(mMainWnd);

enum enumMainWndProp {
	NCSP_MNWND_MAX = NCSP_PANEL_MAX + 1
};

/**
 * \def NCSS_MNWND_MODE
 * \brief indicate the main window is mode or not
 * \note this style is used by inner, you can read this style to know
 *  the main window is mode or not , it's READONLY
 */
#define NCSS_MNWND_MODE  (1<<NCSS_PANEL_SHIFT)
#define NCSS_MNWND_SHIFT (NCSS_PANEL_SHIFT+1)
enum enumMainWndNotify{
	NCSN_MNWND_MAX = NCSN_PANEL_MAX + 1
};

/** @} end of MainWnd */

#ifdef __cplusplus
}
#endif

#endif

