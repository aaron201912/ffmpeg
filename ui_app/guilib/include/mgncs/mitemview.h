/**
 * \file mitemview.h
 * \author XiaoweiYan
 * \date 2009/06/15
 *
 * This file includes the definition of mItemView.
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

#ifndef _MGUI_CTRL_ITEMVIEW_H
#define _MGUI_CTRL_ITEMVIEW_H

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

/**
 * \defgroup Widget_ItemView mItemView
 * @{
 */


/**
 * \def NCSCTRL_ITEMVIEW
 * \brief The name of mItemView control.
 */
#define NCSCTRL_ITEMVIEW   NCSCLASSNAME("itemview")

typedef struct _mItemView mItemView;
typedef struct _mItemViewClass mItemViewClass;
typedef struct _mItemViewRenderer mItemViewRenderer;

/**
 * \var typedef int  (*NCS_CB_INITITEM)(mItemView *self, HITEM hItem);
 * \brief The callback of initializing item.
 */
typedef int  (*NCS_CB_INITITEM)(mItemView *self, HITEM hItem);

/**
 * \var typedef void (*NCS_CB_DSTRITEM)(mItemView *self, HITEM hItem);
 * \brief The callback of destroying item.
 */
typedef void (*NCS_CB_DSTRITEM)(mItemView *self, HITEM hItem);

/**
 * \var typedef void (*NCS_CB_DRAWITEM) (mItemView *self, HITEM hItem, HDC hdc, RECT *rcDraw);
 * \brief The callback of drawing item.
 */
typedef void (*NCS_CB_DRAWITEM)(mItemView *self, HITEM hItem, HDC hdc, RECT *rcDraw);

/**
 * \struct NCS_CB_ITEMOPS
 * \brief The structure of NCS_CB_ITEMOPS. The Item operations interface.
 */
typedef struct _NCS_CB_ITEMOPS
{
	/**
	 * \brief initialize an item, called when an itemview item is created
	 */
    NCS_CB_INITITEM     initItem; 
	/**
	 * \brief uninitialize an item, called when an item is destroied
	 */
    NCS_CB_DSTRITEM  destroyItem;
	/**
	 * \brief draw an item
	 */
    NCS_CB_DRAWITEM     drawItem;  
} NCS_CB_ITEMOPS;

/**
 * \def NCSS_ITEMV_AUTOSORT
 * \brief Sort items automatically.
 */
#define NCSS_ITEMV_AUTOSORT    (0x0002<<NCSS_SWGT_SHIFT)

/**
 * \def NCSS_ITEMV_LOOP
 * \brief Travel all items circularly.
 */
#define NCSS_ITEMV_LOOP        (0x0004<<NCSS_SWGT_SHIFT)

/**
 * \def NCSS_ITEMV_SINGLE
 * \brief Only have a selected item simultaneously.
 */
#define NCSS_ITEMV_SINGLE      (0x0000<<NCSS_SWGT_SHIFT)

/**
 * \def NCSS_ITEMV_MULTIPLE
 * \brief Allow user to select multiple items. Reserved.
 */
#define NCSS_ITEMV_MULTIPLE    (0x0008<<NCSS_SWGT_SHIFT)

/**
 * \def NCSS_ITEMV_SHIFT
 * \brief The bits used by mItemView in style. 
 */
#define NCSS_ITEMV_SHIFT   (NCSS_SWGT_SHIFT+4)

#define pItemList (self->itemList)

#define mItemViewHeader(className)  \
	mScrollWidgetHeader(className)  \
    NCS_CB_ITEMOPS   itemOps;        \
    mItemManager    *itemList;      \
    int             defItemHeight;

/**
 * \struct mItemView
 * \brief The structure of mItemView control, which derived from
 *        mScrollWidget. It is the abstract superclass of all classes
 *        which represent controls that have items.
 *
 *  - itemOps\n
 *    User-defined operations for items, \a NCS_CB_ITEMOPS.
 *
 *  - itemList\n
 *    The pointer of item manager, \a mItemManager.
 *
 *  - defItemHeight\n
 *    The default height of item.
 */
struct _mItemView
{
	mItemViewHeader(mItemView)
};

