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

#ifndef _MGUI_NCSCTRL_BOXLAYOUTPIECE_H
#define _MGUI_NCSCTRL_BOXLAYOUTPIECE_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _mBoxLayoutPieceClass mBoxLayoutPieceClass;
typedef struct _mBoxLayoutPiece mBoxLayoutPiece;

#define mBoxLayoutPieceClassHeader(clss, superCls) \
	mLayoutPieceClassHeader(clss, superCls) \
/** procted **/ \
	void (*recalcBox)(clss *); \
	int  (*getCellAutoSize)(clss *, int i); \
/** public **/ \
	int (*setCellInfo)(clss *, int idx, int size, int type/*=NCS_BOXLAYOUTPIECE_ST_FIXED*/, BOOL bupdate/*=FALSE*/); \
	mHotPiece* (*setCell)(clss *, int idx, mHotPiece *cell); \
	mHotPiece* (*getCell)(clss *, int idx);

struct _mBoxLayoutPieceClass
{
	mBoxLayoutPieceClassHeader(mBoxLayoutPiece, mLayoutPiece)
};

MGNCS_EXPORT extern mBoxLayoutPieceClass g_stmBoxLayoutPieceCls;

#define mBoxLayoutPieceHeader(clss) \
	mLayoutPieceHeader(clss)  \
	int left:16; \
	int top:16; \
	int right:16; \
	int bottom:16; \
	unsigned short count; \
	unsigned char  margin; \
	unsigned char  space; \
	unsigned short * cell_info; \
	mHotPiece ** cells;

	
struct _mBoxLayoutPiece
{
	mBoxLayoutPieceHeader(mBoxLayoutPiece)
};

#define mBoxLayoutPiece_getSizeType(size)  mLayoutPiece_getSizeType(size)
#define mBoxLayoutPiece_setSizeType(size, type) mLayoutPiece_setSizeType(size, type)
#define mBoxLayoutPiece_setSize(size, value)    mLayoutPiece_setSize(size, value)
#define mBoxLayoutPiece_getSize(size) mLayoutPiece_getSize(size)


enum mBoxLayoutPieceProps{
	NCSP_BOXLAYOUTPIECE_COUNT = BOXLAYOUTPIECE_PROP_BEGIN,
	NCSP_BOXLAYOUTPIECE_SPACE     = PIECECOMM_PROP_SPACE,
	NCSP_BOXLAYOUTPIECE_MARGIN    = PIECECOMM_PROP_MARGIN
};

/** protected functio */

BOOL mBoxLayoutPiece_calcSize(mBoxLayoutPiece *self, int total_size, int *sizes);

#ifdef __cplusplus
}
#endif

#endif

