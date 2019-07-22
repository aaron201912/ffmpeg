/*
 * \file mbtnnavbar.h
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

#ifndef __MGNCS4TOUCH_BNB_H__
#define __MGNCS4TOUCH_BNB_H__

/**
 * \defgroup ControlBtnNavBar mBtnNavBar
 * @{
 */
#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

/**
 * \def NCSCTRL_BTNNAVBAR
 * \brief the name of btnnavbar control
 */
#define NCSCTRL_BTNNAVBAR          NCSCLASSNAME("btnnavbar")

typedef struct _mBtnNavBar         mBtnNavBar;
typedef struct _mBtnNavBarClass    mBtnNavBarClass;
typedef struct _mBtnNavBarRenderer mBtnNavBarRenderer;

/**
 * \define mBtnNavBarStyle
 * \brief the style id of \ref mBtnNavBar
 *
 * \var NCSS_BNB_LRECT
 * \brief Identifies the left button is on behalf of the triangular shape of the left.
 *
 * \var NCSS_BNB_LOPT
 * \brief Identifies the left button is a rounded rectangular shape.
 *
 * \var NCSS_BNB_LNONE
 * \brief Identifies the left button is not displayed the shape.
 *
 * \var NCSS_BNB_RRECT
 * \brief Identifies the right button is on behalf of the triangular shape of the right.
 *
 * \var NCSS_BNB_ROPT
 * \brief Identifies the right button is a rounded rectangular shape.
 *
 * \var NCSS_BNB_RNONE
 * \brief Identifies the right button is not displayed the shape.
 */
#define NCSS_BNB_LRECT      (0x01 << NCSS_WIDGET_SHIFT)
#define NCSS_BNB_LOPT       (0x02 << NCSS_WIDGET_SHIFT)
#define NCSS_BNB_LNONE      (0x04 << NCSS_WIDGET_SHIFT)
#define NCSS_BNB_RRECT      (0x08 << NCSS_WIDGET_SHIFT)
#define NCSS_BNB_ROPT       (0x10 << NCSS_WIDGET_SHIFT)
#define NCSS_BNB_RNONE      (0x20 << NCSS_WIDGET_SHIFT)
#define NCSS_BNB_SHIFT      (NCSS_WIDGET_SHIFT + 6)

/**
 * \enum mBtnNavBarProp
 * \brief the properties id of \ref mBtnNavBar
 *
 * \var NCSP_BNB_LSTRING
 * \brief Store the string on the left button.
 *
 * \var NCSP_BNB_RSTRING
 * \brief Store the string on the right button.
 *
 * \var NCSP_BNB_FONT
 * \brief Set the font size on the navigation bar.
 *
 * \var NCSP_BNB_MARGIN
 * \brief Left and right ends of the blank reserved for control.
 *
 * \var NCSP_BNB_LEFT_W
 * \brief The width of the left button.
 *
 * \var NCSP_BNB_RIGHT_W
 * \brief The width of the right button.
 *
 * \var NCSP_BNB_BTN_H
 * \brief Left and right buttons of the high.
 */
enum mBtnNavBarProp {
    NCSP_BNB_LSTRING = NCSP_WIDGET_MAX + 1,
    NCSP_BNB_RSTRING,
    NCSP_BNB_FONT,
    NCSP_BNB_MARGIN,
    NCSP_BNB_LEFT_W,
    NCSP_BNB_RIGHT_W,
    NCSP_BNB_BTN_H,
    NCSP_BNB_MAX
};

#define mBtnNavBarHeader(clsName) \
    mWidgetHeader(clsName)        \
    PLOGFONT caption_font;        \
    char *left_str;               \
    char *right_str;              \
    int margin_w;                 \
    int leftbt_w;                 \
    int rightbt_w;                \
    int button_h;

/**
 * \struct mBtnNavBar
 * \brief BtnNavBar class, derived from \ref mWidget
 *
 * \sa \ref mWidget
 */
struct _mBtnNavBar {
    mBtnNavBarHeader(mBtnNavBar)
};

#define mBtnNavBarClassHeader(clsName, parentClass) \
    mWidgetClassHeader(clsName, parentClass)

/**
 * \struct mBtnNavBarClass
 * \brief  toolcaption struct of toolcaption control, derived from \ref mWidgetClass
 *
 * \sa \ref mWidgetClass
 */
struct _mBtnNavBarClass {
    mBtnNavBarClassHeader(mBtnNavBar, mWidget)
};

#define mBtnNavBarRendererHeader(clsName, parentClass) \
    mWidgetRendererHeader(clsName, parentClass)

/**
 * \struct mBtnNavBarRenderer
 * \brief BtnNavBar class renderer interface, derived from \ref mWidgetRenderer
 *
 * \sa mBtnNavBar, mBtnNavBarClass, mWidgetRenderer
 */
struct _mBtnNavBarRenderer {
    mBtnNavBarRendererHeader(mBtnNavBar, mWidget)
};

/**
 * \var g_stmBtnNavBarCls
 * \brief global mBtnNavBarClass
 */
MTOUCH_EXPORT extern mBtnNavBarClass g_stmBtnNavBarCls;

/**
 * \enum mBtnNavBarData
 * \brief define the clicked add of param
 *
 * \var NCSD_BNB_LEFT
 * \brief Left button click event listener param.
 *
 * \var NCSD_BNB_RIGHT
 * \brief Right button click event listener param.
 */
enum mBtnNavBarData {
    NCSD_BNB_LEFT = 0,
    NCSD_BNB_RIGHT,
};

/**
 * \enum mBtnNavBarNotify
 * \brief the notification code of \ref mBtnNavBar
 *
 * \var NCSN_BNB_BTNCLICK
 * \brief Around both ends of the button click event listener.
 */
enum mBtnNavBarNotify {
    NCSN_BNB_BTNCLICK = NCSN_WIDGET_MAX + 1,
    NCSN_BNB_MAX,
};

/**
 * \enum mBtnNavBarBoxLayoutPieceIndexID
 * \brief the piece index id of \ref mBtnNavBar
 *
 * \var BNB_LEFT_PIECE_ID
 * \brief The left piece of the index id.
 *
 * \var BNB_RIGHT_PIECE_ID
 * \brief The right piece of the index id.
 */
enum mBtnNavBarBoxLayoutPieceIndexID {
    BNB_LEFT_PIECE_ID = 0,
    // 1 is the text piece (NULL)
    BNB_RIGHT_PIECE_ID = 2,
    BNB_MAX_PIECE_ID,
};

#define GetLeftBtnPiece(self)   _M((mHBoxLayoutPiece *)self->body, getCell, BNB_LEFT_PIECE_ID)
#define GetRightBtnPiece(self)  _M((mHBoxLayoutPiece *)self->body, getCell, BNB_RIGHT_PIECE_ID)

/**
 * @} end of ControlBtnNavBar
 */
#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif  /* __MGNCS4TOUCH_BNB_H__ */

