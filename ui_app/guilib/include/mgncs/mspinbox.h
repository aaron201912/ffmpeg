/**
 * \file mspinbox.h
 * \author Wangjian<wangjian@minigui.org>
 * \date 2009/03/01
 *
 * This file includes the definition of mSpinBox control.
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

#ifdef _MGNCSCTRL_SPINBOX

#ifndef _MGUI_NCSCTRL_SPINBOX_H
#define _MGUI_NCSCTRL_SPINBOX_H

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

/**
 * \defgroup Control_Spinbox mSpinBox
 * @{
 */

/*
 * \def NCSCTRL_SPINBOX
 * \brief the name of spinbox control
*/
#define NCSCTRL_SPINBOX   NCSCLASSNAME("spinbox")

#define NCS_LEN_SPINBOX     63

typedef struct _mSpinBox            mSpinBox;
typedef struct _mSpinBoxClass       mSpinBoxClass;
typedef struct _mSpinBoxRenderer    mSpinBoxRenderer;

typedef struct _mSpinBoxItemInfo    mSpinBoxItemInfo;
#define mSpinBoxHeader(clsName)     \
    mSpinnerHeader(clsName)      \
	union {                         \
		char* num_format;           \
		mSpinBoxItemInfo *items;    \
	}info;

/**
 * \struct mSpinBox
 * \brief the spinbox struct of spinbox control, derived from \ref mSpinner.
 *
 *  \sa mSpinBoxClass
 */
struct _mSpinBox
{
	mSpinBoxHeader(mSpinBox)
};

#define mSpinBoxClassHeader(clsName, parentClass)   \
	mSpinnerClassHeader(clsName, parentClass)       \
    BOOL (*addItem)(clsName *self, char *item);    \
    BOOL (*removeItem)(clsName *self, int index);  \
    BOOL (*setItem)(clsName *self, int index, char *item);     \
    char* (*getItem)(clsName *self, int index);

/**
 * \struct mSpinBoxClass
 * \brief the VTable of \a mSpinBox, derived from \ref mSpinnerClass.
 *
 * \sa mSpinnerClass
 *
 *  - BOOL \b addItem(\a mSpinBox *self, char *item); \n
 *    add a item to the list
 *    \param item - the item to add
 *
 *  - BOOL \b removeItem(\a mSpinBox *self, int index); \n
 *    remove a item from the list
 *    \param index - the item index to remove
 *
 *  - BOOL \b setItem(\a mSpinBox *self, int index, const char *item); \n
 *    set the content of one item
 *    \param index - the item index to set
 *    \param item - the content to set
 *
 *  - char * \b getItem(\a mSpinBox *self, int index); \n
 *    get the content of one item
 *    \param index - the item index to get
 *
 */
struct _mSpinBoxClass
{
	mSpinBoxClassHeader(mSpinBox, mSpinner)
};

MGNCS_EXPORT extern mSpinBoxClass g_stmSpinBoxCls;

#define mSpinBoxRendererHeader(clsName, parentClass)    \
	mSpinnerRendererHeader(clsName, parentClass)

/**
 * \struct mSpinBoxRenderer
 * \brief Spinner class's Renderer interface, derived from \ref mSpinnerRenderer.
 *
 * \sa mSpinBox, mSpinBoxClass
 */
struct _mSpinBoxRenderer
{
	mSpinBoxRendererHeader(mSpinBox, mSpinner)
};

/** Spinner Property define:*/
enum mSpinBoxProp {
    NCSP_SPNBOX_MAXPOS = NCSP_SPNR_MAXPOS,      /**< the maximum value */
    NCSP_SPNBOX_MINPOS = NCSP_SPNR_MINPOS,      /**< the minimum value */
    NCSP_SPNBOX_CURPOS = NCSP_SPNR_CURPOS,      /**< the current value */
    NCSP_SPNBOX_LINESTEP = NCSP_SPNR_LINESTEP,  /**< the line step length*/
    NCSP_SPNBOX_MAX = NCSP_SPNR_MAX
};


