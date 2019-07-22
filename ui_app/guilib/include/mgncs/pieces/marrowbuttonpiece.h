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
#if defined (_MGNCSCTRL_IMWORDSEL) || defined (_MGNCSCTRL_SPINNER) || defined (_MGNCSCTRL_SCROLLBAR)

#ifndef _MGUI_NCSCTRL_ARROWBUTTONPIECE_H
#define _MGUI_NCSCTRL_ARROWBUTTONPIECE_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _mArrowButtonPieceClass mArrowButtonPieceClass;
typedef struct _mArrowButtonPiece mArrowButtonPiece;

#define mArrowButtonPieceClassHeader(clss, superCls) \
	mAbstractButtonPieceClassHeader(clss, superCls)

struct _mArrowButtonPieceClass
{
	mArrowButtonPieceClassHeader(mArrowButtonPiece, mAbstractButtonPiece)
};

MGNCS_EXPORT extern mArrowButtonPieceClass g_stmArrowButtonPieceCls;

#define mArrowButtonPieceHeader(clss) \
	mAbstractButtonPieceHeader(clss)

struct _mArrowButtonPiece
{
	mArrowButtonPieceHeader(mArrowButtonPiece)
};

/*
#define mArrowButtonPiece_setArrow(self, arrow) \
	((self)->revert = ((arrow)>>NCS_PIECE_PAINT_ARROW_SHIFT))
#define mArrowButtonPiece_getArrow(self) \
	((self)->revert<<NCS_PIECE_PAINT_ARROW_SHIFT)
*/

#ifdef __cplusplus
}
#endif

#endif
#endif		//_MGNCSCTRL_IMWORDSEL

