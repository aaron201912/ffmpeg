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

#ifndef _MG_MINIPOPMENUMGR_H
#define _MG_MINIPOPMENUMGR_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \defgroup PopMenuManager mPopMenuMgr
 * @{
 */

typedef struct _mPopMenuMgrClass mPopMenuMgrClass;
typedef struct _mPopMenuMgr mPopMenuMgr;

#define mPopMenuMgrClassHeader(clss, clsSuper) \
	mObjectClassHeader(clss, clsSuper) \
	BOOL (*addItem)(clss *, UINT type, const char* str, PBITMAP bmp, int id, UINT state,  mPopMenuMgr *subMenu, DWORD add_data); \
	HMENU (*createMenu)(clss *); \
	void (*popMenu)(clss *, mObject *owner); \
	BOOL (*addSeparator)(clss *); \
	BOOL (*getMenuItem)(clss *, int idx, MENUITEMINFO *pmii, BOOL byCommand);

/**
 * \struct mPopMenuMgrClass
 * \brief mPopMenuMgr class, derived from \ref mObject
 *
 * - BOOL \b addItem(mPopMenuMgr *self, int type, const char *str, \
 *   				 PBITMAP bmp, int id, int state, \
 *   				 mPopMenuMgr *subMenu, DWORD add_data); \n
 *   add a menu item
 *   \param type - menu item type, same as MiniGUI \b MENUITEMINFO :
 *   		- MTF_STRING      a menu item with string only
 *   		- MTF_BITMAP      a menu item with bitmap only
 *   		- MTF_BMPSTRING   a menu item with string and bitmap
 *   		- MTF_RADIOCHECK  a menu item with a radio button
 *   		- MTF_MARKCHECK   a menu item with a check button
 *   		- MTF_SEPARATOR   a seprator item
 *   \param str - the string of menu item, when type is MTF_STRING or MTF_BMPSTRING
 *   \param bmp - the bitmap pointer of menu item, when type is MTF_BITMAP or MTF_BMPSTRING
 *   \param id - the id of menu item
 *   \param state - the state of menu item, same as MiniGIUI \b MENUITEMINFO, by default it is MFS_ENABLED
 *   		- MFS_GRAYED     the menu item is disabled
 *   		- MFS_DISABLED   the menu item is disabled
 *   		- MFS_CHECKED    the menu item is checked
 *   		- MFS_ENABLED    the menu item is enabled , default value
 *   		- MFS_UNCHECKED  the menu item is unchecked
 *   \param subMenu - the sub menu of this menu item, by default is NULL
 *   \param add_data - the additional data of menu item, by default is 0
 *   \return TRUE - sucessed, FALSE - failed
 *
 * - HMENU \b createMenu(mPopMenuMgr *self); \n
 *   create a pop menu handle from itself
 *   \return the pop menu handle
 *
 * - void \b popMenu(mPopMenuMgr *self, mObject *owner); \n
 *   pop itself
 *   \param owner - it should be a \b mWidget pointer, indicate the owner who want a pop menu
 * 
 * - BOOL \b addSeparator(mPopMenuMgr *self);\n
 *   add a seprator for menu item
 *   \return TRUE -sucessed; FALSE -failed
 *
 * - BOOL \b getMenuItem(mPopMenuMgr *self, int idx, MENUITEMINFO *pmii, BOOL byCommand);\n
 *   store a menu item info into MENUITEMINFO
 *   \param idx - the index(byCommand==FALSE) or id of menu item
 *   \param pmii - Out param, store the menu item info
 *   \param byCommand - indiate the param idx is the index or id of menu item. TRUE for id, 
 *                      FALSE for index.
 *   \return TRUE -sucessed, FALSE - failed
 *
 * \sa mPopMenuMgr
 */

struct _mPopMenuMgrClass {
	mPopMenuMgrClassHeader(mPopMenuMgr, mObject)
};

MGNCS_EXPORT extern mPopMenuMgrClass g_stmPopMenuMgrCls;


typedef struct mPopMenuItem mPopMenuItem;

#define mPopMenuMgrHeader(clss) \
	mObjectHeader(clss) \
	mPopMenuItem *head;

/**
 * \struct mPopMenuMgr
 * \brief the members of mPropMenuMgr
 *
 */

struct _mPopMenuMgr {
	mPopMenuMgrHeader(mPopMenuMgr)
};

/**
 * @} end of PopMenuManager
 */

#ifdef __cplusplus
}
#endif

#endif

