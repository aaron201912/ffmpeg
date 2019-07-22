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

#ifndef  MPIECEITEM_INC
#define  MPIECEITEM_INC

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _mNormalVerctor
{
    float angle;
    int x;
    int y;
    int z;
} mNormalVector;

typedef struct _mPieceItemClass mPieceItemClass;
typedef struct _mPieceItem mPieceItem;

#define mPieceItemHeader(clss) \
    mObjectHeader(clss) \
    list_t list;    \
    int x;  \
    int y;  \
    mHotPiece *piece; \
    float wscalefactor; \
    float hscalefactor; \
    int alpha; \
    mNormalVector normalVector; \
    HDC cacheDC; \
    BOOL isEnableCache; \
    BOOL underLayout;\
    int type;

#define mPieceItemClassHeader(clss, superCls) \
    mObjectClassHeader(clss, superCls)  \
    void (*setPiece)(clss*, mHotPiece*); \
    mHotPiece* (*getPiece)(clss*); \
    void (*setX)(clss*, int);    \
    void (*setY)(clss*, int);    \
    int  (*getX)(clss*);    \
    int  (*getY)(clss*);    \
    int  (*getType)(clss*);    \
    void (*setType)(clss*, int);

struct _mPieceItem
{   
    mPieceItemHeader(mPieceItem)
};

struct _mPieceItemClass
{   
    mPieceItemClassHeader(mPieceItem, mObject)
};

MGNCS_EXPORT extern mPieceItemClass g_stmPieceItemCls;

#ifdef __cplusplus
}
#endif

#endif   /* ----- #ifndef MPIECEITEM_INC  ----- */

