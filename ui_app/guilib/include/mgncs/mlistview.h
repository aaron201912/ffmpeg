/**
 * \file mlistview.h
 * \author XiaoweiYan
 * \date 2009/06/13
 *
 * This file includes the definition of ListView.
 *
 \verbatim

    This file is part of mGNCS, a component for MiniGUI.

    Copyright (C) 2008~2018, Beijing FMSoft Technologies Co., Ltd.

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

#ifdef _MGNCSCTRL_LISTVIEW

#ifndef _MGUI_CTRL_LISTV_H
#define _MGUI_CTRL_LISTV_H


#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

/**
 * \defgroup Widget_ListView mListView
 * @{
 */

typedef struct _mListView mListView;
typedef struct _mListViewClass mListViewClass;
typedef struct _mListViewRenderer mListViewRenderer;

/**
 * \enum ncsListVIRType
 * \brief The relationship between items.
 */
typedef enum
{
    /** Parent */
    NCSID_LISTV_IR_PARENT,
    /** First child */
    NCSID_LISTV_IR_FIRSTCHILD,
    /** Last child */
    NCSID_LISTV_IR_LASTCHILD,
    /** next */
    NCSID_LISTV_IR_NEXTSIBLING,
    /** previous */
    NCSID_LISTV_IR_PREVSIBLING,
}ncsListVIRType;

/**
 * \def NCSF_LSTHDR_LEFTALIGN
 * \brief Horizontal left align.
 */
#define NCSF_LSTHDR_LEFTALIGN       (0x00<<NCSF_LSTCLM_SHIFT)

/**
 * \def NCSF_LSTHDR_TOPALIGN
 * \brief Vertical top align.
 */
#define NCSF_LSTHDR_TOPALIGN        (0x00<<NCSF_LSTCLM_SHIFT)

/**
 * \def NCSF_LSTHDR_RIGHTALIGN
 * \brief Horizontal right align.
 */
#define NCSF_LSTHDR_RIGHTALIGN      (0x01<<NCSF_LSTCLM_SHIFT)

/**
 * \def NCSF_LSTHDR_CENTERALIGN
 * \brief Horizontal center align.
 */
#define NCSF_LSTHDR_CENTERALIGN     (0x02<<NCSF_LSTCLM_SHIFT)

/**
 * \def NCSF_LSTHDR_BOTTOMALIGN
 * \brief Vertical bottom align.
 */
#define NCSF_LSTHDR_BOTTOMALIGN     (0x04<<NCSF_LSTCLM_SHIFT)

/**
 * \def NCSF_LSTHDR_VCENTERALIGN
 * \brief Vertical center align.
 */
#define NCSF_LSTHDR_VCENTERALIGN    (0x08<<NCSF_LSTCLM_SHIFT)

/**
 * \def NCSF_LSTHDR_VALIGNMASK
 * \brief The mask of vertical align.
 */
#define NCSF_LSTHDR_VALIGNMASK      (0x0C<<NCSF_LSTCLM_SHIFT)

/**
 * \def NCSF_LSTHDR_SHIFT
 * \brief The bits used by header in flags.
 */
#define NCSF_LSTHDR_SHIFT           (NCSF_LSTCLM_SHIFT+4)

/**
 * \enum ncsListVFindType
 * \brief The found type.
 */
typedef enum
{
    /** According to additional data to find item. */
    NCSID_LISTV_FT_ADDDATA,
    /** According to item text to find item. */
    NCSID_LISTV_FT_TEXT,
}ncsListVFindType;

/**
 * The structure of searching item information.
 */
typedef struct _NCS_LISTV_FINDINFO
{
    /** The found type. \sa ncsListVFindType */
    ncsListVFindType type;

    /** The searching start item. */
    HITEM   parent;

    /** The searching start item index. Note when parent is null, use it as start searching position. */
    int     startIdx;

    /** The searching data information. */
    union {
        /** The searching text. */
        struct {
            /* The number of searching column. */
            int     size;
            /* The text of searching column */
            char    **text;
        }string;
        /** The additional data. */
        DWORD   addData;
    }data;
}NCS_LISTV_FINDINFO;

/**
 * The structure of list column information.
 */
