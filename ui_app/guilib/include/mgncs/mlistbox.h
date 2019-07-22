/**
 * \file mlistbox.h
 * \author XiaoweiYan
 * \date 2009/02/24
 *
 * This file includes the definition of ListBox.
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

#ifdef _MGNCSCTRL_LISTBOX

#ifndef _MGUI_CTRL_LSTBOX_H
#define _MGUI_CTRL_LSTBOX_H


#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#include "mitem.h"
#include "mitem_manager.h"
#include "mitemview.h"
/**
 * \defgroup Widget_ListBox mListBox
 * @{
 */

/**
 * The structure of listbox item information.
 */
typedef struct _NCS_LSTBOX_ITEMINFO
{
    /**
     * The text string.
     */
    char*   string;

    /**
     * Check mark and image flag. It can be one of the following values:
     * - \ref NCSF_LSTBOX_CMBLANK
     *   The item is blank.
     * - \ref NCSF_LSTBOX_CMCHECKED
     *   The item is checked.
     * - \ref NCSF_LSTBOX_CMPARTCHECKED
     *   The item is partly checked.
     *
     * If you want to significant the item which to be rendered,
     * you can OR'd \a flag with \ref NCSF_LSTBOX_SIGNIFICANT.
     *
     * For \ref NCSS_LSTBOX_USEBITMAP list box, if you want to display bitmap
     * other than icon, you can OR'd \a flag with \ref NCSF_ITEM_USEBITMAP.
     */
    DWORD   flag;

    /**
     * The handle to icon or pointer to image object of the item.
     */
    DWORD   image;

	/**
	 * The Additional Data of the item
	 */
	DWORD addData;
}NCS_LSTBOX_ITEMINFO;

typedef struct _mListBox mListBox;
typedef struct _mListBoxClass mListBoxClass;
typedef struct _mListBoxRenderer mListBoxRenderer;

/**
 * \def NCSCTRL_LISTBOX
 * \brief The control class name of mListBox.
 */
#define NCSCTRL_LISTBOX             NCSCLASSNAME("listbox")

/**
 * \def NCSS_LSTBOX_SHIFT
 * \brief The bits used by mListBox in style.
 */
#define NCSS_LSTBOX_SHIFT           (NCSS_ITEMV_SHIFT+4)

/**
 * \def NCSS_LSTBOX_SINGLE
 * \brief Only have a selected item simultaneously.
 */
#define NCSS_LSTBOX_SINGLE          NCSS_ITEMV_SINGLE

/**
 * \def NCSS_LSTBOX_MULTIPLE
 * \brief Allow user to select multiple items.
 */
#define NCSS_LSTBOX_MULTIPLE        NCSS_ITEMV_MULTIPLE

/**
 * \def NCSS_LSTBOX_SORT
 * \brief Sort strings alphabetically.
 */
#define NCSS_LSTBOX_SORT            NCSS_ITEMV_AUTOSORT

/**
 * \def NCSS_LSTBOX_MOUSEFOLLOW
 * \brief When mouse moved, change the selected item. Only for \ref
 *        NCSS_LSTBOX_SINGLE.
 */
#define NCSS_LSTBOX_MOUSEFOLLOW     (0x0001<<NCSS_ITEMV_SHIFT)

/**
 * \def NCSS_LSTBOX_STRING
 * \brief Display a string in an item.
 */
#define NCSS_LSTBOX_STRING          (0x0000<<NCSS_ITEMV_SHIFT)

/**
 * \def NCSS_LSTBOX_USEBITMAP
 * \brief Display a icon or bitmap in an item.
 */
#define NCSS_LSTBOX_USEBITMAP       (0x0002<<NCSS_ITEMV_SHIFT)

/**
 * \def NCSS_LSTBOX_CHECKBOX
 * \brief Display a check box in an item.
 */
#define NCSS_LSTBOX_CHECKBOX        (0x0004<<NCSS_ITEMV_SHIFT)

