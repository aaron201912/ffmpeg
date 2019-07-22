/**
 * \file msledit.h
 * \author Wangjian<wangjian@minigui.org>
 * \date 2009/07/14
 *
 * This file includes the definition of mSlEdit control.
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

#ifndef _MGUI_NCSCTRL_SLEDIT_H
#define _MGUI_NCSCTRL_SLEDIT_H

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

/**
 * \defgroup Control_SlEdit mSlEdit
 * @{
 */

/*
 * \def NCSCTRL_SLEDIT
 * \brief the name of sledit control
*/
#define NCSCTRL_SLEDIT   NCSCLASSNAME("sledit")

typedef struct _mSlEdit         mSlEdit;
typedef struct _mSlEditClass    mSlEditClass;
typedef struct _mSlEditRenderer mSlEditRenderer;

#define mSlEditHeader(Class) \
        mEditHeader(Class)      \
        DWORD   status;         \
        int     editPos;        \
        int     selStart;       \
        int     selEnd;         \
        int     nContX;         \
        int     nContW;         \
        RECT    rcVis;          \
        int     nVisW;          \
        int     startX;         \
        int     startY;         \
        int     nBlockSize;     \
        StrBuffer content;      \
        char    *tipText;       \
        int     hardLimit;      \
        int     passwdChar;     \
        int     changed;        \
        TextCopyPaste *cp;

/** 
 * \struct mSlEdit
 * \brief the single line control, derived from \ref mEdit.
 *
 *   - \b status \n
 *      The status of the sledit. the value may be:
 *         - NCST_SLEDIT_FOCUSED 
 *         - NCST_SLEDIT_MODIFY 
 *         - NCST_SLEDIT_TMP    
 *         - NCST_SLEDIT_REPLACE
 *   - \b editPos \n
 *      The modify position,(the position of the caret)
 *   - \b selStart \n
 *      The start of the selected text
 *   - \b selEnd \n
 *      The end of the selected text
 *   - \b nContX \n
 *      Content x offset position
 *   - \b nContW \n
 *      Content width
 *   - \b rcVis \n
 *      Content rect
 *   - \b nVisW \n
 *      Visible area width
 *   - \b startX \n
 *      Text x display position
 *   - \b startY \n
 *      Text y display position
 *   - \b nBlockSize \n
 *      String buffer block size 
 *   - \b content \n
 *      String text buffer
 *   - \b tipText \n
 *      Tip text
 *   - \b hardLimit \n
 *      Hard limit
 *   - \b passwdChar \n
 *      Password character
 *   - \b changed \n
 *      Change flag
 *   - \b cp \n
 *      Copy & Paste operators
 *      
 *  \sa mSlEditClass
 */
struct _mSlEdit
{
    mSlEditHeader(mSlEdit)
};

#define mSlEditClassHeader(clsName, parentClass) \
        mEditClassHeader(clsName, parentClass) 

/**
 * \struct mSlEditClass
 * \brief the VTable of \a mSlEdit, derived from \ref mEditClass.
 *
 * \sa mSlEdit
 */
struct _mSlEditClass
{
    mSlEditClassHeader(mSlEdit, mEdit)
};

/**
 * \var g_stmSlEditCls
 * \brief the instance of mSlEditClass
 *
 * \sa mSlEditClass
 */
MGNCS_EXPORT extern mSlEditClass g_stmSlEditCls;

#define mSlEditRendererHeader(clsName, parentClass) \
	mEditRendererHeader(clsName, parentClass)

/**
 * \struct mSlEditRenderer
 * \brief SlEdit class's Renderer interface, derived from \ref mEditRenderer.
 *
 * \sa mSlEdit, mSlEditClass
 */
struct _mSlEditRenderer
{
	mSlEditRendererHeader(mSlEdit, mEdit)
};


/**
 * \def NCSS_SLEDIT_PASSWORD
 * \brief Displays an asterisk (*) for each character typed into the edit control.
 */ 
#define NCSS_SLEDIT_PASSWORD   (0x0001L<<NCSS_EDIT_SHIFT)

/**
 * \def NCSS_SLEDIT_AUTOSELECT
 * \brief Selects all text when getting focus.
 */ 
#define NCSS_SLEDIT_AUTOSELECT (0x0002L<<NCSS_EDIT_SHIFT)

/*
 * \def NCSS_SLEDIT_TIP
 * \brief Shows specified tip text.
 */
#define NCSS_SLEDIT_TIP        (0x0004L<<NCSS_EDIT_SHIFT)

#define NCSS_SLEDIT_SHIFT       (NCSS_EDIT_SHIFT+3)

/** SlEdit Property define :*/
enum mSlEditProp 
{
    NCSP_SLEDIT_TIPTEXT = NCSP_EDIT_MAX + 1, /**< tip text , Type : char* */
    NCSP_SLEDIT_PWDCHAR,                  /**< password character , Type : char */
    NCSP_SLEDIT_MAX
};

/** SlEdit Notify Code :*/
enum mSlEditNotify
{
    NCSN_SLEDIT_CHANGE = NCSN_EDIT_CHANGE,  /**< content changed */
    NCSN_SLEDIT_ENTER = NCSN_ITEMV_ENTER,  /**< enter key pressed */
    NCSN_SLEDIT_MAX = NCSN_EDIT_MAX + 1,
};

#define NCST_SLEDIT_FOCUSED     0x00000001L
#define NCST_SLEDIT_MODIFY      0x00000002L
#define NCST_SLEDIT_TMP         0x00000004L
#define NCST_SLEDIT_REPLACE     0x00000008L

    /** @} end of Control_SlEdit */

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif /* _MGUI_NCSCTRL_SLEDIT_H */