typedef struct _NCS_LISTV_CLMINFO
{
    /** The column index */
    int     index;
    /** The column width */
    int     width;
    /** The column text */
    char    *text;
    /** The column image */
    DWORD   image;
    /** The column flag, it can be OR'ed the following values:
     *  - NCSF_LSTCLM_LEFTALIGN\n
     *    Current column text is left align. Default value.
     *  - NCSF_LSTCLM_CENTERALIGN\n
     *    Current column text is center align.
     *  - NCSF_LSTCLM_RIGHTALIGN\n
     *    Current column text is right align.
     *  - NCSF_LSTHDR_LEFTALIGN\n
     *    The header text is left align. Default value.
     *  - NCSF_LSTHDR_CENTERALIGN\n
     *    The header text is center align.
     *  - NCSF_LSTHDR_RIGHTALIGN\n
     *    The header text is right align.
     *  */
    DWORD   flags;
    NCS_CB_LISTV_CMPCLM pfnCmp;

    /*
     * \sa ncsLstClmSortType
     */
    ncsLstClmSortType sort;
} NCS_LISTV_CLMINFO;

/**
 * The data information of item in specified row and column.
 */
typedef struct _NCS_LISTV_ITEMDATA
{
    /** The row index. */
    int     row;
    /** The column index. */
    int     col;
    /** The text. */
    char*   text;
    /** The text foreground color. */
    int     textColor;

    /** The item flags. It can be one of the following values:
     *  - NCSF_ITEM_USEBITMAP\n
     *    The item image is BITMAP.
     *  - NCSF_ITEM_USEICON\n
     *    The item image is icon.
     */
    DWORD   flags;
    /** The pointer to BITMAP ro ICON. */
    DWORD   image;
}NCS_LISTV_ITEMDATA;

/**
 * The structure of listitem information.
 */
typedef struct _NCS_LISTV_ITEMINFO
{
    /** The item index. */
    int     index;
    /** The postion which item has been inserted in. */
    int     retPos;
    /** The parent, can be null. If null, the item will act as first-level node.*/
    HITEM   parent;
    /** The visible height */
    int     height;
    /** The flags, can be one of the following values:
     *  - NCSF_ITEM_NORMAL
     *  - NCSF_ITEM_SELECTED
     *  - NCSF_ITEM_DISABLED
     *  - NCSF_LSTITM_FOLD
     */
    int     flags;
    /** The folded icon */
    HICON   foldIcon;
    /** The unfolded icon */
    HICON   unfoldIcon;
    /** The number of item data. */
    int     dataSize;
    /** The item data */
    NCS_LISTV_ITEMDATA *data;
	/** user additonal data */
	DWORD   addData;
}NCS_LISTV_ITEMINFO;

/** Type of drawing header background. */
typedef void (*NCS_CB_LISTV_DRAWHDRBK)(mListView *self, HITEM hHdr, HDC hdc, RECT *rcDraw);
/** Type of drawing header item. */
typedef void (*NCS_CB_LISTV_DRAWHDR)(mListView *self, HITEM hHdr, HDC hdc, RECT *rcDraw);

typedef void (*NCS_CB_DRAWITEMBK)(mListView *self, HITEM hItem, HDC hdc, RECT *rcDraw);
/** Contains function pointers for customized drawing */
typedef struct _NCS_CB_LISTV_CSTMHDROPS
{
    /** Colunm header background drawing function */
    NCS_CB_LISTV_DRAWHDRBK pfnDrawHdrBk;
    /** Colunm header item drawing function */
    NCS_CB_LISTV_DRAWHDR   pfnDrawHdrItem;
} NCS_CB_LISTV_CSTMHDROPS;


#define NCSSPEC_LISTV_HDR   MAX_USER_SPECIAL_KEY + 2000

typedef struct _NCS_LISTV_CLMRD{
	const char* text;
	const char* image_file;
	DWORD       width;
	DWORD       flags;
}NCS_LISTV_CLMRD;

MGNCS_EXPORT extern const unsigned char gListVColumnRecordTypes[];

/**
 * \def NCSCTRL_LISTVIEW
 * \brief The control class name of mListView.
 */
#define NCSCTRL_LISTVIEW NCSCLASSNAME("listview")

/**
 * \def NCSS_LISTV_NOTIFY
 * \sa NCSS_NOTIFY
 */
