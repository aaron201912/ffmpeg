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

#ifndef _MGUI_NCSCTRL_PAIRPIECE_H
#define _MGUI_NCSCTRL_PAIRPIECE_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _mPairPieceClass mPairPieceClass;
typedef struct _mPairPiece mPairPiece;

#define mPairPieceClassHeader(clss, superCls) \
	mLayoutPieceClassHeader(clss, superCls)

struct _mPairPieceClass
{
	mPairPieceClassHeader(mPairPiece, mLayoutPiece)
};

MGNCS_EXPORT extern mPairPieceClass g_stmPairPieceCls;

#define mPairPieceHeader(clss) \
	mLayoutPieceHeader(clss)  \
	DWORD flags; \
	mHotPiece *first; \
	mHotPiece *second;

struct _mPairPiece
{
	mPairPieceHeader(mPairPiece)
};

#define NCS_PAIRPIECE_DIRECTION_MASK 0x80000000
#define NCS_PAIRPIECE_FIRST_MASK     0x40000000
#define NCS_PAIRPIECE_SIZE_TYPE_MASK 0x30000000
#define NCS_PAIRPIECE_SIZE_MASK      0x00000FFF
#define NCS_PAIRPIECE_SPACE_MASK     0x0FF00000
#define NCS_PAIRPIECE_MARGIN_MASK    0x000FF000

#define mPairPiece_setVert(self)   ((self)->flags |= NCS_PAIRPIECE_DIRECTION_MASK)
#define mPairPiece_setHorz(self)   ((self)->flags &= ~NCS_PAIRPIECE_DIRECTION_MASK)
#define mPairPiece_isVert(self)    ((self)->flags & NCS_PAIRPIECE_DIRECTION_MASK)

#define mPairPiece_setReverse(self)      ((self)->flags |= NCS_PAIRPIECE_FIRST_MASK)
#define mPairPiece_cancelReverse(self)   ((self)->flags &= ~NCS_PAIRPIECE_FIRST_MASK)
#define mPairPiece_reversed(self)        ((self)->flags & NCS_PAIRPIECE_FIRST_MASK)
//#define mPairPiece_getFirst(self)   (((self)->flags&NCS_PAIRPIECE_FIRST_MASK)?self->second:self->first)
#define mPairPiece_getFirst(self)   ((self)->first)
//#define mPairPiece_getSecond(self)   (((self)->flags&NCS_PAIRPIECE_FIRST_MASK)?self->first:self->second)
#define mPairPiece_getSecond(self)  ((self)->second)
#define mPairPiece_setFirst(self, piece)   do{ \
	if(((self)->flags&NCS_PAIRPIECE_FIRST_MASK)) \
		(self)->second=(piece); \
	else \
		(self)->first=(piece); \
}while(0)

#define mPairPiece_setSecond(self, piece)   do{ \
	if(((self)->flags&NCS_PAIRPIECE_FIRST_MASK)) \
		(self)->first=(piece); \
	else \
		(self)->second=(piece); \
}while(0)



enum mPairPieceSizeType{
	NCS_PAIRPIECE_ST_FIXED = NCS_LAYOUTPIECE_ST_FIXED,
	NCS_PAIRPIECE_ST_PERCENT = NCS_LAYOUTPIECE_ST_PERCENT,
	NCS_PAIRPIECE_ST_AUTO = NCS_LAYOUTPIECE_ST_AUTO
};

#define SET_BIT_VALUE(dword, value, mask,offset)  ((dword) = ((dword)& (~(mask))) | (((value)<<(offset))&mask))
#define GET_BIT_VALUE(dword, mask, offset)   (((dword)&(mask))>>(offset))

#define mPairPiece_setFirstSizeType(self, type)  SET_BIT_VALUE(self->flags, type, NCS_PAIRPIECE_SIZE_TYPE_MASK,28)
#define mPairPiece_getFirstSizeType(self)        GET_BIT_VALUE(self->flags, NCS_PAIRPIECE_SIZE_TYPE_MASK, 28)
#define mPairPiece_setFirstSize(self, size)      (self->flags = (self->flags&(~NCS_PAIRPIECE_SIZE_MASK))|((size)&NCS_PAIRPIECE_SIZE_MASK))
#define mPairPiece_getFirstSize(self)            (int)(self->flags&NCS_PAIRPIECE_SIZE_MASK)

#define mPairPiece_setSpace(self, space)         SET_BIT_VALUE(self->flags, space, NCS_PAIRPIECE_SPACE_MASK, 20)
#define mPairPiece_getSpace(self)                (int)GET_BIT_VALUE(self->flags, NCS_PAIRPIECE_SPACE_MASK, 20)

#define mPairPiece_setMargin(self, margin)       SET_BIT_VALUE(self->flags, margin, NCS_PAIRPIECE_MARGIN_MASK, 12)
#define mPairPiece_getMargin(self)               (int)GET_BIT_VALUE(self->flags, NCS_PAIRPIECE_MARGIN_MASK, 12)

enum mPairPieceProps{
	NCSP_PAIRPIECE_SECOND_AS_FIRST = PAIRPIECE_PROP_BEGIN,
	NCSP_PAIRPIECE_FIRST_SIZE_TYPE,
	NCSP_PAIRPIECE_FIRST_SIZE,
	NCSP_PAIRPIECE_FIRST,
	NCSP_PAIRPIECE_SECOND,
	NCSP_PAIRPIECE_DIRECTION = PIECECOMM_PROP_DIRECTION,
	NCSP_PAIRPIECE_SPACE     = PIECECOMM_PROP_SPACE,
	NCSP_PAIRPIECE_MARGIN    = PIECECOMM_PROP_MARGIN
};

#ifdef __cplusplus
}
#endif

#endif