/**
 * \def NCSS_LSTBOX_AUTOCHECK
 * \brief Auto-switch the check box between checked or un-checked
 *        when the user click the check mark box of an item.
 */
#define NCSS_LSTBOX_AUTOCHECK       (0x0008<<NCSS_ITEMV_SHIFT)

/**
 * \def NCSS_LSTBOX_AUTOCHECKBOX
 * \brief Set the style \ref NCSS_LSTBOX_CHECKBOX and \ref NCSS_LSTBOX_AUTOCHECK.
 */
#define NCSS_LSTBOX_AUTOCHECKBOX  \
    (NCSS_LSTBOX_CHECKBOX | NCSS_LSTBOX_AUTOCHECK)

//for list flags
#define NCSF_LSTBOX_SHIFT           (NCSF_ITEM_SHIFT + 4)

/* check mark, for NCSS_LSTBOX_CHECKBOX*/
/**
 * \def NCSF_LSTBOX_CMBLANK
 * \brief The item is blank.
 */
#define NCSF_LSTBOX_CMBLANK         (0x0000<<NCSF_ITEM_SHIFT)

/**
 * \def NCSF_LSTBOX_CMCHECKED
 * \brief The flag of checked item.
 */
#define NCSF_LSTBOX_CMCHECKED       (0x0001<<NCSF_ITEM_SHIFT)

/**
 * \def NCSF_LSTBOX_CMPARTCHECKED
 * \brief The flag is partly checked item.
 */
#define NCSF_LSTBOX_CMPARTCHECKED   (0x0002<<NCSF_ITEM_SHIFT)

/**
 * \def NCSF_LSTBOX_CHECKMASK
 * \brief The flag mask of check mark or image.
 */
#define NCSF_LSTBOX_CHECKMASK     \
    (NCSF_LSTBOX_CMBLANK | NCSF_LSTBOX_CMCHECKED | NCSF_LSTBOX_CMPARTCHECKED)

/**
 * \def NCSF_LSTBOX_BOLD
 * \brief The flag of bold item.
 */
#define NCSF_LSTBOX_BOLD            (0x0004<<NCSF_ITEM_SHIFT)

/**
 * \def NCSF_LSTBOX_SIGNIFICANT
 * \brief The flag of significant item.
 */
#define NCSF_LSTBOX_SIGNIFICANT     (0x0008<<NCSF_ITEM_SHIFT)

#define mListBoxHeader(className)   \
    mItemViewHeader(className)      \
    PLOGFONT font;                  \
    int     itemVisibles;           \
    int     itemHilighted;          \
    int     selItem;                \
    DWORD   listFlag;

/**
 * \struct mListBox
 * \brief The structure of mListBox control, which derived from
 *        mItemView.
 *
 *      - font \n
 *        Logic font for bold display.
 *
 *      - itemVisibles  \n
 *        The number of visible items.
 *
 *      - itemHilighted  \n
 *        Current hilighted item.
 *
 *      - selItem \n
 *        Record last selected item when use \ref NCSS_LSTBOX_MOUSEFOLLOW style.
 *
 *      - listFlag  \n
 *        The flags of listbox.
 */
struct _mListBox
{
	mListBoxHeader(mListBox)
};

#define mListBoxClassHeader(clsName, parentClass)                       \
    mItemViewClassHeader(clsName, parentClass)                          \
    int (*onChar)(clsName*, int asciiCode, DWORD keyFlags);             \
    int (*setSel)(clsName*, int newSel, int flag);                      \
    int (*addString)(clsName*, const char* string, DWORD addData);      \
    int (*insertString)(clsName*, const char* string, DWORD addData, int index);\
    int (*findString)(clsName*, int start, char* string, BOOL bExact);  \
    BOOL (*delString) (clsName*, int index);                            \
    void (*addItems)(clsName*, NCS_LSTBOX_ITEMINFO *info, int count);   \
    int (*removeItemByIdx)(clsName*, int index);                        \
    BOOL (*bold)(clsName*, int index, BOOL bold);                       \
    BOOL (*isHilightByIdx)(clsName*, int index);                        \
    BOOL (*enableByIdx)(clsName*, int index, BOOL enable);              \
    int (*selectByIdx)(clsName*, int index);                            \
    int (*deselectByIdx)(clsName*, int index);                          \
    BOOL (*setCheckMark)(clsName*, int index, DWORD mark);              \
    DWORD (*getCheckMark)(clsName*, int index);