/**
 * \def NCSS_SPNBOX_VERTICAL
 * \brief vertical spinbox
 */
#define NCSS_SPNBOX_VERTICAL           (NCSS_SPNR_VERTICAL)

/**
 * \def NCSS_SPNBOX_HORIZONTAL
 * \brief horizontal spinbox
 */
#define NCSS_SPNBOX_HORIZONTAL         (NCSS_SPNR_HORIZONTAL)

/**
 * \def NCSS_SPNBOX_AUTOLOOP
 * \brief the value of spinbox auto loop
 */
#define NCSS_SPNBOX_AUTOLOOP           (NCSS_SPNR_AUTOLOOP)

/**
 * \def NCSS_SPNBOX_NUMBER
 * \brief spinbox show number
 */
#define NCSS_SPNBOX_NUMBER             (0x0000L<<NCSS_SPNR_SHIFT)

/**
 * \def NCSS_SPNBOX_STRING
 * \brief spinbox shown strings
 */
#define NCSS_SPNBOX_STRING             (0x0001L<<NCSS_SPNR_SHIFT)

/**
 * \def NCSS_SPNBOX_SORT
 * \brief auto sort spinbox
 */
#define NCSS_SPNBOX_SORT               (0x0002L<<NCSS_SPNR_SHIFT)

/**
 * \def NCSS_SPNBOX_EDITNOBORDER
 * \brief spinbox with no-border edit
 */
#define NCSS_SPNBOX_EDITNOBORDER       (0x0004L<<NCSS_SPNR_SHIFT)

/**
 * \def NCSS_SPNBOX_EDITBASELINE
 * \brief spinbox with base-line edit
 */
#define NCSS_SPNBOX_EDITBASELINE       (0x0008L<<NCSS_SPNR_SHIFT)

/**
 * \def NCSS_SPNBOX_READONLY
 * \brief spinbox with read-only edit
 */
#define NCSS_SPNBOX_READONLY           (0x0010L<<NCSS_SPNR_SHIFT)

/**
 * \def NCSS_SPNBOX_UPPERCASE
 * \brief spinbox show uppercase
 */
#define NCSS_SPNBOX_UPPERCASE          (0x0020L<<NCSS_SPNR_SHIFT)

/**
 * \def NCSS_SPNBOX_LOWERCASE
 * \brief spinbox show lowercase
 */
#define NCSS_SPNBOX_LOWERCASE          (0x0040L<<NCSS_SPNR_SHIFT)

/**
 * \def NCSS_SPNBOX_AUTOFOCUS
 * \brief spinbox with auto-focus edit
 */
#define NCSS_SPNBOX_AUTOFOCUS          (0x0080L<<NCSS_SPNR_SHIFT)

#define NCSS_SPNBOX_SHIFT      (NCSS_SPNR_SHIFT + 9)


/** Spinner Notify Codes:*/
enum mSpinBoxNotify {
    NCSN_SPNBOX_CHANGED = NCSN_SPNR_CHANGED,    /**< value changed */
    NCSN_SPNBOX_REACHMAX = NCSN_SPNR_REACHMAX,  /**< reached the max value*/
    NCSN_SPNBOX_REACHMIN = NCSN_SPNR_REACHMIN,  /**< reached the min value*/
    NCSN_SPNBOX_MAX = NCSN_SPNR_MAX
};

//////////////////////////////////////////////
////////////------ status ------ /////////////
//////////////////////////////////////////////

#define NCSST_SPNBOX_NORMAL        0x0000
#define NCSST_SPNBOX_DISABLE       0x0001
#define NCSST_SPNBOX_HILIGHT       0x0002
#define NCSST_SPNBOX_PUSHED        0x0004

/** @} end of Control_Spinbox*/

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif /* _MGUI_NCSCTRL_SPINBOX_H */
#endif //_MGNCSCTRL_SPINBOX
