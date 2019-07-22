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

#ifndef _MGNCS_TABLEVIEWPIECE_H
#define _MGNCS_TABLEVIEWPIECE_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef struct _mTableViewPiece mTableViewPiece;
typedef struct _mTableViewPieceClass mTableViewPieceClass;

typedef struct _mIndexPath{
    int section;
    int row;
}mIndexPath;


typedef enum _TableViewAnimType {
    NCS_TABLEVIEW_ANIMATIONNONE = 0,
    NCS_TABLEVIEW_ANIMATIONFADE,
    NCS_TABLEVIEW_ANIMATIONZOOM,
    NCS_TABLEVIEW_ANIMATIONLEFT,
}TableViewAnimType;

enum mTableViewSeparatorStyle{
    NCS_TABLEVIEW_SEPARATORSTYLE_NONE = 0,
    NCS_TABLEVIEW_SEPARATORSTYLE_SINGLINE,
    NCS_TABLEVIEW_SEPARATORSTYLE_MAX,
};

enum mTableViewPieceState{
    NCS_TABLEVIEW_NORMAL = 0,
    NCS_TABLEVIEW_EDIT,
};

enum mTableViewPieceEvent {
    NCSN_TABLEVIEW_CONTENTCLICKED= USER_PIECE_EVENT_BEGIN +  0x400,
};

enum {
    NCS_TABLEVIEW_GROUP_STYLE = 0, 
    NCS_TABLEVIEW_INDEX_STYLE,
    NCS_TABLEVIEW_INDEXLOCATE_STYLE, /* 带索引定位条风格*/
};

enum mTableViewItemPieceType {
    NCS_TABLEVIEW_NORMALROWTYPE = 0,
    NCS_TABLEVIEW_SEPARATORTYPE,
    NCS_TABLEVIEW_SECTIONTYPE,
    NCS_TABLEVIEW_TITLETYPE,
};

#define MSG_TABLEVIEWPIECE_STATE_CHANGED   (MSG_USER + 100)
#define MSG_TABLEVIEWPIECE_DELITEM         (MSG_USER + 101)

#define NCS_TABLEVIEW_GROUPGAP      30
#define NCS_TABLEVIEW_INDEXLOCATE_W 30
#define NCS_TABLEVIEW_INDEXLOCATE_Y 0

#define NCS_TABLEVIEW_SEPARATOR_DEFAULTCOLOR 0xffced3d6

#define mTableViewPieceHeader(clss) \
	mScrollViewPieceHeader(clss) \
    mTableViewItemPiece* focusPiece; \
    int mode;\
    int style; \
    POINT indexPos;\
    mIndexLocatePiece* indexLocate; \
    mPanelPiece* tablePanel; \
    DWORD separatorColor;\
    int rowHeight;\
    int separatorStyle; 

struct _mTableViewPiece
{
	mTableViewPieceHeader(mTableViewPiece)
};

#define mTableViewPieceClassHeader(clss, superCls) \
	mScrollViewPieceClassHeader(clss, superCls) \
    void (*changeMode)(clss*); \
    void (*getDefaultRowRect)(clss* self, RECT* rect); \
    mPanelPiece* (*createSeparatorLine)(clss* self, BOOL border); \
    mPanelPiece* (*createDefaultRow)(clss* self, mTableViewItemPiece* item_piece);\
    void (*reloadData)(clss* self);\
    mPanelPiece* (*createIndexSectionHead)(clss* self, int section); \
    mPanelPiece* (*createIndexSectionContent)(clss* self, int section);\
    mPanelPiece* (*createGroupSectionContent)(clss* self, int section);\
    mTableViewItemPiece* (*indexPathToItem)(clss* self, const mIndexPath* indexpath);\
    void (*itemToIndexPath)(clss* self, const mTableViewItemPiece* item, mIndexPath* indexpath);\
    void (*insertRowAtIndexPath)(clss* self, const mIndexPath* indexpath, TableViewAnimType animtype);\
    void (*deleteRowAtIndexPath)(clss* self, const mIndexPath* indexpath, TableViewAnimType animtype);\
    void (*rectForHeaderInSection)(clss* self, int section, RECT* rect); \
    void (*rectForRowAtIndexPath)(clss* self, const mIndexPath* indexpath, RECT* rect);\
    void (*rectForSection)(clss* self, int section, RECT* rect);\
    void (*setSeparatorStyle)(clss* self, enum mTableViewSeparatorStyle);\
    void (*setSeparatorColor)(clss* self, DWORD color);\
    /* public interface. */ \
    BOOL (*willSelectRowAtIndexPath)(clss* self, const mIndexPath* indexpath);\
    void (*onCommitInsertRowAtIndexPath)(clss* self, const mIndexPath* indexpath); \
    void (*onCommitDeleteRowAtIndexPath)(clss* self, const mIndexPath* indexpath); \
    mTableViewItemPiece* (*createItemForRow)(clss* self, const mIndexPath* indexpath);\
    int (*numberOfSections)(clss* self); \
    int (*numberOfRowsInSection)(clss* self, int section);\
    const char* (*titleForSection)(clss* self, int section); \
    const char* (*indexForSection)(clss* self, int section);\
    void (*rowDidSelectAtIndexPath)(clss* self, const mIndexPath* indexpath);
    

struct _mTableViewPieceClass
{
	mTableViewPieceClassHeader(mTableViewPiece, mScrollViewPiece)
};

MGNCS_EXPORT extern mTableViewPieceClass g_stmTableViewPieceCls;

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _MGNCS_TABLEVIEWPIECE_H */
