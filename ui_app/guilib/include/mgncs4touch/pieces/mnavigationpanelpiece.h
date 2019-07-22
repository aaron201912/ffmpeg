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

#ifndef _MGNCS_MNAVIGATION_PANEL_PIECE_H
#define _MGNCS_MNAVIGATION_PANEL_PIECE_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _mNavigationPanelPieceClass mNavigationPanelPieceClass;
typedef struct _mNavigationPanelPiece mNavigationPanelPiece;


/* NavigationItem list */
typedef struct _item_list_t {
    list_t list;
    mNavigationItem* item;
} item_list_t;


/* NavigationPanelPiece set and get properties */
#define NAVIGATIONPANELPIECE_PROP_BEGIN    USER_PIECE_PROP_BEGIN + 100
enum {
    NCSP_NAVIGATIONPANELPIECE_BAR = NAVIGATIONBARPIECE_PROP_BEGIN,
    NCSP_NAVIGATIONPANELPIECE_BAR_BKG,
};


/* NaviagtionPanelPiece methods */
#define mNavigationPanelPieceClassHeader(clss, superCls) \
    mPanelPieceClassHeader(clss, superCls) \
    void (*push)(clss*, mNavigationItem*); \
    void (*pop)(clss*); \
    void (*showNavigationBar)(clss*, BOOL, BOOL); \
    BOOL (*currentIsRoot)(clss*);

/* static interface*/
MGNCS_EXPORT extern mNavigationPanelPiece* ncsCreateNavigationPanelPieceWithRootView (mNavigationItem *rootItem);

struct _mNavigationPanelPieceClass
{   
    mNavigationPanelPieceClassHeader(mNavigationPanelPiece, mPanelPiece)
};

MGNCS_EXPORT extern mNavigationPanelPieceClass g_stmNavigationPanelPieceCls;


/* NaviagtioPanelPiece data members */
#define mNavigationPanelPieceHeader(clss) \
    mPanelPieceHeader(clss) \
    PLOGFONT default_title_font; \
    list_t item_head; \
    mNavigationBarPiece* bar;

struct _mNavigationPanelPiece
{   
    mNavigationPanelPieceHeader(mNavigationPanelPiece)
};


#ifdef __cplusplus
}
#endif

#endif /*_MGNCS_MNAVIGATION_PANEL_PIECE_H*/

