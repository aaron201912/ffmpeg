/**
 * \file mitem_manager.h
 * \author XiaoweiYan
 * \date 2009/02/24
 *
 * This file includes the definition of ItemManager.
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

#ifndef _MGUI_CTRL_ITEMMANAGER_H
#define _MGUI_CTRL_ITEMMANAGER_H

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

/**
 * \defgroup Widget_ItemManager mItemManager
 * @{
 */
typedef struct _mItemManager mItemManager;
typedef struct _mItemManagerClass mItemManagerClass;
typedef struct _mItemManagerRenderer mItemManagerRenderer;

/**
 * \var typedef int (*NCS_CB_CMPITEM) (mItemManager *manager, \
 *                              HITEM hItem1, HITEM hItem2);
 * \brief The callback of item compare function.
 */
typedef int (*NCS_CB_CMPITEM) (mItemManager *manager, HITEM hItem1, HITEM hItem2);

/**
 * \var typedef int (*NCS_CB_CMPSTR) (const char* s1, const char* s2, size_t n);
 * \brief The Callback of general string compare function.
 *
 * The function compares the two strings \a s1 and \a s2. It returns
 * an integer less than, equal to, or greater than zero if  \a s1 is found,
 * respectively, to be less than, to match, or be greater than \a s2.
 *
 * \note It only compares the first (at most) \a n characters of s1 and s2. 
 */
typedef int (*NCS_CB_CMPSTR) (const char* s1, const char* s2, size_t n);

/**
 * \def NCSF_ITMMNG_AUTOSORT
 * \brief Sort items automatically.
 */
#define NCSF_ITMMNG_AUTOSORT        (0x01<<NCSF_ITEM_SHIFT)

/**
 * \def NCSF_ITMMNG_FROZEN
 * \brief Allow manager to refresh items or not.
 */
#define NCSF_ITMMNG_FROZEN          (0x02<<NCSF_ITEM_SHIFT)

/**
 * \def NCSF_ITMMNG_SHIFT
 * \brief The bits used by mItemManager in flags.
 */
#define NCSF_ITMMNG_SHIFT           (NCSF_ITEM_SHIFT+2)


#define mItemManagerHeader(className)   \
    mItemHeader(className)              \
    NCS_CB_CMPITEM  itemCmp;            \
    mWidget *obj;                       \
    int     count;                      \
    int     totalHeight;                \
    HITEM   pItemHilighted;             \
    list_t  queue;                      \
    list_t  selQueue;                   \
    NCS_CB_CMPSTR  strCmp;                     

/**
 * \struct mItemManager
 * \brief The structure of mItemManager control, which derived from mItem. 
 *      It is the abstract superclass of all classes which managers items.
 *
 *      - host \n
 *      The window handle which own the item manager.
 *
 *      - itemCmp \n
 *      The pointer to item comparison function.
 *      
 *      - strCmp \n
 *      The pointer to string comparison funciton.
 *
 *      - totalHeight\n
 *      The total height of items.
 *
 *      - pItemHilighted \n
 *      The pointer to the hilighted item.
 *
 *      - queue \n
 *      The list of items.
 *
 *      - selQueue \n
 *      The list of selected items.
 *
 *      - count \n
 *      The count of item.
 */
struct _mItemManager
{
	mItemManagerHeader(mItemManager)
};