#define NCSS_LISTV_NOTIFY    NCSS_NOTIFY

/**
 * \def NCSS_LISTV_LOOP
 * \sa NCSS_ITEMV_LOOP
 */
#define NCSS_LISTV_LOOP      NCSS_ITEMV_LOOP
/**
 * \def NCSS_LISTV_MULTIPLE
 * \sa NCSS_ITEMV_MULTIPLE. Reserved.
 */
#define NCSS_LISTV_MULTIPLE     NCSS_ITEMV_MULTIPLE

/**
 * \def NCSS_LISTV_SINGLE
 * \sa NCSS_ITEMV_SINGLE
 */
#define NCSS_LISTV_SINGLE     NCSS_ITEMV_SINGLE

/**
 * \def NCSS_LISTV_CHECKBOX
 * \brief Display a check box in an item. Reserved.
 */
#define NCSS_LISTV_CHECKBOX  (0x0001<<NCSS_ITEMV_SHIFT)

/**
 * \def NCSS_LISTV_AUTOCHECK
 * \brief Auto-switch the check box between checked or un-checked
 *        when the user click the check mark box of an item. Reserved.
 */
#define NCSS_LISTV_AUTOCHECK (0x0002<<NCSS_ITEMV_SHIFT)

/**
 * \def NCSS_LISTV_AUTOCHECKBOX
 * \brief Set the style NCSS_LISTV_CHECKBOX and NCSS_LISTV_AUTOCHECK. Reserved.
 */
#define NCSS_LISTV_AUTOCHECKBOX \
    (NCSS_LISTV_CHECKBOX | NCSS_LISTV_AUTOCHECK)

/**
 * \def NCSS_LISTV_TREE
 * \brief Uses listview in a treeview style.
 */
#define NCSS_LISTV_TREE      (0x0004<<NCSS_ITEMV_SHIFT)

/**
 * \def NCSS_LISTV_WITHICON
 * \brief Use icons to indicate folded or unfolded status.
 */
#define NCSS_LISTV_WITHICON  (0x0008<<NCSS_ITEMV_SHIFT)

/**
 * \def NCSS_LISTV_SORT
 * \brief Sort items automatically.
 */
#define NCSS_LISTV_SORT      (0x0010<<NCSS_ITEMV_SHIFT)

/**
 * \def NCSS_LISTV_NOHDRDRAG
 * \brief User cann't change column width by dragging header's border.
 */
#define NCSS_LISTV_NOHDRDRAG (0x0020<<NCSS_ITEMV_SHIFT)

/**
 * \def NCSS_LISTV_SHIFT
 * \brief The bits used by mListView in style.
 */
#define NCSS_LISTV_SHIFT     (NCSS_ITEMV_SHIFT + 6)

#define mListViewHeader(className)  \
    mItemViewHeader(className)      \
    NCS_CB_LISTV_DRAWHDRBK  drawHdrBk;  \
    NCS_CB_LISTV_DRAWHDR    drawHdrItem;\
    NCS_CB_DRAWITEMBK   drawItemBk; \
    NCS_CB_DRAWITEM     drawSubItem;\
    mItemManager *hdrList;          \
    mListItem *rootItem;            \
    ncsLstClmSortType sort;         \
    int curCol;                     \
    int gridLineWidth;              \
    int gridLineColor;              \
    NCS_CB_LISTV_CMPCLM pfnSort;    \
    int hdrH;                       \
    int hdrW;                       \
    int dragedHead;                 \
    int dragX;                      \
    mListColumn *clickedHead;       \
    DWORD status;

/**
 * \struct mListView
 * \brief The structure of mListView control, which derived from
 *        mItemView.
 *  - drawHdrBk\n
 *    User-defined function to draw header background.
 *  - drawHdrItem \n
 *    User-defined function to draw header item.
 *  - hdrList\n
 *    The header list.
 *  - rootItem\n
 *    Root item.
 *  - sort\n
 *    Current sort type.
 *  - curCol\n
 *    The index of current sort column.
 *  - gridLineWidth\n
 *    The width of grid line.
 *  - gridLineColor\n
 *    The color of grid line.
 *  - pfnSort\n
 *    The callback of sort function.
 *  - hdrH\n
 *    The height of header.
 *  - hdrW\n
 *    The width of header.
 *  - dragedHead\n
 *    The index of current draged header.
 *  - drawX\n
 *    The x coordinate when dragging header.
 *  - clickedHead\n
 *    The pointer to current clicked header.
 *  - status\n
 */
