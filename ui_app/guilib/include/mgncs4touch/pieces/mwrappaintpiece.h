/*
 * \file 
 * \author FMSoft
 * \date 
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

#ifndef _MGUI_NCSCTRL_WRAPPAINT_H
#define _MGUI_NCSCTRL_WRAPPAINT_H

#ifdef __cplusplus
extern "C" {
#endif

enum {
    NCSP_WRAPPAINT_EFFECT = USER_PIECE_PROP_BEGIN + 1,
};

#define NCS_WRAPPAINT_ZOOM        1
#define NCS_WRAPPAINT_FOLDERPLANE 2

typedef struct _mWrapPaintPieceClass mWrapPaintPieceClass;
typedef struct _mWrapPaintPiece mWrapPaintPiece;

#define mWrapPaintPieceHeader(clss) \
    mStaticPieceHeader(clss) \
    mHotPiece* wrappiece;  \
    int effect;

#define mWrapPaintPieceClassHeader(clss, superCls) \
    mStaticPieceClassHeader(clss, superCls)

struct _mWrapPaintPiece
{   
    mWrapPaintPieceHeader(mWrapPaintPiece)
};

struct _mWrapPaintPieceClass
{   
    mWrapPaintPieceClassHeader(mWrapPaintPiece, mStaticPiece)
};

MGNCS_EXPORT extern mWrapPaintPieceClass g_stmWrapPaintPieceCls;

#ifdef __cplusplus
}
#endif
#endif /*_MGUI_NCSCTRL_WRAPPAINT_H*/
