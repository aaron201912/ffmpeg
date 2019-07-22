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

#ifndef PIECE_ID_H
#define PIECE_ID_H

/**
 * system piece property ids range
 */
enum SysPiecePropsRange{
	SYS_PIECE_PROP_BEGIN = 1,
	PIECECOMM_PROP_BEGIN = SYS_PIECE_PROP_BEGIN,
	PIECECOMM_PROP_ALIGN,
	PIECECOMM_PROP_VALIGN,
	PIECECOMM_PROP_AUTOWRAP,
	PIECECOMM_PROP_WORDBREAK,
	PIECECOMM_PROP_DRAWMOD,
	PIECECOMM_PROP_DIRECTION,
	PIECECOMM_PROP_SPACE,
	PIECECOMM_PROP_MARGIN,
	PIECECOMM_PROP_MAX,
	PIECECOMM_PROP_POS,
	PIECECOMM_PROP_MIN,
	PIECECOMM_PROP_LINESTEP,
	PIECECOMM_PROP_PAGESTEP,
	PIECECOMM_PROP_SELECT,
	PIECECOMM_PROP_END = PIECECOMM_PROP_BEGIN + 256,

	HOTPIECE_PROP_BEGIN,
	HOTPIECE_PROP_END,


	LABELPIECE_PROP_BEGIN,
	LABELPIECE_PROP_END = LABELPIECE_PROP_BEGIN + 10,

	LEDLBLPIECE_PROP_BEGIN,
	LEDLBLPIECE_PROP_END = LEDLBLPIECE_PROP_BEGIN + 10,

	ANIMATEPIECE_PROP_BEGIN,
	ANIMATEPIECE_PROP_END = ANIMATEPIECE_PROP_BEGIN + 10,

	IMAGEPIECE_PROP_BEGIN,
	IMAGEPIECE_PROP_END = IMAGEPIECE_PROP_BEGIN + 10,

	IMAGELABELPIECE_PROP_BEGIN,
	IMAGELABELPIECE_PROP_END = IMAGELABELPIECE_PROP_BEGIN + 10,

	ABP_PROP_BEGIN,
	ABP_PROP_END = ABP_PROP_BEGIN + 10,

	PAIRPIECE_PROP_BEGIN,
	PAIRPIECE_PROP_END = PAIRPIECE_PROP_BEGIN + 10,

	SLIDERPIECE_PROP_BEGIN,
	SLIDERPIECE_PROP_END = SLIDERPIECE_PROP_BEGIN + 10,

	BOXLAYOUTPIECE_PROP_BEGIN,
	BOXLAYOUTPIECE_PROP_END = BOXLAYOUTPIECE_PROP_BEGIN + 10,

	SPINBOXPIECE_PROP_BEGIN,
	SPINBOXPIECE_PROP_END = SPINBOXPIECE_PROP_BEGIN + 10,

	GRIDBOXPIECE_PROP_BEGIN,
	GRIDBOXPIECE_PROP_END = GRIDBOXPIECE_PROP_BEGIN + 10,

	DAYGRIDPIECE_PROP_BEGIN,
	DAYGRIDPIECE_PROP_END = DAYGRIDPIECE_PROP_BEGIN + 10,

	CHECKBUTTONPIECE_PROP_BEGIN,
	CHECKBUTTONPIECE_PROP_END = CHECKBUTTONPIECE_PROP_BEGIN + 10,

	WEEKHEADPIECE_PROP_BEGIN,
	WEEKHEADPIECE_PROP_END = WEEKHEADPIECE_PROP_BEGIN + 10,

	MONTHPIECE_PROP_BEGIN,
	MONTHPIECE_PROP_END = MONTHPIECE_PROP_BEGIN + 10,

	MLISTSPINNEDPIECE_PROP_BEGIN,
	MLISTSPINNEDPIECE_PROP_END = MLISTSPINNEDPIECE_PROP_BEGIN + 10,

	TOOLIMAGEPIECE_PROP_BEGIN,
	TOOLIMAGEPIECE_PROP_END = TOOLIMAGEPIECE_PROP_BEGIN + 10,

	LINELAYOUTPIECE_PROP_BEGIN,
	LINELAYOUTPIECE_PROP_END = LINELAYOUTPIECE_PROP_BEGIN + 10,