#define mItemManagerClassHeader(clsName, superCls) \
	mItemClassHeader(clsName, superCls)            \
    NCS_CB_CMPITEM (*setItemCmpFunc)(clsName*, NCS_CB_CMPITEM func);        \
    int (*moveItem)(clsName*, mItem *curItem, int count, mItem* prevItem);  \
    int (*insertItem)(clsName*, HITEM hItem, HITEM prev, HITEM next, int index, int *pos);  \
    HITEM (*createItem)(clsName*, HITEM prev, HITEM next, int index, int *pos); \
    int (*inItem)(clsName*, int mouseX, int mouseY, HITEM *pRet, POINT *pt);    \
    NCS_CB_CMPSTR (*setStrCmpFunc)(clsName*, NCS_CB_CMPSTR func);               \
    NCS_CB_CMPSTR (*getStrCmpFunc)(clsName*);                   \
    int (*getItemYPos)(clsName*, HITEM hItem);                  \
    void (*setAutoSortItem) (clsName*, BOOL sort);              \
    int (*getTotalHeight)(clsName*);                            \
    int (*adjustItemsHeight)(clsName*, int diff);               \
    BOOL (*freeze)(clsName*, BOOL lock);                        \
    BOOL (*isFrozen)(clsName*);                                 \
    HITEM (*getFirstItem)(clsName*);                            \
    list_t* (*getQueue)(clsName*);                              \
    HITEM (*getListEntry)(list_t* entry);                       \
    int (*isEmpty)(clsName*);                                   \
    int (*removeItem)(clsName*, HITEM hItem);                   \
    int (*sortItems)(clsName*, NCS_CB_CMPITEM pfn);             \
    BOOL (*removeAll)(clsName*);                                \
    int (*indexOf)(clsName*, HITEM hItem);                      \
    int (*setHeight)(clsName*, HITEM hItem, int height);        \
    int (*getHeight)(clsName*, HITEM hItem);                    \
    HITEM (*getItem)(clsName*, int index);                      \
    HITEM (*getNext)(clsName*, HITEM hItem);                    \
    HITEM (*getPrev)(clsName*, HITEM hItem);                    \
    BOOL (*isSelected)(clsName*, HITEM hItem);                  \
    BOOL (*isEnabled)(clsName*, HITEM hItem);                   \
    BOOL (*enable)(clsName*, HITEM hItem, BOOL enable);         \
    void (*selectAll)(clsName*);                                \
    void (*deselectAll)(clsName*);                              \
    BOOL (*select)(clsName*, HITEM hItem, BOOL sel);            \
    HITEM (*hilight)(clsName*, HITEM hItem);                    \
    HITEM (*getHilight)(clsName*);                              \
    BOOL (*isHilight)(clsName*, HITEM hItem);                   \
    int (*getItemCount)(clsName*);                              \
    void (*setAddData)(clsName*, HITEM hItem, DWORD addData);   \
    void (*setImage)(clsName*, HITEM hItem, DWORD image);       \
    DWORD (*getAddData)(clsName*, HITEM hItem);                 \
    DWORD (*getImage)(clsName*, HITEM hItem);                   \
    void (*setFlags)(clsName*, HITEM hItem, DWORD flags);       \
    DWORD (*getFlags)(clsName*, HITEM hItem);                   \
    BOOL (*setText)(clsName*, HITEM hItem, const char* text);   \
    const char* (*getText)(clsName*, HITEM hItem);              \
    int (*getTextLen)(clsName*, HITEM hItem);                   \
    BOOL (*getSelection)(clsName*, HITEM *pRet, int count);     \
    int (*getSelectionCount)(clsName*);

