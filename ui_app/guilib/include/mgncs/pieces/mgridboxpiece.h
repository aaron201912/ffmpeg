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

#ifndef _MGUI_NCSCTRL_GRIDBOXPIECE_H
#define _MGUI_NCSCTRL_GRIDBOXPIECE_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _mGridBoxPieceClass mGridBoxPieceClass;
typedef struct _mGridBoxPiece mGridBoxPiece;

#define CELL_POINT_ISNULL(cp)   ((cp).row == 0xFFFF || (cp).col == 0xFFFF)
#define NULL_CELL_POINT  { 0xFFFF, 0xFFFF}
typedef struct _CELL_POINT{
	unsigned short row;
	unsigned short col;
}CELL_POINT;

#define mGridBoxPieceClassHeader(clss, superCls) \
	mRenderablePieceClassHeader(clss, superCls)  \
	CELL_POINT (* hitCell)(clss *, int x, int y);  \
	RECT * (*getCellRect)(clss *, int row, int col, RECT *prc); \
	void (*updateNewCell)(clss *, CELL_POINT old_cell, CELL_POINT new_cell, int old_mask, int new_mask, mWidget *owner);

struct _mGridBoxPieceClass
{
	mGridBoxPieceClassHeader(mGridBoxPiece, mRenderablePiece)
};

MGNCS_EXPORT extern mGridBoxPieceClass g_stmGridBoxPieceCls;

#define mGridBoxPieceHeader(clss) \
	mRenderablePieceHeader(clss)  \
	unsigned short rows; \
	unsigned short cols; 
	
struct _mGridBoxPiece
{
	mGridBoxPieceHeader(mGridBoxPiece)
};


#define NCS_GRIDBOXPIECE_PAINT_MASK_DRAWSEL 0x01
#define NCS_GRIDBOXPIECE_PAINT_MASK_HOT     0x02
#define NCS_GRIDBOXPIECE_PAINT_MASK_REDRAW  0x04
#define NCS_GRIDBOXPIECE_PAINT_MASK_ALL_CELLS 0x10
typedef struct mGridBoxPiecePaintData{
	DWORD mask;
	CELL_POINT  hot_cell;
	CELL_POINT  sel_cell;
	DWORD add_data;
}mGridBoxPiecePaintData;

enum mGridBoxPieceProps{
	NCSP_GRIDBOXPIECE_ROWS = GRIDBOXPIECE_PROP_BEGIN,
	NCSP_GRIDBOXPIECE_COLS
};

#define mGridBoxPiece_makeCell(col, row)  (DWORD)(((col)&0xFF)|(((row)&0xFF)<<8))
#define mGridBoxPiece_getRow(cell)   (((cell)&0xFF00)>>8)
#define mGridBoxPiece_getCol(cell)   ((cell)&0xFF)


#ifdef __cplusplus
}
#endif

#endif

