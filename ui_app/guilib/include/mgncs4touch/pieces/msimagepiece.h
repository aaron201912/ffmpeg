/*
 * \file msimagepiece.h
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
#ifndef __MGNCS4TOUCH_SIMAGEPIECE_H__
#define __MGNCS4TOUCH_SIMAGEPIECE_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _mSImagePieceClass mSImagePieceClass;
typedef struct _mSImagePiece      mSImagePiece;

#define mSImagePieceClassHeader(clss, superCls) \
	mImagePieceClassHeader(clss, superCls)

struct _mSImagePieceClass {
	mSImagePieceClassHeader(mSImagePiece, mImagePiece)
};

MGNCS_EXPORT extern mSImagePieceClass g_stmSImagePieceCls;

#define mSImagePieceHeader(clss) \
	mImagePieceHeader(clss)      \
	int state;

struct _mSImagePiece {
	mSImagePieceHeader(mSImagePiece)
};

// #define mSImagePiece_setAlign(self, align) ((self)->img.revert=((((self)->img.revert)&0xF0)|((align)&0xF)))
// #define mSImagePiece_getAlign(self)   ((self)->img.revert&0xF)

// #define mSImagePiece_setVAlign(self, valign) ((self)->img.revert=(((self)->img.revert&0x0F)|(((valign)&0xF)<<4)))
// #define mSImagePiece_getVAlign(self)  (((self)->img.revert&0xF0)>>4)

#if 0
enum mSImagePieceProps {
	NCSP_IMAGEPIECE_IMAGE = IMAGEPIECE_PROP_BEGIN,
	NCSP_IMAGEPIECE_ICON,
	NCSP_IMAGEPIECE_MYBITMAP,
	NCSP_IMAGEPIECE_IMAGEFILE,
	NCSP_IMAGEPIECE_ALIGN    = PIECECOMM_PROP_ALIGN,
	NCSP_IMAGEPIECE_VALIGN   = PIECECOMM_PROP_VALIGN,
	NCSP_IMAGEPIECE_DRAWMODE = PIECECOMM_PROP_DRAWMOD
};
#endif

#ifdef __cplusplus
}
#endif

#endif	// __MGNCS4TOUCH_SIMAGEPIECE_H__