#define mItemViewClassHeader(clsName, parentClass)                          \
	mScrollWidgetClassHeader(clsName, parentClass)                          \
	int (*onRButtonDown)(clsName*, int x, int y, DWORD keyFlags);           \
	int (*onRButtonUp)(clsName*, int x, int y, DWORD keyFlags);             \
	int (*onLButtonDBClk)(clsName*, int x, int y, DWORD keyFlags);          \
    NCS_CB_DRAWITEM (*setItemDraw)(clsName*, NCS_CB_DRAWITEM func);         \
    NCS_CB_INITITEM (*setItemInit)(clsName*, NCS_CB_INITITEM func);         \
    NCS_CB_DSTRITEM (*setItemDestroy)(clsName*,                             \
            NCS_CB_DSTRITEM func);                                          \
    \
    NCS_CB_CMPITEM (*setItemCmpFunc)(clsName*, NCS_CB_CMPITEM func);        \
    HWND (*createItem)(clsName*, HITEM prev, HITEM next, int index,         \
            int height, const char* text, DWORD addData, int *pos, BOOL adjust);              \
    int (*insertItem)(clsName*, HITEM hItem, HITEM prev, HITEM next,        \
            int index, int *pos);                                           \
    int (*removeItem)(clsName*, HITEM hItem);                               \
    int (*inItem)(clsName*, int mouseX, int mouseY, HITEM *pRet, POINT *pt);\
    NCS_CB_CMPSTR (*setStrCmpFunc)(clsName*, NCS_CB_CMPSTR func);           \
    NCS_CB_CMPSTR (*getStrCmpFunc)(clsName*);                               \
    void (*setAutoSortItem)(clsName*, BOOL sort);                           \
    int (*getTotalHeight)(clsName*);                                        \
    int (*adjustItemsHeight)(clsName*, int diff);                           \
    void (*freeze)(clsName*, BOOL lock);                                    \
    BOOL (*isFrozen)(clsName*);                                             \
    list_t* (*getQueue)(clsName*);                                          \
    HITEM (*getListEntry)(clsName*, list_t* entry);                         \
    HITEM (*getFirstItem)(clsName*);                                        \
    int (*isEmpty)(clsName*);                                               \
    void (*sortItems)(clsName*, NCS_CB_CMPITEM func);                       \
    BOOL (*removeAll)(clsName*);                                            \
    int (*indexOf)(clsName*, HITEM hItem);                                  \
    int (*setItemHeight)(clsName*, HITEM hItem, int height);                \
    int (*getItemHeight)(clsName*, HITEM hItem);                            \
    HITEM (*getNext)(clsName*, HITEM hItem);                                \
    HITEM (*getPrev)(clsName*, HITEM hItem);                                \
    HITEM (*getItem)(clsName*, int index);                                  \
    BOOL (*isSelected)(clsName*, HITEM hItem);                              \
    BOOL (*isEnabled)(clsName*, HITEM hItem);                               \
    BOOL (*enable)(clsName*, HITEM hItem, BOOL enable);                     \
    void (*selectAll)(clsName*);                                            \
    void (*deselectAll)(clsName*);                                          \
    int (*hilight)(clsName*, HITEM hItem);                                  \
    HITEM (*getHilight)(clsName*);                                          \
    BOOL (*isHilight)(clsName*, HITEM hItem);                               \
    int (*getItemCount)(clsName*);                                          \
    DWORD (*getAddData)(clsName*, HITEM hItem);                             \
    void (*setAddData)(clsName*, HITEM hItem, DWORD addData);               \
    void (*setImage)(clsName*, HITEM hItem, DWORD image);                   \
    DWORD (*getImage)(clsName*, HITEM hItem);                               \
    void (*setFlags)(clsName*, HITEM hItem, DWORD flags);                   \
    DWORD (*getFlags)(clsName*, HITEM hItem);                               \
    BOOL (*setText)(clsName*, HITEM hItem, const char* text);               \
    const char* (*getText)(clsName*, HITEM hItem);                          \
    int (*getTextLen)(clsName*, HITEM hItem);                               \
    int (*getSelectionCount)(clsName*);                                     \
    BOOL (*getSelection)(clsName*, HITEM *pRet, int count);                 \
    \
    BOOL (*select)(clsName*, HITEM hItem);                                  \
    BOOL (*deselect)(clsName*, HITEM hItem);                                \
    BOOL (*setTextByIdx)(clsName*, int index, const char* text);            \
    const char* (*getTextByIdx)(clsName*, int index);                       \
    \
    int (*getFirstVisItem)(clsName*);                                       \
    void (*resetContent)(clsName*);                                         \
    int (*getRect)(clsName*, HITEM hItem, RECT *rcItem, BOOL bConv);        \
    int (*getCurSel)(clsName*);                                             \
    int (*setCurSel)(clsName*, int newSel);                                 \
    void (*refreshItem)(clsName*, HITEM hItem, const RECT *rcInv);          \
    BOOL (*showItemByIdx)(clsName*, int index);             \
    BOOL (*showItem)(clsName*, HITEM hItem);  