struct _mListView
{
	mListViewHeader(mListView)
};

#define mListViewClassHeader(clsName, parentClass)  \
    mItemViewClassHeader(clsName, parentClass)      \
    NCS_CB_DRAWITEMBK (*setItemBkDraw)(clsName*, NCS_CB_DRAWITEMBK func);\
    NCS_CB_DRAWITEM (*setSubItemDraw)(clsName*, NCS_CB_DRAWITEM func);  \
    HITEM (*getRelatedItem)(clsName*,HITEM hItem,ncsListVIRType type);  \
    void (*setBackground)(clsName*,int row,int col,int *color);         \
    void (*setForeground)(clsName*,int row,int col,int *color);         \
    int (*getBackground)(clsName*,int row,int col,int *color);          \
    int (*getForeground)(clsName*,int row,int col,int *color);          \
    void (*sort)(clsName*, NCS_CB_LISTV_CMPCLM func, int col, ncsLstClmSortType sort);\
    void (*setCustomDrawHeader)(clsName*, NCS_CB_LISTV_CSTMHDROPS *func);\
    BOOL (*setItemInfo)(clsName*, NCS_LISTV_ITEMDATA *info);            \
    BOOL (*getItemInfo)(clsName*, NCS_LISTV_ITEMDATA *info);            \
    const char* (*getItemText)(clsName*, int row, int col);             \
    int (*getItemTextLen)(clsName*, int row, int col);                  \
	BOOL (*setItemText)(clsName*, int row, int col, const char* str);   \
    void (*setSortDirection)(clsName*, ncsLstClmSortType direction);    \
    ncsLstClmSortType (*getSortDirection)(clsName*);                    \
    HITEM (*findItem)(clsName*, NCS_LISTV_FINDINFO *info);              \
    mListColumn* (*getColumn)(clsName*, int index);                     \
    int (*getColumnIndex)(clsName*, mListColumn *column);               \
    int (*getColumnWidth)(clsName*, int index);                         \
    void (*setColumnWidth)(clsName*, int index, int width);             \
    int (*getColumnCount)(clsName*);                                    \
    mListColumn* (*getSortColumn)(clsName*);                            \
    void (*setSortColumn)(clsName*, mListColumn* column);               \
    void (*showColumn)(clsName*, mListColumn *column);                  \
    BOOL (*delColumn)(clsName*, int index);                             \
    BOOL (*addColumn)(clsName*, NCS_LISTV_CLMINFO *info);               \
    HITEM (*addItem)(clsName*, NCS_LISTV_ITEMINFO *info);               \
    BOOL (*setHeadText)(clsName*, int col, const char* text);           \
    HITEM (*getChildItem)(clsName*, HITEM parent,int index);            \
    int (*getChildCount)(clsName*, HITEM hItem);                        \
    int (*foldItem)(clsName*, HITEM hItem, BOOL fold);


