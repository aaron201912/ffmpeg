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

#ifndef _MGUI_NCSCTRL_CHKBTN_H
#define _MGUI_NCSCTRL_CHKBTN_H
 

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

/**
 * \defgroup ControlCheckButton mCheckButton
 * @{
 */

/**
 * \def NCSCTRL_CHECKBUTTON
 * \brief the name of checkbutton class
 */

#define NCSCTRL_CHECKBUTTON        NCSCLASSNAME("checkbutton")

typedef struct _mCheckButton mCheckButton;
typedef struct _mCheckButtonClass mCheckButtonClass;
typedef struct _mCheckButtonRenderer mCheckButtonRenderer;

#define mCheckButtonHeader(className) \
	mButtonHeader(className) 

/**
 * \struct mCheckButton
 * \brief CheckButton, derived from \ref mButton
 */

struct _mCheckButton
{
	mCheckButtonHeader(mCheckButton)
};

#define mCheckButtonClassHeader(className, parentClass) \
	mButtonClassHeader(className, parentClass)

/**
 * \struct mCheckButtonClass
 * \brief checkbutton class struct of button control, derived from mButtonClass
 */
 
struct _mCheckButtonClass
{
	mCheckButtonClassHeader(mCheckButton, mButton)
};

#define mCheckButtonRendererHeader(clsName, parentClass) \
	mButtonRendererHeader(clsName, parentClass) \
    void (*drawCheckbutton)(clsName *self, HDC hdc, const RECT* pRect, int status);

/**
 * \struct mCheckButtonRenderer
 * \brief Checkbutton class renderer interface, derived from \ref mButtonRenderer
 *
 *  \sa mCheckButton, mCheckButtonClass, mButton, mButtonClass
 *
 * - void (\b *drawCheckbutton)( clsName \a *self, HDC \a hdc, const Rect \a *pRect, int \a status);
 *   draw checkbutton using renderer
 *   \param *self - this pointer of checkbutton
 *   \param hdc - DC of control
 *   \param *pRect - the rectangle of checkbutton frame
 *   \param status - the button state
 *
 */

/* define the render of image static */
struct _mCheckButtonRenderer {
	mCheckButtonRendererHeader(mCheckButton, mButton)
};

/**
 * \var g_stmCheckButtonCls
 * \brief global mCheckButtonClass
 */
MGNCS_EXPORT extern mCheckButtonClass g_stmCheckButtonCls;

/**
 * \enum mCheckButtonProp
 * \brief the properties id of \ref mCheckButton, derived from mButtonProp
 */
enum mCheckButtonProp{
	NCSP_CHKBTN_MAX = NCSP_BUTTON_MAX + 1
};

/**
 * \enum mCheckButtonNotify
 * \brief the notification id of \ref mCheckButton
 */

enum mCheckButtonNotify{
	NCSN_CHKBTN_MAX = NCSN_BUTTON_MAX + 1
};

#define NCSS_CHKBTN_SHIFT NCSS_BUTTON_SHIFT

/**
 * @} end of ControlCheckButton
 */
#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif /* _MGUI_NCSCTRL_RECTANGLE_H */

