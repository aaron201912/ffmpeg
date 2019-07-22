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
#ifndef _MGNCS_RECTPIECE_H
#define _MGNCS_RECTPIECE_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _mRectPieceClass mRectPieceClass;
typedef struct _mRectPiece mRectPiece;

#define mRectPieceClassHeader(clss, superCls) \
	mStaticPieceClassHeader(clss, superCls)

struct _mRectPieceClass
{
	mRectPieceClassHeader(mRectPiece, mStaticPiece)
};

MGNCS_EXPORT extern mRectPieceClass g_stmRectPieceCls;

#define mRectPieceHeader(clss) \
	mStaticPieceHeader(clss) \
	DWORD fill_color; \
	DWORD border_color; \
	unsigned int border_size:12; \
	unsigned int x_radius:10; \
	unsigned int y_radius:10;

struct _mRectPiece
{
	mRectPieceHeader(mRectPiece)
};

enum mRectPieceProps {
	NCSP_RECTPIECE_FILLCOLOR    = RECTPIECE_PROP_BEGIN,
	NCSP_RECTPIECE_BORDERCOLOR,
	NCSP_RECTPIECE_BORDERSIZE,
	NCSP_RECTPIECE_XRADIUS,
	NCSP_RECTPIECE_YRADIUS,
	NCSP_RECTPIECE_FILLCLR_RED, //FILLCOLOR 
	NCSP_RECTPIECE_FILLCLR_GREEN,
	NCSP_RECTPIECE_FILLCLR_BLUE,
	NCSP_RECTPIECE_FILLCLR_ALPHA,
	NCSP_RECTPIECE_BRDCLR_RED, //BorderColor
	NCSP_RECTPIECE_BRDCLR_GREEN,
	NCSP_RECTPIECE_BRDCLR_BLUE,
	NCSP_RECTPIECE_BRDCLR_ALPHA,
	NCSP_RECTPIECE_MAX
};


#ifdef __cplusplus
}
#endif

#endif

