/**
 * \file mcombobox.h
 * \author Wangjian<wangjian@minigui.org>
 * \date 2009/03/01
 *
 * This file includes the definition of mCombobox control.
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

#ifdef _MGNCSCTRL_COMBOBOX

#ifndef _MGUI_PANEL_COMBOBOX_H
#define _MGUI_PANEL_COMBOBOX_H


#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

/**
 * \defgroup Control_Combobox mCombobox
 * @{
 */

/**
 * \def NCSCTRL_COMBOBOX
 * \brief the name of combobox control
*/
#define NCSCTRL_COMBOBOX        NCSCLASSNAME("combobox")

typedef struct _mCombobox           mCombobox;
typedef struct _mComboboxClass      mComboboxClass;
typedef struct _mComboboxRenderer   mComboboxRenderer;

#define mComboboxHeader(clsName) \
	 mPanelHeader(clsName) \
    mListBox *listbox; \
    mSlEdit *edit; \
    RECT rcList; \
    RECT rcEdit; \
    RECT rcBtn; \
    unsigned int flag; \
    int dropWidth;

/**
 * \struct mCombobox
 * \brief the struct of combobox control, derived from \ref mPanel.
 *
 *   - \b listbox \n
 *      The listbox control.
 *   - \b edit \n
 *      The edit control.
 *   - \b rcList \n
 *      The Rectangle of the listbox control.
 *   - \b rcEdit \n
 *      The Rectangle of the edit control.
 *   - \b rcBtn \n
 *      The Rectangle of the button.(only used by dropdown style one)
 *   - \b flag \n
 *      The flag mark for status
 *   - \b dropWidth \n
 *      The width of the dropdown list
 *
 *  \sa mComboboxRenderer, mComboboxClass
 */
struct _mCombobox
{
	mComboboxHeader(mCombobox)
};

#define mComboboxClassHeader(clsName, parentClass) \
	mPanelClassHeader(clsName, parentClass) \
    BOOL (*addItem)(clsName *self, const char *item, DWORD addData); \
    BOOL (*removeItem)(clsName *self, int index); \
    BOOL (*setItem)(clsName *self, int index, const char *item); \
    const char* (*getItem)(clsName *self, int index); \
    void (*setAddData)(clsName *self, int index, DWORD addData); \
    DWORD (*getAddData)(clsName *self, int index);

/**
 * \struct mComboboxClass
 * \brief the VTable of \a mCombobox, derived from \ref mPanelClass.
 *
 * \sa mPanelClass
 *
 *  - BOOL \b addItem(\a mCombobox *self, char *item, DWORD addData); \n
 *    add a item to the list
 *    \param item - the item to add
 *    \param addData - the additional data
 *
 *  - BOOL \b removeItem(\a mCombobox *self, int index); \n
 *    remove a item from the list
 *    \param index - the item index to remove
 *
 *  - BOOL \b setItem(\a mCombobox *self, int index, const char *item); \n
 *    set the content of one item
 *    \param index - the item index to set
 *    \param item - the content to set
 *
 *  - char * \b getItem(\a mCombobox *self, int index); \n
 *    get the content of one item
 *    \param index - the item index to get
 *
 */
struct _mComboboxClass
{
	mComboboxClassHeader(mCombobox, mPanel)
};

MGNCS_EXPORT extern mComboboxClass g_stmComboboxCls;

#define mComboboxRendererHeader(clsName, parentClass) \
	mPanelRendererHeader(clsName, parentClass) \
	void (*drawDropDownBtn)(clsName* self, HDC, const RECT*);

/**
 * \struct mComboboxRenderer
 * \brief Combobox class's Renderer interface, derived from \ref mPanelRenderer.
 *
 * \sa mCombobox, mComboboxClass
 *
 *  - void \b drawDropDownBtn(\a mCombobox *self, HDC hdc, const RECT *rc);\n
 *    draw the drop down button
 *    \param hdc - the DC in which draw button
 *    \param rc- the Rectange of the button
 *
 */
struct  _mComboboxRenderer {
	mComboboxRendererHeader(mCombobox, mPanel)
};

/**
 * \enum mComboboxProp
 * \brief the properties id of \ref mCombobox
 */
