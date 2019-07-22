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

#ifndef  MABSTRACTITEMMANAGER_INC
#define  MABSTRACTITEMMANAGER_INC

#ifdef __cplusplus
extern "C" {
#endif

/* typedef struct _mAbstractItemManagerClass mAbstractItemManagerClass;
 * typedef struct _mAbstractItemManager mAbstractItemManager;
 */

#define mAbstractItemManagerHeader(clss) \
    mObjectHeader(clss)

#define mAbstractItemManagerClassHeader(clss, superCls) \
    mObjectClassHeader(clss, superCls)  \
    int  (*addItem)(clss*, mPieceItem*);  \
    void (*removeItem)(clss*, mPieceItem*);  \
    void (*clear)(clss*);   \
    mItemIterator* (*createItemIterator)(clss*);

struct _mAbstractItemManager
{   
    mAbstractItemManagerHeader(mAbstractItemManager)
};

struct _mAbstractItemManagerClass
{   
    mAbstractItemManagerClassHeader(mAbstractItemManager, mObject)
};

MGNCS_EXPORT extern mAbstractItemManagerClass g_stmAbstractItemManagerCls;

#ifdef __cplusplus
}
#endif

#endif   /* ----- #ifndef MABSTRACTITEMMANAGER_INC  ----- */
