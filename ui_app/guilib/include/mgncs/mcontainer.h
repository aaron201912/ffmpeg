/**
 * \file mcontainer.h
 * \author XiaoweiYan
 * \date 2009/02/24
 *
 * This file includes the definition of mContainer control.
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

#ifdef _MGNCSCTRL_CONTAINER

#ifndef _MGUI_WIDGET_CTNR_H
#define _MGUI_WIDGET_CTNR_H


#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

/**
 * \defgroup Control_Container mContainer
 * @{
 */

/**
 * \def NCSCTRL_CONTAINER
 * \brief The control class name of mContainer.
 */
#define NCSCTRL_CONTAINER        NCSCLASSNAME("container")

typedef struct _mContainer mContainer;
typedef struct _mContainerClass mContainerClass;
typedef struct _mContainerRenderer mContainerRenderer;

#define mContainerHeader(className) \
	mScrollWidgetHeader(className) \
    HWND hPanel;

/**
 * \struct mContainer
 * \brief The structure of mContainer, which derived from \a mScrollWidget.
 *        It is a control with scrollbars which is capable of containing other
 *        controls. When the user uses the scrollbars, it will scroll its content
 *        automatically.
 *
 *      - hPanel \n
 *      The handle of panel in scrollcontainer.
 */
struct _mContainer
{
	mContainerHeader(mContainer)
};

#define mContainerClassHeader(clsName, parentClass)                   \
	mScrollWidgetClassHeader(clsName, parentClass)                    \
    BOOL (*addIntrinsicControls)(clsName* self, const PCTRLDATA pCtrl, int nCount); \
    HWND (*setFocus)(clsName* self, int id); \
    HWND (*getFocus)(clsName* self); \
	HWND (*getPanel)(clsName* self); \
	void (*adjustContent)(clsName *self);

/**
 * \struct mContainerClass
 * \brief The virtual function table of mContainer, which derived from
 *        mScrollWidgetClass.
 *
 *  - BOOL (*\b addIntrinsicControls)(mContainer *self, const PCTRLDATA pCtrl, int nCount); \n
 *    Add intrinsic controls in MiniGUI to container.
 *      \param pCtrl      The pointer to control data in MiniGUI V3.0.x.
 *      \param nCount     The number of controls.
 *      \return           TRUE for success, otherwise FALSE.
 *
 *  - HWND (* \b setFocus)(mContainer *self, int id); \n
 *    Set focus control.
 *      \param id         The identifier of active child.
 *      \return           The handle to the old active child of its parent.
 *
 *  - HWND (*\b getFocus)(mContainer *self);\n
 *    Get focus control.
 *      \return           The handle to the active child.
 *
 *  - HWND (*\b getPanel)(mContainer *self);\n
 *    Get the panel handle.
 *     	\return 		  The panel handler of container.
 *
 *  - void (*\b adjustContent)(mContainer *self); \n
 *    Adjust the content size.
 */
struct _mContainerClass
{
	mContainerClassHeader(mContainer, mScrollWidget)
};

#define mContainerRendererHeader(clsName, parentClass) \
	mScrollWidgetRendererHeader(clsName, parentClass)

/**
 * \struct mContainerRenderer
 * \brief The renderer structure of mContainer, which inheried from
 *        mScrollWidgetRenderer.
 */
struct  _mContainerRenderer {
	mContainerRendererHeader(mContainer, mScrollWidget)
};


/**
 * \var g_stmContainerCls
 * \brief The instance of mContainerClass.
 *
 * \sa mContainerClass
 */
MGNCS_EXPORT extern mContainerClass g_stmContainerCls;

/**
 * \def NCSS_CTNR_SHIFT
 * \brief The bits used by mContainer in style.
 */
#define NCSS_CTNR_SHIFT NCSS_SWGT_SHIFT

/**
 * \enum mContainerNotify
 * \brief The notification code id of mContainer.
 */
enum mContainerNotify {
    /**
     * The maximum value of mContainer notification code id.
     */
	NCSN_CTNR_MAX = NCSN_SWGT_MAX + 1
};

    /** @} end of Control_Container */
#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif /* _MGUI_WIDGET_CTNR_H */
#endif 	//_MGNCSCTRL_CONTAINER
