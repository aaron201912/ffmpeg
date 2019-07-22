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
#ifndef _MGNCS_IMAGEPIECE_H
#define _MGNCS_IMAGEPIECE_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _mImagePieceClass mImagePieceClass;
typedef struct _mImagePiece mImagePiece;

#define mImagePieceClassHeader(clss, superCls) \
	mStaticPieceClassHeader(clss, superCls)

struct _mImagePieceClass
{
	mImagePieceClassHeader(mImagePiece, mStaticPiece)
};

MGNCS_EXPORT extern mImagePieceClass g_stmImagePieceCls;

#define mImagePieceHeader(clss) \
	mStaticPieceHeader(clss) \
	ImageDrawInfo img;

struct _mImagePiece
{
	mImagePieceHeader(mImagePiece)
};

#define mImagePiece_setAlign(self, align) ((self)->img.revert=((((self)->img.revert)&0xF0)|((align)&0xF)))
#define mImagePiece_getAlign(self)   ((self)->img.revert&0xF)

#define mImagePiece_setVAlign(self, valign) ((self)->img.revert=(((self)->img.revert&0x0F)|(((valign)&0xF)<<4)))
#define mImagePiece_getVAlign(self)  (((self)->img.revert&0xF0)>>4)

enum mImagePieceProps {
	NCSP_IMAGEPIECE_IMAGE = IMAGEPIECE_PROP_BEGIN,
	NCSP_IMAGEPIECE_ICON,
	NCSP_IMAGEPIECE_MYBITMAP,
	NCSP_IMAGEPIECE_IMAGEFILE,
	NCSP_IMAGEPIECE_ALIGN    = PIECECOMM_PROP_ALIGN,
	NCSP_IMAGEPIECE_VALIGN   = PIECECOMM_PROP_VALIGN,
	NCSP_IMAGEPIECE_DRAWMODE = PIECECOMM_PROP_DRAWMOD
};



#ifdef __cplusplus
}
#endif

#endif

