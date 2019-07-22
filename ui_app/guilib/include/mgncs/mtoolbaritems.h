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

#ifndef MGNCS_TOOLBAR_ITEMS_H
#define MGNCS_TOOLBAR_ITEMS_H

//////////////////////////////////////////////////////
/**
 *  mPushToolItem
 *
 */
typedef struct _mPushToolItemClass mPushToolItemClass;
typedef struct _mPushToolItem mPushToolItem;

typedef struct mPushToolItemCreateInfo{
	mToolImage * toolImg;
	const char * str;
	UINT         flags;
}mPushToolItemCreateInfo;

#define mPushToolItemClassHeader(clss, clssSuper) \
	mAbstractButtonPieceClassHeader(clss, clssSuper) \
	mHotPiece * (*createContent)(clss*, mPushToolItemCreateInfo* create_info); \
	BOOL (*setCheck)(clss *, int state); \
	int  (*getCheck)(clss *);

struct _mPushToolItemClass
{
	mPushToolItemClassHeader(mPushToolItem, mAbstractButtonPiece)
};

#define mPushToolItemHeader(clss) \
	mAbstractButtonPieceHeader(clss) 

struct _mPushToolItem
{
	mPushToolItemHeader(mPushToolItem)
};

MGNCS_EXPORT extern mPushToolItemClass g_stmPushToolItemCls;


/**
 *  mMenuToolItem
 */
typedef struct _mMenuToolItemClass mMenuToolItemClass;
typedef struct _mMenuToolItem mMenuToolItem;


#define mMenuToolItemClassHeader(clss, clssSuper) \
	mPushToolItemClassHeader(clss, clssSuper) \
	BOOL (*showMenu)(clss *, mWidget *owner);

struct _mMenuToolItemClass
{
	mMenuToolItemClassHeader(mMenuToolItem, mPushToolItem)
};

#define mMenuToolItemHeader(clss) \
	mPushToolItemHeader(clss) \
	mPopMenuMgr * popmenu;

struct _mMenuToolItem
{
	mMenuToolItemHeader(mMenuToolItem)
};

MGNCS_EXPORT extern mMenuToolItemClass g_stmMenuToolItemCls;



#endif


