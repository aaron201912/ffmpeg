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

#ifndef TRANSROUNDPIECE_H
#define TRANSROUNDPIECE_H

#include <mgplus/mgplus.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum _TranRoundPaintMode {
    TRANROUND_PAINTMODE_GRAPHICSAVE,
    TRANROUND_PAINTMODE_BITBLT,
}TRANROUND_PAINTMODE;

typedef enum _TranRoundFillMode{
    NCSP_TRANROUND_SINGLE_FILL,
    NCSP_TRANROUND_GRADIENT_FILL,
}TRANROUND_FILLMODE;

enum mShapeTransRoundPieceProp
{
    NCSP_TRANROUND_RADIUS = USER_PIECE_PROP_BEGIN + 1,
    NCSP_TRANROUND_PAINTWAY,
    NCSP_TRANROUND_BKCOLOR,
    NCSP_TRANROUND_BORDERSIZE,
    NCSP_TRANROUND_BORDERCOLOR,
    NCSP_TRANROUND_GRADIENTBORDER,
    NCSP_TRANROUND_FILLMODE,
    NCSP_TRANROUND_CORNERFLAG,
    NCSP_TRANROUND_PAINTMODE,
    NCSP_TRANROUND_SHARPFLAG,
    NCSP_TRANROUND_SHARPWIDTH,
    NCSP_TRANROUND_USESHADOW,
    NCSP_TRANROUND_FILLENGINE,
};

enum _TranRoundCornerFlag {
    TRANROUND_CORNERFLAG_LEFTTOP = 1,
    TRANROUND_CORNERFLAG_RIGHTTOP = 2,
    TRANROUND_CORNERFLAG_RIGHTBOTTOM = 4,
    TRANROUND_CORNERFLAG_LEFTBOTTOM = 8,
};

enum _TranRoundSharpFlag {
    TRANROUND_SHARPFLAG_LEFT = 1,
    TRANROUND_SHARPFLAG_RIGHT = 2,
};

enum _TranRoundFillEngine{
    TRANROUND_FILLENGINE_NORMAL,
    TRANROUND_FILLENGINE_PLUS,
};

typedef struct _mColorTable {
    unsigned int tableSize;
    ARGB *colors;
    float *positions;
} mColorTable;

typedef struct _mShapeTransRoundPieceClass mShapeTransRoundPieceClass;
typedef struct _mShapeTransRoundPiece mShapeTransRoundPiece;

#define mShapeTransRoundPieceHeader(clss) \
	mStaticPieceHeader(clss) \
    int corner_radius; \
    int corner_flag; \
    int sharp_width; \
    int sharp_flag; \
    int border_size; \
    int fill_engine; \
    BOOL use_gradient_border; \
    HBRUSH brush_solid; \
    HBRUSH brush_gradient; \
    HBRUSH brush_gradient_border; \
    HGRAPHICS hgs; \
    ARGB border_color; \
    ARGB bk_color; \
    int hgs_width; \
    int hgs_height; \
    BOOL use_shadow; \
    TRANROUND_FILLMODE fill_mode; \
    TRANROUND_PAINTMODE paint_mode; \
    void *paintBuffer;

struct _mShapeTransRoundPiece
{
    mShapeTransRoundPieceHeader(mShapeTransRoundPiece)
};
   
#define mShapeTransRoundPieceClassHeader(clss, superCls) \
    mStaticPieceClassHeader(clss, superCls) \
    HBRUSH (*getBrush) (clss*); \
    HBRUSH (*getBorderBrush)(clss*); \
    int (*setGradientFillColors) (clss*, ARGB *, int);

struct _mShapeTransRoundPieceClass
{   
    mShapeTransRoundPieceClassHeader (mShapeTransRoundPiece, mStaticPiece)
};

// global functions
extern void ShapeTransRoundPiece_setColorTable(mColorTable *colorTable,
        ARGB *colors, float *pos, int num);
extern void ShapeTransRoundPiece_setBrushGradientColors (HBRUSH brush,
        mColorTable *colorTable);
 
MGNCS_EXPORT extern mShapeTransRoundPieceClass g_stmShapeTransRoundPieceCls;

#ifdef __cplusplus
}
#endif
#endif /* TRANSROUNDPIECE_H */