/**
 * \struct mItemManagerClass
 * \brief The virtual function table of mItemManager, which derived from
 *        mItemClass.
 *
 *  - \ref NCS_CB_CMPITEM (*\b setItemCmpFunc)(mItemManager *self, \ref NCS_CB_CMPITEM func);\n
 *    The function set item's comparison function.
 *      \param func The new item's comparison function.
 *      \return     The old item's comparison function.
 *
 *  - int (*\b moveItem)(mItemManager *self, mItem *curItem, int count, mItem* prevItem);\n
 *    The function move the specified items to the specified position.
 *      \param curItem  The handle to first moved item.
 *      \param count    The count of moved items.
 *      \param prevItem The handle to target item.
 *      \return The old comparison function.
 *  
 *  - int (*\b insertItem)(mItemManager *self, \ref HITEM hItem, \ref HITEM prev,
 *                          \ref HITEM next, int index, int *pos);  \n
 *    The function is used to insert a item to mItemManager. If has 
 *   \ref NCSF_ITMMNG_AUTOSORT flag, will insert item to sort position. Otherwise,
 *   the priority of \a prev is higher than \a next, the \a next is higher
 *   than \a index.
 *      \param hItem    The item handle.
 *      \param prev     The specified previous position. 
 *      \param next     The specified next position. 
 *      \param index    The specified insert position.
 *      \param pos      The position in which item has been inserted. It can be NULL.
 *      \return         0 on success, otherwise -1.
 *
 *  - \ref HITEM (*\b createItem)(mItemManager *self, \ref HITEM prev, \ref HITEM next, int index, int *pos);\n
 *    The function creates a new item and inserts it to specified position.
 *      \param prev     The specified previous position. 
 *      \param next     The specified next position. 
 *      \param index    The specified insert position.
 *      \param pos      The position in which item has been inserted. It can be NULL.
 *      \return         The handle to new item.
 *
 *  - int (*\b inItem)(mItemManager *self, int mouseX, int mouseY, \ref HITEM *pRet, POINT *pt);\n
 *    The function gets the item that mouse is in it. 
 *      \param mouesX   The mouse x position.
 *      \param mouseY   The mouse y position. 
 *      \param pRet     The handle to current item.
 *      \param pt       The left-top point of item.
 *      \return         The index of item on success, otherwise -1.
 *
 *  - \ref NCS_CB_CMPSTR (*\b setStrCmpFunc)(mItemManager *self, \ref NCS_CB_CMPSTR func);\n
 *    The function sets string comparison function.
 *      \param func The new string comparison function.
 *      \return     The old string comparison function.
 *
 *  - NCS_CB_CMPSTR (*\b getStrCmpFunc)(mItemManager *self); \n
 *    The function gets current string comparison function.
 *
 *  - int (*\b getItemYPos)(mItemManager *self, \ref HITEM hItem);\n
 *    The function gets the specified item's starting y position.
 *
 *  - void (*\b setAutoSortItem) (mItemManager *self, BOOL sort);\n
 *    The function sets or cancels the \ref NCSF_ITMMNG_AUTOSORT style.
 *
 *  - int (*\b getTotalHeight)(mItemManager *self);\n
 *    The function gets the item's total height.
 *
 *  - int (*\b adjustItemsHeight)(mItemManager *self, int diff);\n
 *    The function offsets the total height.
 *
 *  - BOOL (*\b freeze)(mItemManager *self, BOOL lock);\n
 *    The function is used to freeze the item manager or not. If manager is
 *    frozen, it will not refresh items until unfroze it.
 *
 *  - BOOL (*\b isFrozen)(mItemManager *self);\n
 *    The function determines whether manager is frozen.
 *
 *  - \ref HITEM (*\b getFirstItem)(mItemManager *self);\n
 *    The function is used to get first item.
 *
 *  - list_t* (*\b getQueue)(mItemManager *self);\n
 *    The function is used to get the header of item list.
 *
 *  - \ref HITEM (*\b getListEntry)(list_t* entry);\n
 *    The function is used to get item handle by list entry.
 *
 *  - int (*\b isEmpty)(mItemManager *self);\n
 *    The function determines whether manager is empty.
 *
 *  - int (*\b removeItem)(mItemManager *self, \ref HITEM hItem);\n
 *    The function is used to remove a item from manager.
 *      \param hItem    The handle to item.
 *      \return         0 on success, otherwise -1.
 *
 *  - int (*\b sortItems)(mItemManager *self, \ref NCS_CB_CMPITEM pfn);\n
 *    The function sorts items that uses the specified comparison function.
 *
 *  - BOOL (*\b removeAll)(mItemManager *self);\n
 *    The function deletes all items.
 *
 *  - int (*\b indexOf)(mItemManager *self, \ref HITEM hItem);\n
 *    The function gets the index of the specifed item.
 *      \param hItem    The handle to item.
 *      \return         The item index on success, otherwise -1.
 *
 *  - int (*\b setHeight)(mItemManager *self, \ref HITEM hItem, int height);\n
 *    The function sets the height of the specified item.
 *      \param hItem    The handle to item.
 *      \param height   The new height of item.
 *      \return         The difference value in old and new height 
 *                      on success, otherwise -1.
 *
 *  - int (*\b getHeight)(mItemManager *self, \ref HITEM hItem);\n
 *    The function gets the height of the specified item.
 *
 *  - \ref HITEM (*\b getItem)(mItemManager *self, int index);\n
 *    The function gets the item handle of the specified index.
 *      \param index  The specified index.
 *      \return       The handle to item on success, otherwise NULL.
 *
 *  - \ref HITEM (*\b getNext)(mItemManager *self, \ref HITEM hItem);\n
 *    The function gets the previous item.
 *      \param hItem    The handle to the specified item.
 *      \return         The handle to previous item on success, otherwise NULL.
 *
 *  - \ref HITEM (*\b getPrev)(mItemManager *self, \ref HITEM hItem);\n
 *    The function gets the next item.
 *      \param hItem    The handle to the specified item.
 *      \return         The handle to next item on success, otherwise NULL.
 *
 *  - BOOL (*\b isSelected)(mItemManager *self, \ref HITEM hItem);\n
 *    The function determines whether the specified item is selected or not.
 *
 *  - BOOL (*\b isEnabled)(mItemManager *self, \ref HITEM hItem);\n
 *    The function determines whether the specified item is enabled or not.
 *
 *  - BOOL (*\b enable)(mItemManager *self, \ref HITEM hItem, BOOL enable);\n
 *    The function enables the specified item or not.
 *      \param hItem    The handle to the specified item.
 *      \param enable   The enable status:TRUE for enable, FALSE for disable.
 *      \return         TRUE on success, otherwise FALSE.
 *
 *  - void (*\b selectAll)(mItemManager *self);\n
 *    The function selects all items.
 *
 *  - void (*\b deselectAll)(mItemManager *self);\n
 *    The function deselects all items.
 *
 *  - BOOL (*\b select)(mItemManager *self, \ref HITEM hItem, BOOL sel);\n
 *    The function selects the specified item or not.
 *      \param hItem    The handle to the specified item.
 *      \param sel      The selected status:TRUE for select, FALSE for deselect.
 *      \return         TRUE on success, otherwise FALSE.
 *
 *  - \ref HITEM (*\b hilight)(mItemManager *self, \ref HITEM hItem);\n
 *    The function hilights the specified item. 
 *
 *  - \ref HITEM (*\b getHilight)(mItemManager *self);\n
 *    The function gets the hilighted item.
 *      \return     The handle to old hilighted item on success, otherwise 0.
 *
 *  - BOOL (*\b isHilight)(mItemManager *self, \ref HITEM hItem);\n
 *    The function determines whether the specified item is hilighted or not.
 *
 *  - int (*\b getItemCount)(mItemManager *self);\n
 *    The function is used to get the number of items.
 *      \return       The number of items.
 *
 *  - void (*\b setAddData)(mItemManager *self, \ref HITEM hItem, DWORD addData);\n
 *    The function sets the additional data of the specified item.
 *      \param hItem    The handle to the specified item.
 *      \param addData  The new additional data.
 *
 *  - void (*\b setImage)(mItemManager *self, \ref HITEM hItem, DWORD image);\n
 *    The function sets the image of the specified item.
 *      \param hItem    The handle to the specified item.
 *      \param image    The new image.
 *
 *  - DWORD (*\b getAddData)(mItemManager *self, \ref HITEM hItem);\n
 *    The function gets the additional data of the specified item.
 *
 *  - DWORD (*\b getImage)(mItemManager *self, \ref HITEM hItem);\n
 *    The function gets the image of the specified item.
 *
 *  - void (*\b setFlags)(mItemManager *self, \ref HITEM hItem, DWORD flags);\n
 *    The function sets the flags of the specified item.
 *      \param hItem    The handle to the specified item.
 *      \param flags    The new flags.
 *
 *  - DWORD (*\b getFlags)(mItemManager *self, \ref HITEM hItem);\n
 *    The function gets the flags of the specified item.
 *
 *  - BOOL (*\b setText)(mItemManager *self, \ref HITEM hItem, const char* text);\n
 *    The function sets the text of the specified item.
 *      \param hItem    The handle to item.
 *      \param text     The next text.
 *      \return         TRUE on success, otherwise FALSE.
 *
 *  - const char* (*\b getText)(mItemManager *self, \ref HITEM hItem);\n
 *    The function gets the text of the specified item.
 *      \param hItem    The handle to item.
 *      \return         The text.
 *
 *  - int (*\b getTextLen)(mItemManager *self, \ref HITEM hItem);\n
 *    The function gets the text length of the specified item.
 *      \param hItem    The handle to item.
 *      \return         The text length.
 *
 *  - BOOL (*\b getSelection)(mItemManager *self, \ref HITEM *pRet, int count);\n
 *    Get the array of item handles for the selected items.
 *      \param pRet     The array of handles.
 *      \param count    The maximum size of array.
 *      \return         TRUE on success, otherwise FALSE.
 *
 *  - int (*\b getSelectionCount)(mItemManager *self); \n
 *    The function gets the number of selected items.
 */
struct _mItemManagerClass
{
	mItemManagerClassHeader(mItemManager, mItem)
};

/**
 * \var g_stmItemManagerCls
 * \brief The instance of mItemManagerClass.
 *
 * \sa mItemManagerClass
 */
MGNCS_EXPORT extern mItemManagerClass g_stmItemManagerCls;

/**
 * \enum mItemManagerProp
 * \brief The properties id of mItemManager.
 */
enum mItemManagerProp
{
    /**
     * The flags of item manager.
     */
    NCSP_ITMMNG_FLAGS = NCSP_ITEM_FLAGS,

    /**
     * The number of items. Read only.
     */
    NCSP_ITMMNG_ITEMCOUNT = NCSP_ITEM_MAX + 1,

    /**
     * The total height. Read only.
     */
    NCSP_ITMMNG_TOTALHEIGHT,

    /**
     * The maximum value of mItemManager properties id.
     */
	NCSP_ITMMNG_MAX,
};
    /** @} end of Widget_ItemManager */

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif /* _MGUI_CTRL_ITEMMANAGER_H */
