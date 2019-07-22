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

#ifndef  MGROUPITEMMANAGER_INC
#define  MGROUPITEMMANAGER_INC

#ifdef __cplusplus
extern "C" {
#endif

typedef int (*ITEM_CMP)(const char* s1, const char* s2);
typedef struct _mGroupItemManagerClass mGroupItemManagerClass;
typedef struct _mGroupItemManager mGroupItemManager;

#define mGroupItemManagerHeader(clss) \
    mAbstractItemManagerHeader(clss) \
    list_t itemList; \
    ITEM_CMP itemCmp;

#define mGroupItemManagerClassHeader(clss, superCls) \
    mAbstractItemManagerClassHeader(clss, superCls) \
    BOOL (*addGroupItem)(clss*,  mGroupPieceItem*, BOOL); \
    BOOL (*removeGroupItem)(clss*, const char*, BOOL); \
    mGroupPieceItem* (*searchGroupItem)(clss*, const char*, BOOL); \
    mGroupPieceItem* (*searchGreaterGroupItem)(clss*, const char*, BOOL); \
    mItemIterator* (*createGroupItemIterator)(clss*, const char*);

struct _mGroupItemManager
{   
    mGroupItemManagerHeader(mGroupItemManager)
};

struct _mGroupItemManagerClass
{   
    mGroupItemManagerClassHeader(mGroupItemManager, mAbstractItemManager)
};

MGNCS_EXPORT extern mGroupItemManagerClass g_stmGroupItemManagerCls;

#ifdef __cplusplus
}
#endif


#endif   /* ----- #ifndef MGROUPITEMMANAGER_INC  ----- */

