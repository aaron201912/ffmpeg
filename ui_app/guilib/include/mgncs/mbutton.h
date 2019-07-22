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

#ifndef _MGUI_NCSCTRL_BUTTON_H
#define _MGUI_NCSCTRL_BUTTON_H
 
#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

/**
 * \defgroup ControlButton mButton
 * @{
 */

/**
 *  \def NCSCTRL_BUTTON
 *  \brief the name of button control
 */
#define NCSCTRL_BUTTON   NCSCLASSNAME("button")

typedef struct _mButton mButton;
typedef struct _mButtonClass mButtonClass;
typedef struct _mButtonRenderer mButtonRenderer;

/**
 * \enum mButtonCheckState
 * \brief define the check state of button
 */

enum mButtonCheckState{
	NCS_BUTTON_UNCHECKED = 0,
	NCS_BUTTON_HALFCHECKED,
	NCS_BUTTON_CHECKED
};

/**
 * \var NCS_BUTTON_UNCHECKED
 * \brief button unchecked state, the button is not pushed down
 */
/**
 * \var NCS_BUTTON_HALFCHECKED
 * \brief button halfchecked state, it is a state between checked and unchecked, halfchecked state is valid if the button is a 3-state button
 */
/**
 * \var NCS_BUTTON_CHECKED
 * \brief button checked state, the button is pushed down
 */

#define mButtonHeader(clsName)  \
	mWidgetHeader(clsName) \
	mWidget* group;

/**
 * \struct mButton
 * \brief Button class, derived from \ref mWidget
 *
 * - mWidget * \b group - the pointer of button group
 *
 * \sa \ref mWidget
 */

struct _mButton
{
	mButtonHeader(mButton)
};

#define mButtonClassHeader(clsName, parentClass)        \
	mWidgetClassHeader(clsName, parentClass) \
	mObject * (*createContent)(clsName *self, DWORD dwStyle); \
	mObject * (*createButtonBody)(clsName *self, DWORD dwStyle, mObject * content);

/**
 * \struct mButtonClass
 * \brief  button struct of button control, derived from \ref mWidgetClass
 *
 * \sa \ref mWidgetClass
 *
 * - mObject * (\b *createContent)( clsName \a *self, DWORD \a dwStyle);\n
 *   create button content based on button style\n
 *   \param *self - this pointer of button
 *   \param dwStyle - the style of button
 *   \return mObject * the button content
 * 
 * - mObject * (\b *createButtonBody)( clsName \a *self, DWORD \a dwStyle, mObject * \a content);\n
 *   create button body
 *   \param *self - this pointer of button
 *   \param dwStyle - the style of button
 *   \param *content - the content of button
 *   \return mObject * the button body
 *
 */

struct _mButtonClass
{
	mButtonClassHeader(mButton, mWidget)
};

#define mButtonRendererHeader(clsName, parentClass)     \
	mWidgetRendererHeader(clsName, parentClass)         \
    void (*drawPushButton)(clsName *self, HDC hdc,     \
            const RECT *rect, \
			DWORD color1, DWORD color2, int status);

/**
 * \struct mButtonRenderer
 * \brief Button class renderer interface, derived from \ref mWidgetRenderer
 *
 *  \sa mButton, mButtonClass, mWidgetRenderer
 *
 *  - void (\b *drawPushButton)( clsName \a *self, HDC \a hdc, const RECT \a *rect, DWORD \a color1, DWORD \a color2, int \a status); \n
 *    draw button using diffrent renderer
 *    \param *self - this pointer of button
 *    \param hdc - DC of control
 *    \param *rect - the rectangle to draw button frame
 *    \param color1 - the color 1 for renderer
 *    \param color2 - the color 2 for renderer
 *    \param status - the button state
 *
 */

struct _mButtonRenderer
{
	mButtonRendererHeader(mButton, mWidget)
};

/**
 * \var g_stmButtonCls
 * \brief global mButtonClass
 */
MGNCS_EXPORT extern mButtonClass g_stmButtonCls;

/**
 * \enum mButtonProp
 * \brief the properties id of \ref mButton
 */
