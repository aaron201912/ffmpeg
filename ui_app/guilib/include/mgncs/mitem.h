/**
 * \file mitem.h
 * \author XiaoweiYan
 * \date 2009/02/24
 *
 * This file includes the definition of mItem.
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


#ifndef _MGUI_WIDGET_ITEM_H
#define _MGUI_WIDGET_ITEM_H
 

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

/**
 * \defgroup Widget_Item mItem
 * @{
 */

typedef struct _mItem mItem;
typedef struct _mItemClass mItemClass;

/**
 * \var HITEM
 * \brief The handle of item widget.
 */
typedef GHANDLE HITEM;

/**
 * \def hEditor
 * \brief The handle of item editor.
 */
#define hEditor HWND

/**
 * \def NCSF_ITEM_SHIFT
 * \brief The bits used by mItem in flags.
 */
#define NCSF_ITEM_SHIFT             4

/**
 * \def NCSF_ITEM_MASK
 * \brief The mask of item flags.
 */
#define NCSF_ITEM_MASK              0xFFFF

/**
 * \def NCSF_ITEM_NORMAL
 * \brief Normal item.
 */
#define NCSF_ITEM_NORMAL            0x0000

/**
 * \def NCSF_ITEM_SELECTED
 * \brief Selected item.
 */
#define NCSF_ITEM_SELECTED          0x0001

/**
 * \def NCSF_ITEM_DISABLED
 * \brief Disabled item.
 */
#define NCSF_ITEM_DISABLED          0x0002

/**
 * \def NCSF_ITEM_USEBITMAP
 * \brief The item with bitmap.
 */
#define NCSF_ITEM_USEBITMAP         0x0004

/**
 * \def NCSF_ITEM_USEICON
 * \brief The item with icon.
 */
#define NCSF_ITEM_USEICON           0x0008

/**
 * \def NCSF_ITEM_IMAGEMASK
 * \brief The mask of item image/icon style.
 */
#define NCSF_ITEM_IMAGEMASK         (NCSF_ITEM_USEBITMAP | NCSF_ITEM_USEICON)

#define mItemHeader(className)  \
	mObjectHeader(className)    \
    list_t  list;               \
    list_t  selList;            \
    DWORD   flags;              \
    char    *string;            \
    DWORD   image;              \
    hEditor editor;             \
    int     height;             \
    DWORD   addData;

/**
 * \struct mItem
 * \brief The structure of mItem. It is the abstract superclass of all items 
 *        occur within specific controls. For example, a listbox will contain 
 *        string or bitmap items. 
 *
 *      - list\n
 *        The item list header. \n
 *
 *      - selList\n
 *        The selected item list header. \n
 *
 *      - string\n
 *        The pointer of string. \n
 *
 *      - image\n
 *        The pointer of image. \n
 *
 *      - editor\n
 *        The handle of item editor. \n
 *
 *      - flags\n
 *        The flags of item. It can be OR'ed by the following values:\n
 *          - NCSF_ITEM_NORMAL \n
 *            Normal status. \n
 *          - NCSF_ITEM_SELECTED \n
 *            Selected status. \n
 *          - NCSF_ITEM_DISABLED \n
 *            Disabled status. \n
 *          - NCSF_ITEM_USEBITMAP \n
 *            Use bitmap. \n
 *          - NCSF_ITEM_USEICON\n
 *            Use icon. \n
 *
 *      - addData\n
 *        The additional data.
 */
struct _mItem
{
	mItemHeader(mItem)
};

#define mItemClassHeader(clsName, superCls)             \
	mObjectClassHeader(clsName, superCls)               \
	BOOL (*setProperty)(clsName*, int id, DWORD value); \
	DWORD (*getProperty)(clsName*, int id);             \
    BOOL (*isSelectedItem)(clsName*);                   \
    BOOL (*isEnabledItem)(clsName*);                    \
    void (*setItemEditor)(clsName*, hEditor editor);    \
    hEditor (*getItemEditor)(clsName*);                 \
    void (*setItemAddData)(clsName*, DWORD addData);    \
    DWORD (*getItemAddData)(clsName*);                  \
    void (*setItemImage)(clsName*, DWORD image);        \
    DWORD (*getItemImage)(clsName*);                    \
    void (*setItemFlags)(clsName*, DWORD flags);        \
    DWORD (*getItemFlags)(clsName*);                    \
    BOOL (*setItemHeight)(clsName*, int height);        \
    int (*getItemHeight)(clsName*);                     \
    BOOL (*setItemString)(clsName*, const char* string);\
    char* (*getItemString)(clsName*);

