/*
 * \file mnsdrawpiece.h
 * \author miniStudio team of FMSoft
 * \date 2010/10/09
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
#ifndef __MGNCS4TOUCH_NSDRAWPIECE_H__
#define __MGNCS4TOUCH_NSDRAWPIECE_H__

#ifdef __cplusplus
extern "C" {
#endif

#define SetNSDrawPieceRect(self, prc) do {            \
	(self)->left   = (short)((prc->left)   & 0xFFFF); \
	(self)->top    = (short)((prc->top)    & 0xFFFF); \
	(self)->right  = (short)((prc->right)  & 0xFFFF); \
	(self)->bottom = (short)((prc->bottom) & 0xFFFF); \
} while (0)

typedef struct _mNSDrawPiece      mNSDrawPiece;
typedef struct _mNSDrawPieceClass mNSDrawPieceClass;

#define mNSDrawPieceHeader(clss) \
	mHotPieceHeader(clss)        \
	DWORD shape;                 \
	DWORD dotHit;                \
	int left:16;                 \
	int top:16;                  \
	int right:16;                \
	int bottom:16;

struct _mNSDrawPiece {
	mNSDrawPieceHeader(mNSDrawPiece)
};

#define mNSDrawPieceClassHeader(clss, superCls) \
	mHotPieceClassHeader(clss, superCls)

struct _mNSDrawPieceClass {
	mNSDrawPieceClassHeader(mNSDrawPiece, mHotPiece)
};

MGNCS_EXPORT extern mNSDrawPieceClass g_stmNSDrawPieceCls;

enum mNSDrawPieceProp {
	NCSP_NSDRAW_PIECE_TICK = USER_PIECE_PROP_BEGIN + 1,
	NCSP_NSDRAW_PIECE_ANGLE,
	NCSP_NSDRAW_PIECE_DOTHIT,
	NCSP_NSDRAW_PIECE_MAX,
};

enum mNSDrawPieceShape {
	NSDRAW_SHAPE_TICK = NCSP_NSDRAW_PIECE_MAX + 1,
	NSDRAW_SHAPE_ANGLE,
};

#ifdef __cplusplus
}
#endif

#endif	// __MGNCS4TOUCH_NSDRAWPIECE_H__