/**
 * \struct mItemViewClass
 * \brief The virtual function table of mItemView, which derived from
 *        mScrollWidgetClass.
 *
 *  - \ref NCS_CB_CMPITEM (*\b setItemCmpFunc)(mItemView *self, \ref NCS_CB_CMPITEM func);\n
 *    The function set item's compare function.
 *      \param func The new item's compare function.
 *      \return     The old item's compare function.
 *
 *  - int (*\b moveItem)(mItemView *self, mItem *curItem, int count, mItem *prevItem);\n
 *    The function move the specified items to the specified position.
 *      \param curItem  The handle to first moved item.
 *      \param count    The count of moved items.
 *      \param prevItem The handle to target item.
 *      \return The old compare function.
 *  
 *  - int (*\b insertItem)(mItemView *self, \ref HITEM hItem, \ref HITEM prev,
 *                          \ref HITEM next, int index, int *pos);  \n
 *    The function is used to insert a item to mItemView. If has 
 *    \ref NCSF_ITMMNG_AUTOSORT flag, will insert item to sort position. Otherwise,
 *    the priority of \a prev is higher than \a next, the \a next is higher
 *    than \a index.
 *      \param hItem    The item handle.
 *      \param prev     The specified previous position. 
 *      \param next     The specified next position. 
 *      \param index    The specified insert position.
 *      \param pos      The position in which item has been inserted. It can be NULL.
 *      \return         0 on success, otherwise -1.
 *
 *  - HITEM (*\b createItem)(mItemView *self, \ref HITEM prev, \ref HITEM next, int index, 
 *           int height, const char* text, DWORD addData, int *pos, BOOL adjust); \n
 *    The function creates a new item and inserts it to specified position.
 *      \param prev     The specified previous position. 
 *      \param next     The specified next position. 
 *      \param index    The specified insert position.
 *      \param pos      The position in which item has been inserted. It can be NULL.
 *      \param adjust   Whether adjust total height or not.
 *      \return         The handle to new item.
 *
 *  - int (*\b inItem)(mItemView *self, int mouseX, int mouseY, \ref HITEM *pRet, POINT *pt);\n
 *    The function gets the item that mouse is in it. 
 *      \param mouesX   The mouse x position.
 *      \param mouseY   The mouse y position. 
 *      \param pRet     The handle to current item.
 *      \param pt       The left-top point of item.
 *      \return         The index of item on success, otherwise -1.
 *
 *  - \ref NCS_CB_CMPSTR (*\b setStrCmpFunc)(mItemView *self, \ref NCS_CB_CMPSTR func);\n
 *    The function sets string compare function.
 *      \param func The new string compare function.
 *      \return     The old string compare function.
 *
 *  - \ref NCS_CB_CMPSTR (*\b getStrCmpFunc)(mItemView *self); \n
 *    The function gets current string compare function.
 *
 *  - int (*\b getItemYPos)(mItemView *self, \ref HITEM hItem);\n
 *    The function gets the specified item's starting y position.
 *
 *  - void (*\b setAutoSortItem) (mItemView *self, BOOL sort);\n
 *    The function sets or cancels the \ref NCSF_ITMMNG_AUTOSORT style.
 *
 *  - int (*\b getTotalHeight)(mItemView *self);\n
 *    The function gets the item's total height.
 *
 *  - int (*\b adjustItemsHeight)(mItemView *self, int diff);\n
 *    The function offsets the total height.
 *
 *  - BOOL (*\b freeze)(mItemView *self, BOOL lock);\n
 *    The function is used to freeze the item manager or not. If manager is
 *    frozen, it will not refresh items until unfroze it.
 *
 *  - BOOL (*\b isFrozen)(mItemView *self);\n
 *    The function determines whether manager is frozen.
 *
 *  - \ref HITEM (*\b getFirstItem)(mItemView *self);\n
 *    The function is used to get first item.
 *
 *  - \ref list_t* (*\b getQueue)(mItemView *self);\n
 *    The function is used to get the header of item list.
 *
 *  - \ref HITEM (*\b getListEntry)(\ref list_t* entry);\n
 *    The function is used to get item handle by list entry.
 *
 *  - int (*\b isEmpty)(mItemView *self);\n
 *    The function determines whether manager is empty.
 *
 *  - int (*\b removeItem)(mItemView *self, \ref HITEM hItem);\n
 *    The function is used to remove a item from manager.
 *      \param hItem    The handle to item.
 *      \return         0 on success, otherwise -1.
 *
 *  - int (*\b sortItems)(mItemView *self, \ref NCS_CB_CMPITEM pfn);\n
 *    The function sorts items that uses the specified compare function.
 *
 *  - BOOL (*\b removeAll)(mItemView *self);\n
 *    The function deletes all items.
 *
 *  - int (*\b indexOf)(mItemView *self, \ref HITEM hItem);\n
 *    The function gets the index of the specifed item.
 *      \param hItem    The handle to item.
 *      \return         The item index on success, otherwise -1.
 *
 *  - int (*\b setHeight)(mItemView *self, \ref HITEM hItem, int height);\n
 *    The function sets the height of the specified item.
 *      \param hItem    The handle to item.
 *      \param height   The new height of item.
 *      \return         The difference value in old and new height 
 *                      on success, otherwise -1.
 *
 *  - int (*\b getHeight)(mItemView *self, \ref HITEM hItem);\n
 *    The function gets the height of the specified item.
 *
 *  - \ref HITEM (*\b getItem)(mItemView *self, int index);\n
 *    The function gets the item handle of the specified index.
 *      \param index  The specified index.
 *      \return       The handle to item on success, otherwise NULL.
 *
 *  - \ref HITEM (*\b getPrev)(mItemView *self, \ref HITEM hItem);\n
 *    The function gets the previous item.
 *      \param hItem    The handle to the specified item.
 *      \return         The handle to previous item on success, otherwise NULL.
 *
 *  - HITEM (*\b getNext)(mItemView *self, \ref HITEM hItem);\n
 *    The function gets the next item.
 *      \param hItem    The handle to the specified item.
 *      \return         The handle to next item on success, otherwise NULL.
 *
 *  - BOOL (*\b isSelected)(mItemView *self, \ref HITEM hItem);\n
 *    The function determines whether the specified item is selected or not.
 *
 *  - BOOL (*\b isEnabled)(mItemView *self, \ref HITEM hItem);\n
 *    The function determines whether the specified item is enabled or not.
 *
 *  - BOOL (*\b enable)(mItemView *self, \ref HITEM hItem, BOOL enable);\n
 *    The function enables the specified item or not.
 *      \param hItem    The handle to the specified item.
 *      \param enable   The enable status:TRUE for enable, FALSE for disable.
 *      \return         TRUE on success, otherwise FALSE.
 *
 *  - void (*\b selectAll)(mItemView *self);\n
 *    The function selects all items.
 *
 *  - void (*\b deselectAll)(mItemView *self);\n
 *    The function deselects all items.
 *
 *  - int (*\b hilight)(mItemView *self, \ref HITEM hItem);\n
 *    The function hilights the specified item. 
 *
 *  - \ref HITEM (*\b getHilight)(mItemView *self);\n
 *    The function gets the hilighted item.
 *      \return     The handle to old hilighted item on success, otherwise 0.
 *
 *  - BOOL (*\b isHilight)(mItemView *self, \ref HITEM hItem);\n
 *    The function determines whether the specified item is hilighted or not.
 *
 *  - int (*\b getItemCount)(mItemView *self);\n
 *    The function is used to get the number of items.
 *      \return       The number of items.
 *
 *  - void (*\b setAddData)(mItemView *self, \ref HITEM hItem, DWORD addData);\n
 *    The function sets the additional data of the specified item.
 *      \param hItem    The handle to the specified item.
 *      \param addData  The new additional data.
 *
 *  - void (*\b setImage)(mItemView *self, \ref HITEM hItem, DWORD image);\n
 *    The function sets the image of the specified item.
 *      \param hItem    The handle to the specified item.
 *      \param image    The new image.
 *
 *  - DWORD (*\b getAddData)(mItemView *self, \ref HITEM hItem);\n
 *    The function gets the additional data of the specified item.
 *
 *  - DWORD (*\b getImage)(mItemView *self, \ref HITEM hItem);\n
 *    The function gets the image of the specified item.
 *
 *  - void (*\b setFlags)(mItemView *self, \ref HITEM hItem, DWORD flags);\n
 *    The function sets the flags of the specified item.
 *      \param hItem    The handle to the specified item.
 *      \param flags    The new flags.
 *
 *  - DWORD (*\b getFlags)(mItemView *self, \ref HITEM hItem);\n
 *    The function gets the flags of the specified item.
 *
 *  - BOOL (*\b setText)(mItemView *self, \ref HITEM hItem, const char* text);\n
 *    The function sets the text of the specified item.
 *      \param hItem    The handle to item.
 *      \param text     The next text.
 *      \return         TRUE on success, otherwise FALSE.
 *
 *  - const char* (*\b getText)(mItemView *self, \ref HITEM hItem);\n
 *    The function gets the text of the specified item.
 *      \param hItem    The handle to item.
 *      \return         The text.
 *
 *  - int (*\b getTextLen)(mItemView *self, \ref HITEM hItem);\n
 *    The function gets the text length of the specified item.
 *      \param hItem    The handle to item.
 *      \return         The text length.
 *
 *  - BOOL (*\b getSelection)(mItemView *self, \ref HITEM *pRet, int count);\n
 *    Get the array of item handles for the selected items.
 *      \param pRet     The array of handles.
 *      \param count    The maximum size of array.
 *      \return         TRUE on success, otherwise FALSE.
 *
 *  - int (*\b getSelectionCount)(mItemView *self); \n
 *    The function gets the number of selected items.
 *
 *  - BOOL (*\b select)(mItemView *self, \ref HITEM hItem);\n
 *    The function selects the specified item.
 *      \param hItem    The handle to the specified item.
 *      \return         TRUE on success, otherwise FALSE.
 *
 *  - BOOL (*\b deselect)(mItemView *self, \ref HITEM hItem);\n
 *    The function deselects the specified item.
 *      \param hItem    The handle to the specified item.
 *      \return         TRUE on success, otherwise FALSE.
 *
 *  - \ref NCS_CB_DRAWITEM (*\b setItemDraw)(mItemView *self, \ref NCS_CB_DRAWITEM func);\n
 *    Sets the user-defined function of drawing item.
 *      \param func     The new funciton.
 *      \return         The old function.
 *
 *  - \ref NCS_CB_INITITEM (*\b setItemInit)(mItemView *self, \ref NCS_CB_INITITEM func);\n
 *    Sets the user-defined function of initializing item.
 *      \param func     The new funciton.
 *      \return         The old function.
 *
 *  - \ref NCS_CB_DSTRITEM (*\b setItemDestroy)(mItemView *self, \ref NCS_CB_DSTRITEM func);\n
 *    Sets the user-defined function of destroying item.
 *      \param func     The new funciton.
 *      \return         The old function.
 *
 *  - int (*\b getFirstVisItem)(mItemView *self);\n
 *    Gets the first visible item index.
 *
 *  - BOOL (*\b setTextByIdx)(mItemView *self, int index, const char* text);\n
 *    Sets text of item by index.
 *      \param index    The index of item.
 *      \return         TRUE on success, FALSE for failure.
 *
 *  - const char* (*\b getTextByIdx)(mItemView *self, int index);\n
 *    Gets text of item by index.
 *      \param index    The index of item.
 *      \return         The string of item on success, otherwise NULL.
 *
 *  - int (*\b getRect)(mItemView *self, \ref HITEM hItem, RECT *rcItem, BOOL bConv);\n
 *    Get the specified item's rectangle.
 *      \param hItem    The handle to the specified item.
 *      \param index    The index of item.
 *      \return         0 on success, otherwise -1.
 *
 *  - void (*\b refreshItem)(mItemView *self, \ref HITEM hItem, const RECT *rcInv);\n
 *    Refresh the specified item.
 *      \param hItem    The handle to the specified item.
 *      \param rcInv    The pointer to a RECT structure which defines the 
 *                      invalid rectangle. NULL for the whole item.
 *
 *  - BOOL (*\b showItemByIdx)(mItemView *self, int index);\n
 *    Show item by index.
 *
 *  - BOOL (*\b showItem)(mItemView *self, \ref HITEM hItem);\n
 *    Shows the specified item.
 *
 *  - int (*\b setCurSel)(mItemView *self, int newSel);\n
 *    Selects an item in a multiple-selection list box and scroll it into viewport.
 *      \param newSel   The index of the item.
 *      \param flag     Indicates the changes to be made to the listbox item,
 *                      can be one of the following values:
 *                          - -1\n    If the item has been selected, makes it 
 *                                      unselected, vice versa.
 *                          - 0\n     Makes the item unselected. 
 *                          - other\n Makes the item selected. 
 *
 *      \return         0 on success, else -1 to indicates you passed an invalid
 *                      index or the list box has no \ref NCSS_ITEMV_MULTIPLE style.
 *        
 *  - int (*\b getCurSel)(mItemView *self);\n
 *    Get the index of current selected item.
 *      \return         The index of current selected item on success, 
 *                      otherwise -1.
 *
 *  - void (*\b resetContent)(mItemView *self); \n
 *      Reset all property and remove all items.
 */

