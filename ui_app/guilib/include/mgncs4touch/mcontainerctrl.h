/*
 * \file 
 * \author FMSoft
 * \date 
 *
 \verbatim

    This file is part of mGNCS4Touch, one of MiniGUI components.

    Copyright (C) 2008-2018 FMSoft (http://www.fmsoft.cn).

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

#ifndef _MGUI_NCSCTRL_CTNR_H
#define _MGUI_NCSCTRL_CTNR_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define NCSCTRL_CONTAINERCTRL NCSCLASSNAME("containerctrl")

typedef struct _mContainerCtrl mContainerCtrl;
typedef struct _mContainerCtrlClass mContainerCtrlClass;
typedef struct _mContainerCtrlRenderer mContainerCtrlRenderer;

#define mContainerCtrlHeader(clsName)  \
    mWidgetHeader(clsName)

struct _mContainerCtrl
{
    mContainerCtrlHeader(mContainerCtrl)
};

#define mContainerCtrlClassHeader(clsName, parentClass) \
    mWidgetClassHeader(clsName, parentClass)            \
    void (*setBody)(clsName *, mHotPiece*);

struct _mContainerCtrlClass
{
    mContainerCtrlClassHeader(mContainerCtrl, mWidget)
};

#define mContainerCtrlRendererHeader(clsName, parentClass) \
    mWidgetRendererHeader(clsName, parentClass)

struct _mContainerCtrlRenderer
{
    mContainerCtrlRendererHeader(mContainerCtrl, mWidget)
};

MGNCS_EXPORT extern mContainerCtrlClass g_stmContainerCtrlCls;

enum mContainerCtrlProp
{
    NCSP_CTNRCTRL_MAX = NCSP_WIDGET_MAX + 1
};

#define NCSS_CTNRCTRL_SHIFT NCSS_WIDGET_SHIFT

enum mContainerCtrlNotify
{
    NCSN_CTNRCTRL_MAX = NCSN_WIDGET_MAX + 1
};

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _MGUI_NCSCTRL_CTNR_H */
