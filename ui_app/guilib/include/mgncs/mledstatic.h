/**
 * \file 
 * \author 
 * \date 
 *
 * \brief 
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

#ifdef _MGNCSCTRL_LEDLABEL

#ifndef _MGUI_NCSCTRL_LEDLABEL_H
#define _MGUI_NCSCTRL_LEDLABEL_H

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

/**
 * \defgroup ControlLedStatic mLEDLabel
 * @{
 */

#include "mwidget.h"

/**
 * \def NCSCTRL_LEDLABEL
 * \brief the name of mLEDLabel
 */

#define NCSCTRL_LEDLABEL   NCSCLASSNAME("ledlabel")


typedef struct _mLEDLabel mLEDLabel;
typedef struct _mLEDLabelClass mLEDLabelClass;
typedef struct _mLEDLabelRenderer mLEDLabelRenderer;

/* define mLEDLabel */
#define mLEDLabelHeader(clsName)  \
	mStaticHeader(clsName)

/**
 * \struct mLEDLabel
 * \brief LedLabel class, derived from \ref mStatic
*/

struct _mLEDLabel
{
	mLEDLabelHeader(mLEDLabel)
};

/* define mLEDLabelClass */
#define mLEDLabelClassHeader(clsName, parentClass)    \
	mStaticClassHeader(clsName, parentClass)

/**
 * \struct mLEDLabelClass
 * \brief VTable of mLEDLabel, derived from \ref mStaticClass
 */

struct _mLEDLabelClass
{
	mLEDLabelClassHeader(mLEDLabel, mStatic)
};


/* define IRendererStatic */
#define mLEDLabelRendererHeader(clsName, parentClass) \
	mStaticRendererHeader(clsName, parentClass)

/**
 * \struct mLEDLabelRenderer
 * \brief mLEDLabel renderer interface, derived from \ref mStaticRenderer
 */
struct  _mLEDLabelRenderer {
	mLEDLabelRendererHeader(mLEDLabel, mStatic)
};

/**
 * \var g_stmButtonCls
 * \brief global mLEDLabelClass
 */
MGNCS_EXPORT extern mLEDLabelClass g_stmLEDLabelCls;

/* define property of static */

/**
 * \enum mLEDLabelProp
 * \brief the properties id of \ref mLEDLabel
 */

enum mLEDLabelProp {
	NCSP_LEDLBL_COLOR = NCSP_STATIC_MAX + 1,
	NCSP_LEDLBL_WIDTH,
	NCSP_LEDLBL_HEIGHT,
	NCSP_LEDLBL_GAP,
	NCSP_LEDLBL_MAX,
};

/**
 * \enum enumLEDLabelNotify
 * \brief the notification code of \ref mLEDLabel
 */
enum enumLEDLabelNotify {
	NCSN_LEDLBL_MAX = NCSN_STATIC_MAX + 1
};

// BOOL ncsLedDrawText(HDC hdc, char *str, int len, const RECT *rect, DWORD uflag, int width, int height);

/**
 * @} end of ControlLedStatic
 */
#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif /* _MGUI_NCSCTRL_LEDLABEL_H */
#endif //_MGNCSCTRL_LEDLABEL