/**
 * \struct mListViewClass
 * \brief The virtual function table of mListView, which derived from
 *        mItemViewClass.
 *
 * - NCS_CB_DRAWITEMBK (*\b setItemBkDraw)(mListView *self,
 *                                      NCS_CB_DRAWITEMBK func);\n
 *  Set the user-defined function of drawing item background.
 *      \param func     The new funciton.
 *      \return         The old function.
 *
 * - NCS_CB_DRAWITEM (*\b setSubItemDraw)(mListView *self,
 *                                      NCS_CB_DRAWITEM func); \n
 *  Set the user-defined function of drawing subitem.
 *      \param func     The new funciton.
 *      \return         The old function.
 *
 * - BOOL (*\b setItemInfo)(mListView *self,
 *              \ref NCS_LISTV_ITEMDATA *info);\n
 *   Set item information.
 *      \param info     The new information of item.
 *      \return         TRUE on success, otherwise FALSE.
 *
 * - BOOL (*\b getItemInfo)(mListView *self,
 *              \ref NCS_LISTV_ITEMDATA *info);\n
 *   Get item information.
 *
 * - BOOL (*\b setItemText)(clsName*, int row, int col, const char* str); \n
 *   Set the text for item.
 *
 * - const char* (*\b getItemText)(mListView *self, int row, int col);\n
 *   Get the text in specified row and column.
 *      \param row      The row index. If \a rowItem is non-zero, needn't it.
 *      \param col      The column index.
 *      \return         the text on success, otherwise "".
 *
 * - int (*\b getItemTextLen)(mListView *self, int row, int col);\n
 *   Get the text length in specified row and column.
 *      \param row      The row index. If \a rowItem is non-zero, needn't it.
 *      \param col      The column index.
 *      \return         the text length on success, otherwise -1.
 *
 * - mListColumn * (*\b getColumn)(mListView *self, int index);\n
 *   Get the pointer to the specified column.
 *
 * - int (*\b getColumnIndex)(mListView *self, mListColumn *column); \n
 *   Get the index of column.
 *
 * - int (*\b getColumnWidth)(mListView *self, int index);           \n
 *   Get the width of column.
 *
 * - void (*\b setColumnWidth)(mListView *self, int index, int width);\n
 *   Set the width of column.
 *
 * - int (*\b getColumnCount)(mListView *self);                      \n
 *   Get the number of column.
 *
 * - void (*\b setSortDirection)(mListView *self, \ref ncsLstClmSortType direction);\n
 *   Set sorted type.
 *
 * - \ref ncsLstClmSortType (*\b getSortDirection)(mListView *self);\n
 *   Get sorted type.
 *
 * - mListColumn * (*\b getSortColumn)(mListView *self);              \n
 *   Get sorted column.
 *
 * - void (*\b setSortColumn)(mListView *self, mListColumn *column); \n
 *   Set sorted column.
 *
 * - void (*\b showColumn)(mListView *self, mListColumn *column);\n
 *   Show the specified column.
 *
 * - BOOL (*\b delColumn)(mListView *self, int index);\n
 *   Delete the specified column.
 *
 * - BOOL (*\b addColumn)(mListView *self, \ref NCS_LISTV_CLMINFO *info);\n
 *   Add a new column.
 *
 * - \ref HITEM (*\b addItem)(clsName*, \ref NCS_LISTV_ITEMINFO *info); \n
 *   Add a new item.
 *
 * - void (*\b setBackground)(mListView *self,
 *                      int row, int col, int *color); \n
 *   Set the background. If color is NULL, it will cancel
 *   previous setting.
 *
 * - void (*\b setForeground)(mListView *self,
 *                      int row, int col, int *color); \n
 *   Set the foreground. If color is NULL, it will cancel
 *   previous setting.
 *
 * - int (*\b getBackground)(mListView *self,
 *                      int row, int col, int *color); \n
 *   Get the background.
 *
 * - int (*\b getForeground)(mListView *self,
 *                      int row, int col, int *color); \n
 *   Get the foreground.
 *
 * - \ref HITEM (*\b findItem)(mListView *self, \ref NCS_LISTV_FINDINFO *info);\n
 *   Find a item.
 *
 * - HITEM (*\b getRelatedItem)(mListView *self,
 *                      HITEM hItem, \ref ncsListVIRType type);\n
 *   Get the related item.
 *      \param type     The relationship, can be one of the following
 *                      values:
 *                      - \ref NCSID_LISTV_IR_PARENT\n
 *                      - \ref NCSID_LISTV_IR_FIRSTCHILD\n
 *                      - \ref NCSID_LISTV_IR_LASTCHILD\n
 *                        If no children, will return itself.
 *                      - \ref NCSID_LISTV_IR_NEXTSIBLING\n
 *                      - \ref NCSID_LISTV_IR_PREVSIBLING\n
 *
 * - \ref HITEM (*getChildItem)(mListView *self, \ref HITEM parent, int index);\n
 *   Get the specified item according to parent and index.
 *
 * - int (*\b getChildCount)(mListView *self, HITEM hItem);\n
 *   Get the child number of the specified item.
 *
 * - int (*\b foldItem)(mListView *self, HITEM hItem, BOOL fold); \n
 *   Fold or unfold the specified item.
 *      \param fold     whether folded or not.
 *      \return         0 on success, otherwise -1.
 *
 * - BOOL (*\b setHeadText)(mListView *self, int col, const char* text); \n
 *   Set the header text of specified column.
 *
 * - void (*\b sort)(mListView *self, \ref NCS_CB_LISTV_CMPCLM func,
 *                      int col, \ref ncsLstClmSortType sort); \n
 *   Sort items.
 *      \param func     The comparison function.
 *      \param col      The index of sorted column.
 *      \param sort     The sort type.
 *
 * - void (*\b setCustomDrawHeader)(mListView *self, \ref NCS_CB_LISTV_CSTMHDROPS *func); \n
 *   Set the custom drawing header functions.
 *
 */