struct _mItemViewClass
{
	mItemViewClassHeader(mItemView, mScrollWidget)
};

#define mItemViewRendererHeader(clsName, parentClass) \
	mScrollWidgetRendererHeader(clsName, parentClass)

/**
 * \struct mItemViewRenderer
 * \brief The structure of mItemView renderer, which inheried
 *        from mScrollWidgetRenderer.
 */
struct  _mItemViewRenderer {
	mItemViewRendererHeader(mItemView, mScrollWidget)
};

/**
 * \var g_stmItemViewCls
 * \brief The instance of mItemViewClass.
 *
 * \sa mItemViewClass
 */
MGNCS_EXPORT extern mItemViewClass g_stmItemViewCls;

/**
 * \enum mItemViewProp
 * \brief The properties id of mItemView.
 */
enum mItemViewProp
{
    /**
     * The default item height.
     */
	NCSP_ITEMV_DEFITEMHEIGHT = NCSP_SWGT_MAX + 1,

    /**
     * The number of items.
     */
    NCSP_ITEMV_ITEMCOUNT,
    /**
     * The maximum value of mItemView properties id.
     */
    NCSP_ITEMV_MAX,
};

/**
 * \enum mItemViewNotify
 * \brief The notification code id of mItemView.
 */
enum mItemViewNotify {
    /**
     * Mouse click.
     */
    NCSN_ITEMV_CLICKED = NCSN_WIDGET_CLICKED,

    /**
     * Changing selected item.
     */
    NCSN_ITEMV_SELCHANGING = NCSN_SWGT_MAX + 1,

    /**
     * Selected item changed.
     */
    NCSN_ITEMV_SELCHANGED,

    /**
     * ENTER pressed.
     */
    NCSN_ITEMV_ENTER,

    /**
     * Get focus.
     */
    NCSN_ITEMV_SETFOCUS,

    /**
     * Lost focus.
     */
    NCSN_ITEMV_KILLFOCUS,

    /**
     * The maximum value of mItemView notification code id.
     */
    NCSN_ITEMV_MAX
};

    /** @} end of Widget_ItemView */
#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif /* _MGUI_CTRL_ITEMVIEW_H */
