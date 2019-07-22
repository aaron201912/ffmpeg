/**
 * \file 
 * \author 
 * \date 
 *
 * \brief 
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

#ifndef _MGUI_NCSCTRL_BTNGRP_H
#define _MGUI_NCSCTRL_BTNGRP_H

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

/**
 * \defgroup ControlButtongroup mButtonGroup
 * @{
 */

/**
 * \def NCSCTRL_BUTTONGROUP
 * \brief name of buttongroup control
 */
#define NCSCTRL_BUTTONGROUP        NCSCLASSNAME("buttongroup")

typedef struct _mButtonGroup mButtonGroup;
typedef struct _mButtonGroupClass mButtonGroupClass;
typedef struct _mButtonGroupRenderer mButtonGroupRenderer;

typedef struct _mButtonNode mButtonNode;
/**
 * \struct _mButtonNode
 * \brief the list node include member button
 *
 * - \ref mWidget \a *pbtn \n
 *   button pointer
 * - mButtonNode \a *next \n
 *   pointer to next button node
 *
 */
struct _mButtonNode
{
	mWidget         *pbtn;
	mButtonNode     *next;
};

#define mButtonGroupHeader(clsName) \
	mGroupBoxHeader(clsName) \
    mButtonNode *pBtnList; 

/**
 * \struct mButtonGroup
 * \brief A group control, mamanger a list of buttons, 
 *  in which only one button can be selected. Derived from \ref mGroupBox.
 *
 * - mButtonNode \a *pBtnList \n
 *   button list
 */
struct _mButtonGroup
{
	mButtonGroupHeader(mButtonGroup)
};

#define mButtonGroupClassHeader(clsName, parentClass) \
	mGroupBoxClassHeader(clsName, parentClass) \
/*private*/ \
    BOOL (*addButton)(clsName *group, mWidget *button);	\
	BOOL (*checkBtn)(clsName *group, mWidget *btn_to_check);

/**
 * \struct mButtonGroupClass
 * \brief struct of mButtonGroupClass control, derived from \ref mGroupBoxClass
 *
 * - BOOL (\b *addButton)(clsName \a *group, \ref mWidget \a *button);\n
 *   add a button to the list \n
 *	 \param *group - pointer of the mButtonGroup
 *	 \param *button - pointer of the button to add
 *	 \return TRUE - button added, FALSE - failed to add button
 *
 * - BOOL (\b *checkBtn)(clsName \a *group, \ref mWidget \a *btn_to_check);\n
 *   change the check state of button
 *   \param *group - the pointer of button group
 *   \param *btn_to_check - the pointer of button
 *   \return TRUE - button checked, FALSE - fail to check button:w
 *
 */
 
struct _mButtonGroupClass
{
	mButtonGroupClassHeader(mButtonGroup, mGroupBox)
};

#define mButtonGroupRendererHeader(clsName, parentClass) \
	mGroupBoxRendererHeader(clsName, parentClass) \
    void (*drawButtongroup)(clsName *self, HDC, const RECT*);

/**
 * \struct mButtonGroupRenderer
 * \brief mButtonGroup renderer interface, derived from \ref mGroupBoxRenderer
 *
 * - void (\b *drawButtongroup)(clsName \a *self, HDC \a hdc, const RECT \a *pRect); \n
 *   draw buttongroup using renderer
 *   \param *self - this pointer of buttongroup
 *   \param hdc - dc of control
 *   \param *pRect - pointer of the rectangle to draw the buttongroup
 */

/* define the render of image static */
struct _mButtonGroupRenderer {
	mButtonGroupRendererHeader(mButtonGroup, mGroupBox)
};

/**
 * \var g_stmButtonGroupCls
 * \brief global mButtonGroupClass
 */
MGNCS_EXPORT extern mButtonGroupClass g_stmButtonGroupCls;

/**
 * \enum mButtonGroupProp
 * \brief properties of \ref mButtonGroup
 */
enum mButtonGroupProp {
	/**
	 * \brief get/set selected id of button
	 *  - Type: int
	 *  - Read Write
	 *  - Change the state of buttons
	 * \sa NCSP_BTNGRP_SELIDX, NCSP_BTNGRP_SELOBJ
	 */
	NCSP_BTNGRP_SELID = NCSP_GRPBOX_MAX + 1,
	/**
	 * \brief set/set selected index
	 *   - Type: idx
	 *   - Read Write
	 *   - Change the state of buttons
	 *
	 *  \sa NCSP_BTNGRP_SELID, NCSP_BTNGRP_SELOBJ
	 */
	NCSP_BTNGRP_SELIDX,
	/**
	 * \brief set/get selected object
	 *   - Type: mWidget* (mButton*)
	 *   - Read Write
	 *   - Change the state of button
	 *  \sa NCSP_BTNGRP_SELID, NCSP_BTNGRP_SELIDX
	 */
	NCSP_BTNGRP_SELOBJ,
	NCSP_BTNGRP_MAX
};

#define NCSS_BTNGRP_SHIFT NCSS_GRPBOX_SHIFT

/**
 * \enum enumButtonGroupNotify
 * \brief notification code of \ref mButtonGroup
 */
enum enumButtonGroupNotify{
	/**
	 * \brief raised when the current button changed
	 * \param the id of new selected button
	 *
	 * \sa NCSP_BTNGRP_SELID
	 */
	NCSN_BTNGRP_SELCHANGED = NCSN_GRPBOX_MAX + 1,
	NCSN_BTNGRP_MAX
};

/**
 * @} end of ControlButtongroup
 */
#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif /* _MGUI_NCSCTRL_BTNGRP_H */