enum mButtonProp {
	/**
	 * \brief Set the button text horizontal align
	 *  - Type \a enumNCSAlign
	 *  - Read Write
	 *  - set the horizontal align value of the text on button
	 *
	 */
	NCSP_BUTTON_ALIGN = NCSP_WIDGET_MAX + 1,
	/**
	 * \brief Set the button text vertical align
	 *  - Type \a enumNCSVAlign
	 *  - Read Write
	 *  - set the vertical align value of the text on button
	 *
	 */
	NCSP_BUTTON_VALIGN,
	/**
	 * \brief Set the button text to autowrap mode 
	 *  - Type int
	 *  - Read Write
	 *  - turn on/off the autowrap mode of button text
	 *
	 */
	NCSP_BUTTON_WORDWRAP,
	/**
	 * \brief Set the button image pointer 
	 *  - Type PBITMAP
	 *  - Read Write
	 *  - Set the image pointer for the image displayed on button
	 *
	 */
	NCSP_BUTTON_IMAGE,
	/**
	 * \brief Set the check state of button 
	 *  - Type \ref mButtonCheckState
	 *  - Read Write
	 *  - set the check state of button to checked/half-checked/unchecked
	 *
	 */
	NCSP_BUTTON_CHECKSTATE,
	/**
	 * \brief Set the image size
	 *  - Type int
	 *  - Read Write
	 *  - Set the image size percent on button, it should be >=15 and <=85, this property works when the button has NCSS_BUTTON_IMAGELABEL style
	 *
	 */
	NCSP_BUTTON_IMAGE_SIZE_PERCENT, 
	/**
	 * \brief Set the button group id 
	 *  - Type int
	 *  - Read Write
	 *  - Set the the button group id
	 *
	 */
	NCSP_BUTTON_GROUPID,
	/**
	 * \brief Set the button group
	 *  - Type mButtonGroup*
	 *  - Read Write
	 *  - Set the button group pointer
	 *
	 */
	NCSP_BUTTON_GROUP,
	NCSP_BUTTON_MAX
};

/**
 * \enum eButtonNotify
 * \brief the notification code of \ref mButton
 */

enum eButtonNotify {
	/**
	 * \brief button pushed 
	 *
	 */
	NCSN_BUTTON_PUSHED = NCSN_WIDGET_MAX + 1,
	/**
	 * \brief button check state changed
	 * \param the new check state
	 *
	 * \sa NCSP_BUTTON_CHECKSTATE
	 */
	NCSN_BUTTON_STATE_CHANGED,
	NCSN_BUTTON_MAX
};

/**
 * \def NCSS_BUTTON_IMAGE
 * \brief indicate a image button
 */
#define NCSS_BUTTON_IMAGE        (1<<NCSS_WIDGET_SHIFT)
/**
 * \def NCSS_BUTTON_CHECKABLE
 * \brief indicate a chackable button
 */
#define NCSS_BUTTON_CHECKABLE    (2<<NCSS_WIDGET_SHIFT)
/**
 * \def NCSS_BUTTON_AUTOCHECK
 * \brief indicate a auto check button
 * \note This style must combine with NCSS_BUTTON_CHECKABLE
 */
#define NCSS_BUTTON_AUTOCHECK    (4<<NCSS_WIDGET_SHIFT)
/**
 * \def NCSS_BUTTON_3DCHECK
 * \brief indicate a three type (unchecked-halfchekced-checked) check button
 * \note This style must combine with NCSS_BUTTON_3DCHECK
 */
#define NCSS_BUTTON_3DCHECK      (8<<NCSS_WIDGET_SHIFT)
/**
 * \def NCSS_BUTTON_IMAGELABEL
 * \brief indicate a button with image and label
 * \note by default, image and label are lined up horizontal
 */
#define NCSS_BUTTON_IMAGELABEL   (0x10<<NCSS_WIDGET_SHIFT)
/**
 * \def NCSS_BUTTON_VERTIMAGELABEL
 * \brief indicate a button with vertical image and label
 * \note This sytle must combine with NCSS_BUTTON_IMAGELABEL
 */
#define NCSS_BUTTON_VERTIMAGELABEL (0x20<<NCSS_WIDGET_SHIFT)
#define NCSS_BUTTON_SHIFT        (NCSS_WIDGET_SHIFT + 6)

/**
 * @} end of ControlButton
 */


#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif /* _MGUI_NCSCTRL_BUTTON_H */

