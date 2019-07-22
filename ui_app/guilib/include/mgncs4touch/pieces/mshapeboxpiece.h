/*
 * \file mshapeboxpiece.h
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

#ifndef __MGNCS4TOUCH_SHAPEBOXPIECE_H__
#define __MGNCS4TOUCH_SHAPEBOXPIECE_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _mShapeBoxPieceClass mShapeBoxPieceClass;
typedef struct _mShapeBoxPiece      mShapeBoxPiece;

#define mShapeBoxPieceClassHeader(clss, superCls) \
	mStaticPieceClassHeader(clss, superCls)

struct _mShapeBoxPieceClass {
	mShapeBoxPieceClassHeader(mShapeBoxPiece, mStaticPiece)
};

MGNCS_EXPORT extern mShapeBoxPieceClass g_stmShapeBoxPieceCls;

#define mShapeBoxPieceHeader(clss) \
	mStaticPieceHeader(clss)
	
struct _mShapeBoxPiece {
	mShapeBoxPieceHeader(mShapeBoxPiece)
};

#ifdef __cplusplus
}
#endif

#endif	// __MGNCS4TOUCH_SHAPEBOXPIECE_H__

