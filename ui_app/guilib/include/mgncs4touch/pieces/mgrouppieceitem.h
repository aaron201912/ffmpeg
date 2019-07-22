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

#ifndef  MGROUPPIECEITEM_INC
#define  MGROUPPIECEITEM_INC

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _mGroupPieceItemClass mGroupPieceItemClass;
typedef struct _mGroupPieceItem mGroupPieceItem;

#define mGroupPieceItemHeader(clss) \
    mPieceItemHeader(clss) \
    list_t child; \
    char* key;

#define mGroupPieceItemClassHeader(clss, superCls) \
    mPieceItemClassHeader(clss, superCls)  \
    void (*setKey)(clss*, const char*); \
    char* (*getKey)(clss*);

struct _mGroupPieceItem
{   
    mGroupPieceItemHeader(mGroupPieceItem)
};

struct _mGroupPieceItemClass
{   
    mGroupPieceItemClassHeader(mGroupPieceItem, mPieceItem)
};

MGNCS_EXPORT extern mGroupPieceItemClass g_stmGroupPieceItemCls;

#ifdef __cplusplus
}
#endif

#endif   /* ----- #ifndef MGROUPPIECEITEM_INC  ----- */

