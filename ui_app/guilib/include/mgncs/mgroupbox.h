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

#ifndef _MGUI_NCSCTRL_GRPBOX_H
#define _MGUI_NCSCTRL_GRPBOX_H

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

/**
 * \defgroup Control_GroupBox mGroupBox
 * @{
 */

/**
 * \def NCSCTRL_GROUPBOX
 * \brief the name of groupbox control
 */

#define NCSCTRL_GROUPBOX        NCSCLASSNAME("groupbox")

typedef struct _mGroupBox mGroupBox;
typedef struct _mGroupBoxClass mGroupBoxClass;
typedef struct _mGroupBoxRenderer mGroupBoxRenderer;

#define mGroupBoxHeader(clsName) \
	mStaticHeader(clsName) 

/**
 * \struct mGroupBox
 * \brief groupbox class, derived from \ref mStatic
 */

struct _mGroupBox
{
	mGroupBoxHeader(mGroupBox)
};

#define mGroupBoxClassHeader(clsName, parentClass)  \
	mStaticClassHeader(clsName, parentClass)        \
    int (*hitTest)(clsName *self, int x, int y);

/**
 * \struct mGroupBoxClass
 * \brief struct of groupbox control, derived from \ref mStaticClass
 *
 * - int (\b *hitTest)(clsName \a *self, int \a x, int \a y); \n
 *   test mouse hit position
 *   \param *self - this pointer of groupbox
 *   \param x - x value of mouse position
 *   \param y - y value of mouse position
 *   \return HT_OUT - mouse hit out of groupbox, HT_TRANSPARENT - mouse hit inside the controlset area of groupbox, HT_CLIENT - mouse hit in groupbox
 */

struct _mGroupBoxClass
{
	mGroupBoxClassHeader(mGroupBox, mStatic)
};

#define mGroupBoxRendererHeader(clsName, parentClass) \
	mStaticRendererHeader(clsName, parentClass) \
    void (*drawGroupbox)(clsName *self, HDC hdc, const RECT *pRect);

/**
 * \struct mGroupBoxRenderer
 * \brief groupbox class renderer interface, derived from \ref mStaticRenderer
 *
 * - void (\b *drawGroupbox)(clsName \a *self, HDC \a hdc, const RECT \a *pRect); \n
 *   draw groupbox using renderer
 *   \param *self - this pointer of groupbox
 *   \param hdc - dc of groupbox control
 *   \param *pRect - rectangle to draw groupbox
 */

/* define the render of image static */
struct _mGroupBoxRenderer {
	mGroupBoxRendererHeader(mGroupBox, mStatic)
};

/**
 * \var g_stmGroupBoxCls
 * \brief global mGroupBoxClass
 */

MGNCS_EXPORT extern mGroupBoxClass g_stmGroupBoxCls;

/**
 * \enum mGroupBoxProp
 * \brief the properties id of \ref mGroupBox
 */

/* property */
enum mGroupBoxProp {
	NCSP_GRPBOX_MAX = NCSP_STATIC_MAX + 1
};

#define NCSS_GRPBOX_SHIFT NCSS_STATIC_SHIFT

/**
 * \enum enumGroupBoxNotify
 * \brief the notification code of \ref mGroupBox
 */
enum enumGroupBoxNotify{
	NCSN_GRPBOX_MAX = NCSN_STATIC_MAX + 1
};

/**
 * @} end of Control_GroupBox
 */
#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif /* _MGUI_NCSCTRL_GRPBOX_H */