struct _mListViewClass
{
	mListViewClassHeader(mListView, mItemView)
};

#define mListViewRendererHeader(clsName, parentClass)               \
	mItemViewRendererHeader(clsName, parentClass)                   \
    void (*drawHeader) (clsName *self, HDC hdc, const RECT* pRect,  \
            DWORD color, int flag);                                 \
    void (*drawFold) (clsName *self, HDC hdc, const RECT* pRect,    \
            DWORD color, HICON icon, int status, int next);

/**
 * \struct mListViewRenderer
 * \brief The structure of mListView renderer, which derived
 *        from mItemViewRenderer.
 *
 *  - void (*\b drawHeader) (clsName *self, HDC hdc, const RECT* pRect,
 *              DWORD color, int flag); \n
 *    Draw header.
 *
 *  - void (*\b drawFold) (clsName *self, HDC hdc, const RECT* pRect,
            DWORD color, HICON icon, int status, int next);\n
      Draw folded or unfoded status.
 */
struct  _mListViewRenderer {
	mListViewRendererHeader(mListView, mItemView)
};

/**
 * \enum mListViewProp
 * \brief The properties id of mListView.
 */
enum mListViewProp
{
    /** The default item height.  */
    NCSP_LISTV_DEFITEMHEIGHT = NCSP_ITEMV_DEFITEMHEIGHT,
    /** The number of row. RO */
    NCSP_LISTV_ROWCOUNT = NCSP_ITEMV_ITEMCOUNT,
    /** The height of header.  */
    NCSP_LISTV_HDRHEIGHT = NCSP_ITEMV_MAX + 1,
    /** The width of header. Read only.  */
    NCSP_LISTV_HDRWIDTH,
    /** The flag of header visible.*/
    NCSP_LISTV_HDRVISIBLE,
    /** The sorted column index.  */
    NCSP_LISTV_SORTCOLUMN,
    /** The width of grid line.  */
    NCSP_LISTV_GRIDLINEWIDTH,
    /** The color of grid line.  */
    NCSP_LISTV_GRIDLINECOLOR,
    /** The number of column. Read only.  */
    NCSP_LISTV_COLCOUNT,
    /** The maximum value of mListView properties id.  */
	NCSP_LISTV_MAX,
};

/**
 * \enum mListViewNotify
 * \brief The notification code id of mListView.
 */
enum mListViewNotify
{
    /** Mouse click.  */
    NCSN_LISTV_CLICKED = NCSN_ITEMV_CLICKED,
    /** Selected item has changed.  */
    NCSN_LISTV_SELCHANGED = NCSN_ITEMV_SELCHANGED,
    /** Right mouse down in item.  */
    NCSN_LISTV_ITEMRDOWN = NCSN_ITEMV_MAX + 1,
    /** Right mouse up in item. */
    NCSN_LISTV_ITEMRUP,
    /** Right mouse down in header. */
    NCSN_LISTV_HDRRDOWN,
    /** Right mouse up in header. */
    NCSN_LISTV_HDRRUP,
    /** Double click in item. */
    NCSN_LISTV_ITEMDBCLK,
    /** Fold item.  */
    NCSN_LISTV_FOLDITEM,
    /** Unfold item.  */
    NCSN_LISTV_UNFOLDITEM,
    /** The maximum value of mListView notification code id.  */
	NCSN_LISTV_MAX
};

/**
 * \var g_stmListViewCls
 * \brief The instance of mListViewClass.
 *
 * \sa mListViewClass
 */
MGNCS_EXPORT extern mListViewClass g_stmListViewCls;
    /** @} end of Widget_ListView */

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif /* _MGUI_CTRL_LISTV_H */
#endif //_MGNCSCTRL_LISTVIEW