/**
 * \struct mListBoxClass
 * \brief The virtual function table of mListBox, which derived from
 *        mItemViewClass.
 *
 *  - int (*\b setSel)(mListBox *self, int newSel, int flag);\n
 *    Selects an item in a multiple-selection list box and scroll it into viewport.
 *      \param newSel   The index of the item.
 *      \param flag     Indicates the changes to be made to the listbox item,
 *                      can be one of the following values:
 *                          - -1\n    If the item has been selected, makes it
 *                                    unselected, vice versa.
 *                          - 0\n     Makes the item unselected.
 *                          - other\n Makes the item selected.
 *
 *      \return         0 on success, else -1 to indicates you passed an invalid
 *                      index or the list box has no \ref NCSS_LSTBOX_MULTIPLE style.
 *
 *  - int (*\b addString)(mListBox *self, const char* string, DWORD addData);\n
 *    Add a new item to listbox.
 *      \param string   The string information of new item.
 *      \param addData  The additional data of new item.
 *      \return         The index of item on success, otherwise -1.
 *
 *  - int (*\b findString)(mListBox *self, int start, char* string, BOOL bExact);\n
 *    Find the index of item which uses string as text from start index to end of items.
 *      \param start      The start index.
 *      \param string     The string text.
 *      \param bExact     Whether exactly found or not.
 *      \return           The index of item on success, otherwise -1.
 *
 *  - int (*\b insertString)(mListBox *self, const char* string, DWORD addData, int index);\n
 *    Insert a new item to specified position \a index.
 *      \param string   The string information of new item.
 *      \param addData  The additional data of new item.
 *      \param index    The index of item.
 *      \return         The index of item on success, otherwise -1.
 *
 *  - BOOL (*\b delString)(mListBox *self, int index);\n
 *    Delete the specified item.
 *      \param index    The index of item.
 *      \return         TRUE on success, otherwise FALSE.
 *
 *  - void (*\b addItems)(mListBox *self, \ref NCS_LSTBOX_ITEMINFO *info, int count);\n
 *    Add a series of items to listbox.
 *      \param info     The information of new items.
 *      \param count    The number of items.
 *
 *  - int (*removeItemByIdx)(clsName*, int index);
 *    Remove an item in listbox.
 *
 *  - BOOL (*\b bold)(mListBox *self, int index, BOOL bold);\n
 *    Bold the specified item or not.
 *      \param index    The index of item.
 *      \param bold     Whether bold or not.
 *      \return         TRUE on success, otherwise FALSE.
 *
 *  - BOOL (*isHilightByIdx)(mListBox *self, int index);\n
 *    Determines whehter the specified item by index is hilighted or not.
 *
 *  - BOOL (*enableByIdx)(mListBox *self, int index, BOOL benable);\n
 *    Enable the specified item by index or not.
 *
 *  - int (*selectByIdx)(mListBox *self, int index);\n
 *    Select the specified item by index.
 *
 *  - int (*deselectByIdx)(mListBox *self, int index);\n
 *    Deselect the specified item by index or not.
 *
 *  - BOOL (*\b setCheckMark)(mListBox *self, int index, DWORD mark);\n
 *    Set checkmark status of the specified item.
 *      \param index    The index of item.
 *      \param mark     The mark flag, can be one of the follwing values:
 *                          - \ref NCSF_LSTBOX_CMBLANK
 *                          - \ref NCSF_LSTBOX_CMCHECKED
 *                          - \ref NCSF_LSTBOX_CMPARTCHECKED
 *      \return         TRUE on success, otherwise FALSE.
 *
 *  - DWORD (*\b getCheckMark)(mListBox *self, int index); \n
 *    Get the checkmark status of the specified item.
 *      \param index    The index of item.
 *      \return         The flag of checkmark on success, otherwise 0.
 */
