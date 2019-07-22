/*
 * \file mitembar.h
 * \author miniStudio team of FMSoft
 * \date 2010/10/09
 *
 \verbatim

    This file is part of mGNCS4Touch, one of MiniGUI components.

    Copyright (C) 2008-2018 FMSoft (http://www.fmsoft.cn).

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

#ifndef __NCS4TOUCH_ITEM_BAR_H__
#define __NCS4TOUCH_ITEM_BAR_H__

/**
 * \defgroup ControlItemBar mItemBar
 * @{
 */
#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

/**
 * \def NCSCTRL_ITEMBAR
 * \brief the name of itembar control
 */
#define NCSCTRL_ITEMBAR     NCSCLASSNAME("itembar")

typedef struct _mItemBar         mItemBar;
typedef struct _mItemBarClass    mItemBarClass;
typedef struct _mItemBarRenderer mItemBarRenderer;

#define mItemBarHeader(clsName) \
    mWidgetHeader(clsName)      \
    BOOL dotHit;                \
    int tickSize;               \
    int imageSize;              \
    int ltextSize;              \
    int rtextSize;              \
    int angleSize;

/**
 * \struct mItemBar
 * \brief ItemBar class, derived from \ref mWidget
 *
 * \sa \ref mWidget
 */
struct _mItemBar {
    mItemBarHeader(mItemBar)
};

#define mItemBarClassHeader(clsName, parentClass) \
    mWidgetClassHeader(clsName, parentClass)

/**
 * \struct mItemBarClass
 * \brief itembar struct of itembar control, derived from \ref mWidgetClass
 *
 * \sa \ref mWidgetClass
 */
struct _mItemBarClass {
    mItemBarClassHeader(mItemBar, mWidget)
};

#define mItemBarRendererHeader(clsName, parentClass) \
    mWidgetRendererHeader(clsName, parentClass)

/**
 * \struct mItemBarRenderer
 * \brief ItemBar class renderer interface, derived from \ref mWidgetRenderer
 *
 * \sa mItemBar, mItemBarClass, mWidgetRenderer
 */
struct _mItemBarRenderer {
    mItemBarRendererHeader(mItemBar, mWidget)
};

/**
 * \var g_stmItemBarCls
 * \brief global mItemBarClass
 */
MTOUCH_EXPORT extern mItemBarClass g_stmItemBarCls;

/**
 * \enum mItemBarProp
 * \brief the properties id of \ref mItemBar
 *
 * \var NCSP_ITEMBAR_CHECKED
 * \brief Identify with NCSS_ITEMBAR_CHECKABLE style controls, is selecte.
 *
 * \var NCSP_ITEMBAR_IMG
 * \brief Displayed as icons with a picture.
 *
 * \var NCSP_ITEMBAR_MARGIN
 * \brief Both ends of the blank size.
 */
enum mItemBarProp {
    NCSP_ITEMBAR_CHECKED = NCSP_WIDGET_MAX + 1,
    NCSP_ITEMBAR_IMG,
    NCSP_ITEMBAR_MARGIN,
    NCSP_ITEMBAR_MAX
};

/**
 * \enum eItemBarNotify
 * \brief the notification code of \ref mItemBar
 *
 * \var NCSN_ITEMBAR_CHECKED
 * \brief Identifies the control with NCSS_ITEMBAR_CHECKABLE style, selection status changes.
 */
enum mItemBarNotify {
    NCSN_ITEMBAR_CHECKED = NCSN_WIDGET_MAX + 1,
    NCSN_ITEMBAR_MAX
};

/**
 * \define mItemBarStyle
 * \brief the style id of \ref mItemBar
 *
 * \var NCSS_ITEMBAR_CHECKABLE
 * \brief Click Check whether there is support for the control.
 *
 * \var NCSS_ITEMBAR_HASCHILD
 * \brief Whether child controls identified.
 */
#define NCSS_ITEMBAR_CHECKABLE      (1 << NCSS_WIDGET_SHIFT)
#define NCSS_ITEMBAR_HASCHILD       (2 << NCSS_WIDGET_SHIFT)
#define NCSS_ITEMBAR_SHIFT          (NCSS_WIDGET_SHIFT + 2)

/**
 * @} end of ControlItemBar
 */
#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif  /* __NCS4TOUCH_ITEM_BAR_H__ */

