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

#ifdef _MGNCSCTRL_COLORBUTTON

#ifndef _MGUI_NCSCTRL_CLRBTN_H
#define _MGUI_NCSCTRL_CLRBTN_H


#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

/**
 * \defgroup ControlColorButton mColorButton
 * @{
 */

/**
 * \def NCSCTRL_COLORBUTTON
 * \brief the name of colorbutton control
 */
#define NCSCTRL_COLORBUTTON        NCSCLASSNAME("colorbutton")

typedef struct _mColorButton mColorButton;
typedef struct _mColorButtonClass mColorButtonClass;
typedef struct _mColorButtonRenderer mColorButtonRenderer;

#define mColorButtonHeader(clsName)   \
	mWidgetHeader(clsName)
/**
 * \struct mColorButton
 * \brief A Button used to select a color, derived from \ref mWidget
 *
 * \sa \ref mWidget
 */

struct _mColorButton
{
	mColorButtonHeader(mColorButton)
};

#define mColorButtonClassHeader(clsName, parentClass) \
	mWidgetClassHeader(clsName, parentClass)

/**
 * \struct mColorButtonClass
 * \brief struct of colorbutton control class, derived from \ref mWidgetClass
 *
 * \sa \ref mWidgetClass
 */

struct _mColorButtonClass
{
	mColorButtonClassHeader(mColorButton, mWidget)
};

#define mColorButtonRendererHeader(clsName, parentClass) \
	mWidgetRendererHeader(clsName, parentClass)

/**
 * \struct mColorButtonRenderer
 * \brief colorbutton renderer interface, derived from \ref mWidgetRenderer
 */
/* define the render of colorbutton */
struct _mColorButtonRenderer {
	mColorButtonRendererHeader(mColorButton, mWidget)
};

/**
 * \var g_stmColorButtonCls
 * \brief global mColorButtonClass
 */
MGNCS_EXPORT extern mColorButtonClass g_stmColorButtonCls;

/**
 * \enum mColorButtonProp
 * \brief properties id of mColorButton
 */
enum mColorButtonProp {
	/**
	 * \brief set/get the color value of referred controller
	 *  - Type: DWORD(ARGB)
	 *  - Read Write
	 *  - Change the color value of referred controller
	 */
	NCSP_CLRBTN_CURCOLOR = NCSP_WIDGET_MAX + 1,
	NCSP_CLRBTN_MAX
};

#define NCSS_CLRBTN_SHIFT NCSS_WIDGET_SHIFT
#define NCSS_CLRBTN_FLAT  (1<<NCSS_WIDGET_SHIFT)

/**
 * \enum enumColorButtonNotify
 * \brief notification code of mColorButton
 */
enum enumColorButtonNotify{
	/**
	 * \brief raised when user select color
	 * \param the value of the selected color (ARGB)
	 *
	 * \sa NCSP_CLRBTN_CURCOLOR
	 */
	NCSN_CLRBTN_COLORCHANGED = NCSN_WIDGET_MAX + 1,
	NCSN_CLRBTN_MAX
};

/**
 * @} end of ControlColorButton
 */
#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif /* _MGUI_NCSCTRL_CLRBTN_H */
#endif		//_MGNCSCTRL_COLORBUTTON