struct _mListBoxClass
{
	mListBoxClassHeader(mListBox, mItemView)
};

#define mListBoxRendererHeader(clsName, parentClass) \
	mItemViewRendererHeader(clsName, parentClass)

/**
 * \struct mListBoxRenderer
 * \brief The structure of mListBox renderer, which derived
 *        from mWidgetRenderer.
 */
struct  _mListBoxRenderer {
	mListBoxRendererHeader(mListBox, mItemView)
};

/**
 * \enum mListBoxProp
 * \brief The properties id of mListBox.
 */
enum mListBoxProp
{
    /**
     * The maximum width of item.
     */
    NCSP_LSTBOX_ITEMWIDTH = NCSP_SWGT_CONTWIDTH,

    /**
     * The number of items. Read only.
     */
    NCSP_LSTBOX_ITEMCOUNT = NCSP_ITEMV_ITEMCOUNT,

    /**
     * The height of item.
     */
    NCSP_LSTBOX_ITEMHEIGHT = NCSP_ITEMV_MAX + 1,

    /**
     * The index of top item.
     */
    NCSP_LSTBOX_TOPITEM,

    /**
     * The index of hilighted item.
     * Note:It has different meaning in different selection style.
     *      In single selected mode, it is same as selected item.
     *      But in multiply selected mode, it stands for current focus item which
     *      can be changed by mouse or keyboard.
     */
    NCSP_LSTBOX_HILIGHTEDITEM,

    /**
     * The maximum value of mListBox properties id.
     */
	NCSP_LSTBOX_MAX,
};

/**
 * \enum mListBoxNotify
 * \brief The notification code id of mListBox.
 */
enum mListBoxNotify
{
    /**
     * Click on the string.
     */
    NCSN_LSTBOX_CLICKED = NCSN_ITEMV_CLICKED,

    /**
     * Selected item changed.
     */
    NCSN_LSTBOX_SELCHANGED = NCSN_ITEMV_SELCHANGED,

    /**
     * User has pressed the ENTER key.
     */
    NCSN_LSTBOX_ENTER = NCSN_ITEMV_ENTER,

    /**
     * Get input focus.
     */
    NCSN_LSTBOX_SETFOCUS = NCSN_ITEMV_SETFOCUS,

    /**
     * Lost input focus.
     */
    NCSN_LSTBOX_KILLFOCUS = NCSN_ITEMV_KILLFOCUS,

    /**
     * No enough memory.
     */
    NCSN_LSTBOX_ERRSPACE = NCSN_ITEMV_MAX + 1,

    /**
     * Double click on an item.
     */
    NCSN_LSTBOX_DBCLK,

    /**
     * The checkbox selection is cancelled in ListBox.
     * - param: int, The index of list item.
     */
    NCSN_LSTBOX_CMCANCEL,
    // Obsolete.
    NCSN_LSTBOX_SELCANCEL = NCSN_LSTBOX_CMCANCEL,

    /**
     * Click on the check mark.
     */
    NCSN_LSTBOX_CLKCHKMARK,

    /**
     * The maximum value of mListBox notification code id.
     */
    NCSN_LSTBOX_MAX,
};

/**
 * \var g_stmListBoxCls
 * \brief The instance of mListBoxClass.
 *
 * \sa mListBoxClass
 */
MGNCS_EXPORT extern mListBoxClass g_stmListBoxCls;
    /** @} end of Widget_ListBox */

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif /* _MGUI_CTRL_LSTBOX_H */
#endif //_MGNCSCTRL_LISTBOX
