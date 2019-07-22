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

#ifndef  MITEMITERATOR_INC
#define  MITEMITERATOR_INC

#include "mpieceitem.h"

#ifdef __cplusplus
extern "C" {
#endif

/* prefix declare. */
struct _mAbstractItemManager;
typedef struct _mAbstractItemManager mAbstractItemManager;

struct _mAbstractItemManagerClass;
typedef struct _mAbstractItemManagerClass mAbstractItemManagerClass;

typedef struct _mItemIteratorClass mItemIteratorClass;
typedef struct _mItemIterator mItemIterator;

#define mItemIteratorHeader(clss) \
    mObjectHeader(clss) \
    mAbstractItemManager* manager;

#define mItemIteratorClassHeader(clss, superCls) \
    mObjectClassHeader(clss, superCls)  \
    mPieceItem* (*next)(clss*); \
    mPieceItem* (*prev)(clss*); \
    mItemIterator* (*duplicate)(clss*);

struct _mItemIterator
{   
    mItemIteratorHeader(mItemIterator)
};

struct _mItemIteratorClass
{   
    mItemIteratorClassHeader(mItemIterator, mObject)
};

MGNCS_EXPORT extern mItemIteratorClass g_stmItemIteratorCls;

#ifdef __cplusplus
}
#endif

#endif   /* ----- #ifndef MITEMITERATOR_INC  ----- */