/**
 * \struct mItemClass
 * \brief The virtual function table of mItemClass.
 *
 *      - BOOL (*\b setProperty)(mItem *self, int id, DWORD value);\n
 *        The function sets the value of the specified properties.
 *          \param id         The property id.
 *          \param value      The property value.
 *          \return TRUE on success, otherwise FALSE. 
 *
 *      - DWORD (*\b getProperty)(mItem *self, int id); \n
 *        The function gets the value of the specified properties. 
 *          \param id         The property id. 
 *          \return the property value on success, otherwise -1. 
 *
 *      - BOOL (*\b isSelectedItem)(mItem *self);\n
 *        The function is used to decide whether the item is selected or not. 
 *          \return TRUE for selected status, otherwise FALSE. 
 *
 *      - BOOL (*\b isEnabledItem)(mItem *self);\n
 *        The function is used to terminate whether the item is enabled or not. 
 *          \return TRUE for enabled status, otherwise FALSE. 
 *
 *      - void (*\b setItemEditor)(mItem *self, \ref hEditor editor); \n
 *        The function is used to set the item editor. 
 *          \param editor     The handle of editor. 
 *
 *      - hEditor (*\b getItemEditor)(mItem *self);\n
 *        The function is used to get the item editor. 
 *          \return The editor handle on success, otherwise HWND_NULL.
 *
 *      - void (*\b setItemAddData)(mItem *self, DWORD addData);\n
 *        The function is used to set the addtional data. 
 *          \param addData    The additional data. 
 *      
 *      - DWORD (*\b getItemAddData)(mItem *self);\n
 *        The function is used to get the additional data. 
 *          \return The additional data. 
 *
 *      - void (*\b setItemImage)(mItem *self, DWORD image);\n
 *        The function is used to set the item image. 
 *          \param addData    The handle of image. 
 *      
 *      - DWORD (*\b getItemImage)(mItem *self);\n
 *        The function is used to get the item image. 
 *          \return The pointer of image, otherwise 0.
 *
 *      - BOOL (*\b setItemString)(mItem *self, char* string);\n
 *        The function is used to set the item string. 
 *          \param string     The item text.
 *          \return TRUE on success, otherwise FALSE.
 *
 *      - char* (*\b getItemString)(mItem *self); \n
 *        The function is used to get the item string.
 *          \return The item text, otherwise NULL. 
 *
 *      - void (*\b setItemFlags)(mItem *self, DWORD flags);\n
 *        The function is used to set the item flags.
 *          \param flags    The item flags.
 *
 *      - DWORD (*\b getItemFlags)(mItem *self);\n
 *        The function is used to get the item flags.
 *
 *      - BOOL (*\b setItemHeight)(mItem *self, int height);\n
 *        The function is used to set the item height.
 *          \param height   The item height.
 *          \return TRUE on success, otherwise FALSE.
 *
 *      - int (*\b getItemHeight)(mItem *self);\n
 *        The function is used to get the item height.
 */
struct _mItemClass
{
	mItemClassHeader(mItem, mObject)
};

/**
 * \enum mItemProp
 * \brief The properties id of mItem.
 */
enum mItemProp
{
    /**
     * The height of item.
     */
    NCSP_ITEM_HEIGHT = 1,

    /**
     * The flags of item.
     */
    NCSP_ITEM_FLAGS,

    /**
     * The maximum value of mItem properties id.
     */
	NCSP_ITEM_MAX,
};

/**
 * \var g_stmItemCls
 * \brief The instance of mItemClass.
 *
 * \sa mItemClass
 */
MGNCS_EXPORT extern mItemClass g_stmItemCls;

    /** @} end of Widget_Item */

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif /* _MGUI_WIDGET_ITEM_H */

