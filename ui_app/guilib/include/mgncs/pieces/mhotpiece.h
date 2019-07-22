/*
 *   This file is part of mGNCS, a component for MiniGUI.
 * 
 *   Copyright (C) 2008~2018, Beijing FMSoft Technologies Co., Ltd.
 * 
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 * 
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 * 
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * 
 *   Or,
 * 
 *   As this program is a library, any link to this program must follow
 *   GNU General Public License version 3 (GPLv3). If you cannot accept
 *   GPLv3, you need to be licensed from FMSoft.
 * 
 *   If you have got a commercial license of this program, please use it
 *   under the terms and conditions of the commercial license.
 * 
 *   For more information about the commercial license, please refer to
 *   <http://www.minigui.com/en/about/licensing-policy/>.
 */

#ifndef _MGNCS_HOT_PIECEC_H
#define _MGNCS_HOT_PIECEC_H

#ifdef __cplusplus
extern "C" {
#endif

#include "piece-id.h"

typedef struct _mHotPieceClass mHotPieceClass;
typedef struct _mHotPiece mHotPiece;

#define mHotPieceClassHeader(clss, superCls) \
	mObjectClassHeader(clss, superCls) \
	mHotPiece* (*hitTest)(clss *,  int x, int y); \
	void (*paint)(clss *, HDC hdc, mObject * owner, DWORD add_data); \
	LRESULT (*processMessage)(clss *, UINT message, WPARAM, LPARAM, mObject *owner); \
	BOOL (*setProperty)(clss *, int id, DWORD value); \
	DWORD(*getProperty)(clss *, int id); \
	BOOL (*enable)(clss *, BOOL benable); \
	BOOL (*isEnabled)(clss *); \
	BOOL (*setRenderer)(clss *, const char* rdr_name); \
	BOOL (*setRect)(clss *, const RECT * prc); \
	BOOL (*getRect)(clss *, RECT * prc);  \
	BOOL (*autoSize)(clss *, mObject *owner, const SIZE *pszMin, const SIZE *pszMax);

struct _mHotPieceClass
{
	mHotPieceClassHeader(mHotPiece, mObject)
};

MGNCS_EXPORT extern mHotPieceClass g_stmHotPieceCls;

#define mHotPieceHeader(clss) \
    mObjectHeader(clss) \
    mHotPiece * parent;

struct _mHotPiece
{
	mHotPieceHeader(mHotPiece)
};

typedef void (*PHotPieceRdrFunc)(mHotPiece*, HDC hdc, mObject* owner, DWORD add_data);


#define SET_BIT(flag, mask)         ((flag)|=(mask))
#define IS_BIT_SET(flag, mask)      (((flag)&(mask)) == (mask))
#define CLEAR_BIT(flag, mask)       ((flag)&=~(mask))


MGNCS_EXPORT mHotPiece * ncsNewPiece(mHotPieceClass *_class, DWORD add_data);
#define NEWPIECEEX(Piece,add_data)   (Piece*)(ncsNewPiece((mHotPieceClass*)((void *)&(Class(Piece))),(DWORD)(add_data)))
#define NEWPIECE(Piece)  NEWPIECEEX(Piece, 0)
MGNCS_EXPORT void ncsDelPiece(mHotPiece* piece);
#define DELPIECE(p)  ncsDelPiece((mHotPiece*)p)


MGNCS_EXPORT void mHotPiece_update(mHotPiece *self, mObject *onwer, BOOL bUpdate);


#ifdef __cplusplus
}
#endif

#endif


