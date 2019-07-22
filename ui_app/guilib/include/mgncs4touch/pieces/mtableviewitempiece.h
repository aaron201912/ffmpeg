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

#ifndef _MGNCS_TABLEVIEWITEMPIECEPIECE_H
#define _MGNCS_TABLEVIEWITEMPIECEPIECE_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef struct _mTableViewItemPiece mTableViewItemPiece;
typedef struct _mTableViewItemPieceClass mTableViewItemPieceClass;

enum mTableViewItemPieceProp
{
    NCSP_BACKGROUND_COLOR = USER_PIECE_PROP_BEGIN + 1,
    NCSP_HIGHTLIGHT_COLOR,
};

#define NCS_TABLEVIEWITEM_BKCOLOR_VALUE     0xFFFFFFFF
#define NCS_TABLEVIEWITEM_HIGHLIGHT_VALUE   0xFF0000FF

enum mTableViewItemPieceMode {
    NCS_TABLEVIEWITEMPIECE_NORMAL = 0,
    NCS_TABLEVIEWITEMPIECE_EDIT, 
};

enum mTableViewItemPieceEvent {
    NCSN_TABLEVIEWITEMPIECE_SWITCHCLICKED = USER_PIECE_EVENT_BEGIN + 0X200,
    NCSN_TABLEVIEWITEMPIECE_DELBTNCLICKED,
    NCSN_TABLEVIEWITEMPIECE_CONTENTCLICKED,
};

#define mTableViewItemPieceHeader(clss) \
	mPanelPieceHeader(clss) \
    enum mTableViewItemPieceMode mode; \
    mHotPiece *switchPiece; \
    mHotPiece *userPiece;   \
    mHotPiece *deletePiece; \
    POINT ptSwitchPiece[2]; \
    POINT ptUserPiece[2];   \
    POINT ptDeletePiece[2]; \
    BOOL userPieceIsLocked; \
    BOOL deletePieceIsAppear; \
    DWORD add_data; \
    const char* picture;\
    const char* text; \
    mHotPiece* accessory; \
    BOOL highlight; \
    BOOL pushDown; \
    MGEFF_ANIMATION modeChangeAnimation; \
    MGEFF_ANIMATION switchStateChangeAnimation; \
    DWORD bkgnd_color; \
    DWORD hilite_color;

struct _mTableViewItemPiece
{
	mTableViewItemPieceHeader(mTableViewItemPiece)
};

#define mTableViewItemPieceClassHeader(clss, superCls) \
	mPanelPieceClassHeader(clss, superCls) \
    void (*changeMode)(clss*); \
    void (*resetEditMode)(clss*); \
    void (*setHighlight)(clss*, BOOL); \
    void (*setUserPiece)(clss*, mHotPiece*); \
    mHotPiece* (*getSwitchPiece)(clss*);  \
    mHotPiece* (*getUserPiece)(clss*); \
    mHotPiece* (*getDeletePiece)(clss*);

struct _mTableViewItemPieceClass
{
	mTableViewItemPieceClassHeader(mTableViewItemPiece, mPanelPiece)
};

MGNCS_EXPORT extern mTableViewItemPieceClass g_stmTableViewItemPieceCls;

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _MGNCS_TABLEVIEWITEMPIECEPIECE_H */
