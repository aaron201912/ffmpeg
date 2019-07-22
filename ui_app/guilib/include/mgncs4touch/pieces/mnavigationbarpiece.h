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

#ifndef _MGNCS_MNAVIGATION_BAR_PIECE_H
#define _MGNCS_MNAVIGATION_BAR_PIECE_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _mNavigationBarPieceClass mNavigationBarPieceClass;
typedef struct _mNavigationBarPiece mNavigationBarPiece;


/* navigation bar default configure */
//#define NAVIGATIONBAR_W                           SCREEN_W
#define NAVIGATIONBAR_W                           800
#define NAVIGATIONBAR_H                           38
#define NAVIGATIONBAR_DEFAULT_BUTTON_H            27
#define NAVIGATIONBAR_HORIZONAL_W                 8 
#define NAVIGATIONBAR_BUTTON_DX                   10 
#define NAVIGATIONBAR_NORMALBUTTON_COLOR1         0xffffffff
#define NAVIGATIONBAR_NORMALBUTTON_COLOR2         0xffd1d1d1
#define NAVIGATIONBAR_PUSHBUTTON_COLOR1           0xa0e2edf9
#define NAVIGATIONBAR_PUSHBUTTON_COLOR2           0xa0a1abb7
#define NAVIGATIONBAR_PUSHBUTTON_COLOR3           0xa0e2edf9
#define NAVIGATIONBAR_BORDER_COLOR1               0xffaaaaaa
#define NAVIGATIONBAR_BORDER_COLOR2               0xff888888
#define NAVIGATIONBAR_NORMAL_POS1                 0.0
#define NAVIGATIONBAR_NORMAL_POS2                 1.0
#define NAVIGATIONBAR_BORDER_POS1                 0.0
#define NAVIGATIONBAR_BORDER_POS2                 1.0
#define NAVIGATIONBAR_BORDER_SIZE                 1
#define NAVIGATIONBAR_DEFAULT_TITLE_FONT_SIZE     20
#define NAVIGATIONBAR_DEFAULT_BUTTON_FONT_SIZE    16
#define NAVIGATIONBAR_DEFAULT_ROUND_RADIUS        4
#define NAVIGATIONBAR_DEFAULT_SHARPWIDTH          10
#define NAVIGATIONBAR_DEFAULT_FONT_COLOR          0xff192952
#define NAVIGATIONBAR_DEFAULT_FONT_SHADOW_COLOR   0x66ffffff
#define NAVIGATIONBAR_DEFAULT_BUTTON_COLOR        0xffe6e6e6
#define NAVIGATIONBAR_DEFAULT_BUTTON_Y            ((NAVIGATIONBAR_H - NAVIGATIONBAR_DEFAULT_BUTTON_H)/2)
#define NAVIGATIONBAR_TITLE_X(w)                  (NAVIGATIONBAR_W/2 - (w)/2)
#define NAVIGATIONBAR_RIGHT_X(w)                  (NAVIGATIONBAR_W - NAVIGATIONBAR_HORIZONAL_W - (w))


/* NavigationBarPiece set and get properties */
#define NAVIGATIONBARPIECE_PROP_BEGIN    USER_PIECE_PROP_BEGIN + 50
enum {
    NCSP_NAVIGATIONBARPIECE_BACKGROUND = NAVIGATIONBARPIECE_PROP_BEGIN,
    NCSP_NAVIGATIONBARPIECE_BKG,
    NCSP_NAVIGATIONBARPIECE_LEFT_BUTTON,
    NCSP_NAVIGATIONBARPIECE_TITLE_BUTTON,
    NCSP_NAVIGATIONBARPIECE_RIGHT_BUTTON,
};


/* NaviagtionBarPiece methods */
#define mNavigationBarPieceClassHeader(clss, superCls) \
    mPanelPieceClassHeader(clss, superCls) 

struct _mNavigationBarPieceClass
{   
    mNavigationBarPieceClassHeader(mNavigationBarPiece, mPanelPiece)
};

MGNCS_EXPORT extern mNavigationBarPieceClass g_stmNavigationBarPieceCls;


/* NaviagtioBarPiece data members */
#define mNavigationBarPieceHeader(clss) \
    mPanelPieceHeader(clss) \
    mShapeTransRoundPiece* background; \
    mButtonPanelPiece* left_button; \
    mTextPiece* title_button; \
    mButtonPanelPiece* right_button; 

struct _mNavigationBarPiece
{   
    mNavigationBarPieceHeader(mNavigationBarPiece)
};


#ifdef __cplusplus
}
#endif

#endif /*_MGNCS_MNAVIGATION_BAR_PIECE_H*/

