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

#ifndef _MGNCS_ITEMPIECE_H
#define _MGNCS_ITEMPIECE_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define ITEMPIECE_WIDTH       480
#define ITEMPIECE_HEIGHT      50

typedef struct _mItemPiece mItemPiece;
typedef struct _mItemPieceClass mItemPieceClass;

enum mItemPieceState {
    ITEMPIECE_STATE_NORMAL,
    ITEMPIECE_STATE_PUSHED,
};

enum mItemPieceEvent {
    NCSN_ITEMPIECE_CONTENT_CLICKED = USER_PIECE_EVENT_BEGIN + 0x700,
    NCSN_ITEMPIECE_BUTTON_CLICKED,
    NCSN_ITEMPIECE_CHECKBOX_ON,
    NCSN_ITEMPIECE_CHECKBOX_OFF,
};

enum mItemPieceAccessoryControlStyle {
    ITEMPIECE_AC_INDICATORBUTTON,
    ITEMPIECE_AC_SWITCH,
    ITEMPIECE_AC_CHECKMARK,
    ITEMPIECE_AC_RADIO,
    ITEMPIECE_AC_INDICATOR,
};

#define mItemPieceHeader(clss) \
	mPanelPieceHeader(clss) \
    HDC privateDC; \
    int state; \
    unsigned margin; \
    unsigned border; \
    unsigned clearance; \
    int offsetLeft; \
    int offsetRight; \
    int align; \
    int valign; \
    BOOL isTitlePiece; \
    ARGB titleColor; \
    ARGB subTitleColor; \
    ARGB detailColor; \
    ARGB bkColor; \
    PLOGFONT titleFont; \
    PLOGFONT subTitleFont; \
    PLOGFONT detailFont; \
    PLOGFONT defaultTitleFont; \
    PLOGFONT defaultSubTitleFont; \
    PLOGFONT defaultDetailFont; \
    mImagePiece *imagePiece; \
    mTextPiece *titlePiece; \
    mTextPiece *subTitlePiece; \
    mTextPiece *detailPiece; \
    mShapeTransRoundPiece *bkPiece; \
    mHotPiece *accessoryControlPiece; \
    int acStyle; \
    int childAlign[6]; \
    int childrenTotal;

struct _mItemPiece
{
	mItemPieceHeader(mItemPiece)
};

#define mItemPieceClassHeader(clss, superCls) \
	mPanelPieceClassHeader(clss, superCls) \
    HDC myDC; \
    void (*setLayout)(clss*, int); \
    void (*activeLayout)(clss*); \
    void (*setImage)(clss*, PBITMAP, int, int); \
    void (*setTitle)(clss*, const char*, PLOGFONT, ARGB*); \
    void (*setSubTitle)(clss*, const char*, PLOGFONT, ARGB*); \
    void (*setDetail)(clss*, const char*, PLOGFONT, ARGB*); \
    void (*setSubStance)(clss*, const char*, PLOGFONT, ARGB*); \
    void (*setAccessoryControl)(clss*, const char*, BOOL, int, DWORD); \
    void (*setCheckBoxState)(clss*, BOOL); \
    int (*getCheckBoxState)(clss*); \
    BOOL (*setItemRect)(clss*, const RECT*);

struct _mItemPieceClass
{
	mItemPieceClassHeader(mItemPiece, mPanelPiece)
};

enum mItemPieceProperty {
    NCSP_ITEMPIECE_MARGIN = USER_PIECE_PROP_BEGIN + 1,
    NCSP_ITEMPIECE_BORDER,
    NCSP_ITEMPIECE_CLEARANCE,
    NCSP_ITEMPIECE_VALIGN,
    NCSP_ITEMPIECE_BKCOLOR,
    NCSP_ITEMPIECE_GRADIENT_BKCOLOR,
    NCSP_ITEMPIECE_MAX
};

MGNCS_EXPORT extern mItemPieceClass g_stmItemPieceCls;

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _MGNCS_ITEMPIECE_H */
