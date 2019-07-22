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
#ifndef _MGNCS_STATICPIECE_H
#define _MGNCS_STATICPIECE_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _mStaticPieceClass mStaticPieceClass;
typedef struct _mStaticPiece mStaticPiece;

#define mStaticPieceClassHeader(clss, superCls) \
	mHotPieceClassHeader(clss, superCls)

struct _mStaticPieceClass
{
	mStaticPieceClassHeader(mStaticPiece, mHotPiece)
};

MGNCS_EXPORT extern mStaticPieceClass g_stmStaticPieceCls;

#define mStaticPieceHeader(clss) \
	mHotPieceHeader(clss) \
	int left:16; \
	int top:16; \
	int right:16; \
	int bottom:16;

struct _mStaticPiece
{
	mStaticPieceHeader(mStaticPiece)
};

#define SetStaticPieceRect(self, prc) do{ \
	(self)->left = (short)((prc->left)&0xFFFF); \
	(self)->top = (short)((prc->top)&0xFFFF); \
	(self)->right = (short)((prc->right)&0xFFFF); \
	(self)->bottom = (short)((prc->bottom)&0xFFFF); \
}while(0)


#ifdef __cplusplus
}
#endif

#endif

