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

#ifdef _MGNCSCTRL_MENUBUTTON

#ifndef _MGUI_NCSCTRL_MNUBTN_H
#define _MGUI_NCSCTRL_MNUBTN_H


#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

/**
 * \defgroup ControlMenuButton mMenuButton
 * @{
 */

/**
 * \def NCSCTRL_MENUBUTTON
 * \brief the name of menubutton class
 */

#define NCSCTRL_MENUBUTTON        NCSCLASSNAME("menubutton")

typedef struct _mMenuButton mMenuButton;
typedef struct _mMenuButtonClass mMenuButtonClass;
typedef struct _mMenuButtonRenderer mMenuButtonRenderer;

#define mMenuButtonHeader(className) \
	mButtonHeader(className)  \
	mPopMenuMgr * popmenu; \
	int cur_item;

/**
 * \struct mMenuButton
 * \brief MenuButton member defintion, derived from \ref mButton
 *
 * - popmenu the popmenu pointer used by MenuButton, \sa mPopMenuMgr
 *
 * - cur_item the current item of MenuButton
 */

struct _mMenuButton
{
	mMenuButtonHeader(mMenuButton)
};

#define mMenuButtonClassHeader(className, parentClass) \
	mButtonClassHeader(className, parentClass)

/**
 * \struct mMenuButtonClass
 * \brief MenuButton class , derived from mButtonClass
 */

struct _mMenuButtonClass
{
	mMenuButtonClassHeader(mMenuButton, mButton)
};

#define mMenuButtonRendererHeader(clsName, parentClass) \
	mButtonRendererHeader(clsName, parentClass)

/**
 * \struct mMenuButtonRenderer
 * \brief MenuButton class renderer interface, derived from \ref mButtonRenderer
 *
 *  \sa mMenuButton, mMenuButtonClass, mButton, mButtonClass
 *
 */

/* define the render of image static */
struct _mMenuButtonRenderer {
	mMenuButtonRendererHeader(mMenuButton, mButton)
};

/**
 * \var g_stmMenuButtonCls
 * \brief global mMenuButtonClass
 */
MGNCS_EXPORT extern mMenuButtonClass g_stmMenuButtonCls;

/**
 * \enum mMenuButtonProp
 * \brief the properties id of \ref mMenuButton, derived from mButtonProp
 */
enum mMenuButtonProp{
	/**
	 * \brief Set the PopMenu, the Value Must be mPopMenuMgr pointer
	 *  - Type mPopMenuMgr*
	 *  - Read Write
	 *  - set the PopMenu pointer
	 *
	 */
	NCSP_MNUBTN_POPMENU = NCSP_BUTTON_MAX + 1,
	/**
	 * \brief Set the current menu item id
	 *  - Type int
	 *  - Read Write
	 *  - set/get the current menu item id
	 *
	 */
	NCSP_MNUBTN_CURITEM,
	NCSP_MNUBTN_MAX
};

/**
 * \enum eMenuButtonNotify
 * \brief the notification id of \ref mMenuButton
 */

enum eMenuButtonNotify{
	/**
	 * \brief Event raised when MenuButton's item changed
	 * \param the new menu item id
	 *
	 */
	NCSN_MNUBTN_ITEMCHANGED = NCSN_BUTTON_MAX + 1,
	NCSN_MNUBTN_MAX
};

#define NCSS_MNUBTN_SHIFT NCSS_BUTTON_SHIFT


/**
 * @} end of ControlMenuButton
 */
#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif /* _MGUI_NCSCTRL_RECTANGLE_H */
#endif //_MGNCSCTRL_MENUBUTTON