	PROGRESSPIECE_PROP_BEGIN,
	PROGRESSPIECE_PROP_END = PROGRESSPIECE_PROP_BEGIN + 10,

	RECTPIECE_PROP_BEGIN,
	RECTPIECE_PROP_END = RECTPIECE_PROP_BEGIN + 10,

	SIMPLELISTPIECE_PROP_BEGIN,
	SIMPLELISTPIECE_PROP_END = SIMPLELISTPIECE_PROP_BEGIN + 10,

	SYS_PIECE_PROP_END = 10000,
	USER_PIECE_PROP_BEGIN
};

/**
 * system peices event ids range
 */
enum mSysPieceEventRange
{
	SYS_PIECE_EVENT_BEGIN = 1,

	ABP_EVENT_BEGIN,

	ABP_EVENT_END = ABP_EVENT_BEGIN + 10,

	SLIDERPIECE_EVENT_BEGIN,
	SLIDERPIECE_EVENT_END = SLIDERPIECE_EVENT_BEGIN + 10,

	SPINBOXPIECE_EVENT_BEGIN,
	SPINBOXPIECE_EVENT_END = SPINBOXPIECE_EVENT_BEGIN + 10,


	MONTHPIECE_EVENT_BEGIN,
	MONTHPIECE_EVENT_END = MONTHPIECE_EVENT_BEGIN + 10,


	SIMPLELISTPIECE_EVENT_BEGIN,
	SIMPLELISTPIECE_EVENT_END = SIMPLELISTPIECE_EVENT_BEGIN + 10,

	SYS_PIECE_EVENT_END = 10000,

	USER_PIECE_EVENT_BEGIN,
};

/**
 * define the common state
 */

enum PieceState{
	PIECE_STATE_NORMAL = 0,
	PIECE_STATE_HILIGHT,
	PIECE_STATE_PUSHED,
	PIECE_STATE_CAPTURED,
	PIECE_STATE_DISABLE
};

enum PieceCheckState{
	PIECE_CHECKSTATE_UNCHECKED = 0,
	PIECE_CHECKSTATE_HALFCHECKED,
	PIECE_CHECKSTATE_CHECKED
};

/**
 * define the common draw flags
 *
 */

/**
 * button or button like pieces common flags
 */
#define NCS_PIECE_PAINT_STATE_MASK       0x000F
#define NCS_PIECE_PAINT_CHECK_STATE_MASK 0x00F0
#define NCS_PIECE_PAINT_GET_CHECK(add_data)      (((add_data)&NCS_PIECE_PAINT_CHECK_STATE_MASK)>>4)
#define NCS_PIECE_PAINT_MAKE_CHECK(value)     \
		(((value)&0xF)<<4)

/**
 * Arrow flags, use with button flags
 */
#define NCS_PIECE_PAINT_ARROW_MASK      0x300
#define NCS_PIECE_PAINT_ARROW_SHIFT     8
#define NCS_PIECE_PAINT_ARROW_SHELL     0x400
enum mArrowPieceType {
	NCS_ARROWPIECE_LEFT = 0x00,
	NCS_ARROWPIECE_RIGHT = 0x100,
	NCS_ARROWPIECE_UP = 0x200,
	NCS_ARROWPIECE_DOWN = 0x300
};
/**
 * button flags
 */
#define NCS_PIECE_PAINT_FLAT            0x010000 //flat style


/**
 * layout flags size type
 */
enum mLayoutPieceSizeType {
	NCS_LAYOUTPIECE_ST_FIXED = 0,
	NCS_LAYOUTPIECE_ST_PERCENT,
	NCS_LAYOUTPIECE_ST_AUTO //use the piece size of iteself
};

#define mLayoutPiece_getSizeType(size)  (((size)&0xC000)>>14)
#define mLayoutPiece_setSizeType(size, type) ((size)=((size)&0x3FFF)|((type)<<14))
#define mLayoutPiece_setSize(size, value)    ((size)=((size)&0xC000)|((value)&0x3FFF))
#define mLayoutPiece_getSize(size)           ((size)&0x3FFF)


/**
 * slider common flags for paint
 */
#define NCS_PIECE_PAINT_VERT             0x00010000
#define NCS_PIECE_PAINT_TICK             0x00020000
//progress
#define NCS_PIECE_PAINT_BLOCK            0x00040000
#define NCS_PIECE_PAINT_PERCENT          0x00080000

#endif