enum mComboboxProp {
	/**
	 * \brief Set/Get height of list item
	 *  - Type int
	 *  - Read Write
	 */
    NCSP_COMB_ITEMHEIGHT = NCSP_LSTBOX_ITEMHEIGHT,
	/**
	 * \brief Get the count of list items
	 *  - Type int
	 *  - ReadOnly
	 */
    NCSP_COMB_ITEMCOUNT = NCSP_LSTBOX_ITEMCOUNT,
	/**
	 * \brief Get/Set the height of dropdown list
	 *  - Type int
	 *  - Read Write
	 *  - the dropdown list height
	 */
	NCSP_COMB_DROPDOWNHEIGHT = NCSP_PANEL_MAX + 1,
	/**
	 * \brief Get/Set the Text limit of edit
	 *  - Type int
	 *  - Read Write
	 *  - the max length of edit accepted
	 */
	NCSP_COMB_TEXTLIMIT,    /**< the text limit of edit */
	/**
	 * \brief Get/Set the selected item of list
	 *  - Type int
	 *  - Read Write
	 *  - The selected item index
	 */
    NCSP_COMB_SELECT,      /**< the the selected item */
    NCSP_COMB_MAX
};


/**
 * \def NCSS_CMBOX_SIMPLE
 * \brief simle combobox, the list box is always show.
 */
#define NCSS_CMBOX_SIMPLE          0x0000L

/**
 * \def NCSS_CMBOX_DROPDOWNLIST
 * \brief dropdownlist combobox, the list box will be shown when user click the dropdown button.
 */
#define NCSS_CMBOX_DROPDOWNLIST    (0x0001L<<NCSS_PANEL_SHIFT)

/**
 * \def NCSS_CMBOX_SORT
 * \brief combobox with the auto sort list.
 */
#define NCSS_CMBOX_SORT            (0x0002L<<NCSS_PANEL_SHIFT)

/**
 * \def NCSS_CMBOX_EDITNOBORDER
 * \brief combobox with no-border edit.
 */
#define NCSS_CMBOX_EDITNOBORDER    (0x0004L<<NCSS_PANEL_SHIFT)

/**
 * \def NCSS_CMBOX_EDITBASELINE
 * \brief combobox with base-line edit.
 */
#define NCSS_CMBOX_EDITBASELINE    (0x0008L<<NCSS_PANEL_SHIFT)

/**
 * \def NCSS_CMBOX_READONLY
 * \brief combobox with read-only edit.
 */
#define NCSS_CMBOX_READONLY        (0x0010L<<NCSS_PANEL_SHIFT)

/**
 * \def NCSS_CMBOX_UPPERCASE
 * \brief combobox with upper-case edit.
 */
#define NCSS_CMBOX_UPPERCASE       (0x0020L<<NCSS_PANEL_SHIFT)

/**
 * \def NCSS_CMBOX_LOWERCASE
 * \brief combobox with lower-case edit.
 */
#define NCSS_CMBOX_LOWERCASE       (0x0040L<<NCSS_PANEL_SHIFT)

/**
 * \def NCSS_CMBOX_AUTOFOCUS
 * \brief combobox with auto focus edit.
 */
#define NCSS_CMBOX_AUTOFOCUS       (0x0080L<<NCSS_PANEL_SHIFT)

#define NCSS_CMBOX_SHIFT (NCSS_PANEL_SHIFT + 9)


/** Combobox Notify Codes:*/
enum enumComboboxNotify {
	NCSN_CMBOX_SELCHANGE = NCSN_PANEL_MAX + 1, /**< selected item have been changed */
	NCSN_CMBOX_SETFOCUS,       /**< control have gaint the focus */
	NCSN_CMBOX_KILLFOCUS,      /**< control have lost the focue*/
	NCSN_CMBOX_EDITCHANGE,     /**< the content of edit have been changed*/
	NCSN_CMBOX_DROPDOWN,       /**< the dropdown list have been shown*/
	NCSN_CMBOX_CLOSEUP,        /**< the dropdown list have been hiden*/
	NCSN_CMBOX_SELECTOK,       /**< an item have been selected */
	NCSN_CMBOX_SELECTCANCEL,   /**< an item have been canceled its selected state*/
	NCSN_CMBOX_MAX
};


//////////////////////////////////////////////
////////////---- some flags ----//////////////
//////////////////////////////////////////////

#define NCSF_CMBOX_BUTTONDOWN         0x0001
#define NCSF_CMBOX_BUTTONUP           0x0002
#define NCSF_CMBOX_DROPPED            0x0004
#define NCSF_CMBOX_HILIGHT            0x0008

#define SET_STATE(lp, mask)   (lp->flag |= (mask))
#define CLEAR_STATE(lp, mask) (lp->flag &= ~(mask))
#define IS_SET(lp, mask)      (lp->flag & (mask))

/** @} end of Control_Combobox*/

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif /* _MGUI_PANEL_COMBOBOX_H */
#endif		//_MGNCSCTRL_COMBOBOX
