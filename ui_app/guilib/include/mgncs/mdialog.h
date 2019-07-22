/*
 * \file mmainwnd.h
 * \author dongjunjie
 * \date 2009/01/01
 *
 * This file include the dialog defines
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

#ifdef _MGNCSCTRL_DIALOGBOX

#ifndef _MGUI_CTRL_DLGBOX_H
#define _MGUI_CTRL_DLGBOX_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \defgroup DialogBox mDialogBox
 * @{
 */

/**
 * \def NCSCTRL_DIALOGBOX
 * \brief the name of \ref mDialogBox control
 */
#define NCSCTRL_DIALOGBOX NCSCLASSNAME("dialogbox")

typedef struct _mDialogBox mDialogBox;
typedef struct _mDialogBoxClass mDialogBoxClass;
typedef struct _mDialogBoxRenderer mDialogBoxRenderer;

/* define mDialogBoxRenderer */
#define mDialogBoxRendererHeader(clsName, parentClass) \
	mMainWndRendererHeader(clsName, parentClass)

/**
 * \struct mDialogBoxRenderer
 * \brief the renerer interface of \ref mDialogBox, inherit from \ref mMainWndRenderer
 *
 * \sa mDialogBox, mDialogBoxClass
 */

struct _mDialogBoxRenderer {
	mDialogBoxRendererHeader(mDialogBox, mMainWnd)
};

/* define mDialogBox */
#define mDialogBoxHeader(className) \
	mMainWndHeader(className)

/**
 * \struct mDialogBox
 * \brief the members of mDialogBox, inherit from mMainWnd
 *
 *  - DWORD \b modeRetValue;\n
 *    The return value after a dialog leave Mode state, return by \b doModal
 *
 * \sa mDialogBoxClass
 */
struct _mDialogBox{
	mDialogBoxHeader(mDialogBox)
};

/* define mDialogBoxClass */
#define mDialogBoxClassHeader(clsName, parentClass)    \
	mMainWndClassHeader(clsName, parentClass)

/**
 * \struct mDialogBoxClass
 * \brief the VTable of \ref mDialogBox, inherit from mMainWndClass
 *
 * \sa mDialogBox
 *
 */

struct _mDialogBoxClass
{
	mDialogBoxClassHeader(mDialogBox, mMainWnd)
};

/**
 * \var g_stmDialogBoxCls
 * \brief the singleton of mDialogBoxClass
 */
MGNCS_EXPORT extern mDialogBoxClass g_stmDialogBoxCls; //Class(mDialogBox);


#define NCSS_DLGBOX_SHIFT  NCSS_MNWND_SHIFT

enum enumDialogBoxNotify {
	NCSN_DLGBOX_MAX = NCSN_MNWND_MAX + 1
};

/** @} end of DialogBox */

#ifdef __cplusplus
}
#endif

#endif
#endif
