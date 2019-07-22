/**
 * \file mpanel.h
 * \author XiaoweiYan
 * \date 2009/02/24
 *
 * This file includes the definition of mPanel control.
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

#ifndef _MGUI_NCSCTRL_PANEL_H
#define _MGUI_NCSCTRL_PANEL_H


#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

/**
 * \defgroup Control_Panel mPanel
 * @{
 */

/**
 * \def NCSCTRL_PANEL
 * \brief The control class name of mPanel.
 */
#define NCSCTRL_PANEL   NCSCLASSNAME("panel")

typedef struct _mPanel mPanel;
typedef struct _mPanelClass mPanelClass;
typedef struct _mPanelRenderer mPanelRenderer;

#define mPanelHeader(className) \
	mWidgetHeader(className)

/**
 * \struct mPanel
 * \brief The structure of mPanel control, which derived from mWidget.
 *        It is a control which is capable of containing other controls.
 */
struct _mPanel
{
	mPanelHeader(mPanel)
};

#define mPanelClassHeader(clsName, parentClass) \
	mWidgetClassHeader(clsName, parentClass)

/**
 * \struct mPanelClass
 * \brief The virtual function table of mPanelClass, which derived from
 *        mWidgetClass.
 */
struct _mPanelClass
{
	mPanelClassHeader(mPanel, mWidget)
};

#define mPanelRendererHeader(clsName, parentClass) \
	mWidgetRendererHeader(clsName, parentClass)

/**
 * \struct mPanelRenderer
 * \brief The structure of mPanel renderer, which derived from
 *        mWidgetRenderer.
 */
struct  _mPanelRenderer {
	mPanelRendererHeader(mPanel, mWidget)
};


/**
 * \var g_stmPanelCls
 * \brief The instance of mPanelClass.
 *
 * \sa mPanelClass
 */
MGNCS_EXPORT extern mPanelClass g_stmPanelCls;

/**
 * \enum mPanelProp
 * \brief The properties id of mPanel.
 */
enum mPanelProp {
    /**
     * The maximum value of mPanel properties.
     */
    NCSP_PANEL_MAX = NCSP_WIDGET_MAX + 1
};

/**
 * \def NCSS_PANEL_SHIFT
 * \brief The bits used by mPanel in style.
 */
#define NCSS_PANEL_SHIFT NCSS_WIDGET_SHIFT

/**
 * \enum mPanelNotify
 * \brief The notification code id of mPanel.
 */
enum mPanelNotify {
    /**
     * The maximum value of mPanel notification code id.
     */
	NCSN_PANEL_MAX = NCSN_WIDGET_MAX + 1
};

    /** @} end of Control_Panel */

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif /* _MGUI_NCSCTRL_PANEL_H */
