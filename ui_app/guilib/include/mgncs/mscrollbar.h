/**
 * \file mscrollbar.h
 * \author Wangjian<wangjian@minigui.org>
 * \date 2009/03/01
 *
 * This file includes the definition of mScrollBar control.
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

#ifdef _MGNCSCTRL_SCROLLBAR

#ifndef _MGUI_NCSCTRL_SCRLBR_H
#define _MGUI_NCSCTRL_SCRLBR_H

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

/**
 * \defgroup Control_ScrollBar mScrollBar
 * @{
 */

/*
 * \def NCSCTRL_SCROLLBAR
 * \brief the name of scrollbar control
*/
#define NCSCTRL_SCROLLBAR   NCSCLASSNAME("scrollbar")

typedef struct _mScrollBar            mScrollBar;
typedef struct _mScrollBarClass       mScrollBarClass;
typedef struct _mScrollBarRenderer    mScrollBarRenderer;

#define mScrollBarHeader(Class) \
    mSliderHeader(Class)

/**
 * \struct mScrollBar
 * \brief the scrollbar struct of scrollbar control, derived from \ref mSlider.
 *
 *  \sa mScrollBarRenderer, mScrollBarClass
 */
struct _mScrollBar
{
	mScrollBarHeader(mScrollBar)
};

#define mScrollBarClassHeader(clsName, parentClass) \
	mSliderClassHeader(clsName, parentClass)

/**
 * \struct mScrollBarClass
 * \brief the VTable of \a mScrollBar, derived from \ref mSliderClass.
 *
 * \sa mSliderClass
 */
struct _mScrollBarClass
{
	mScrollBarClassHeader(mScrollBar, mSlider)
};

/**
 * \var g_stmScrollBarCls
 * \brief the instance of mScrollBarClass
 *
 * \sa mScrollBarClass
 */
MGNCS_EXPORT extern mScrollBarClass g_stmScrollBarCls;


#define mHScrollBar mScrollBar
#define mHScrollBarClass mScrollBarClass
#define g_stmHScrollBarCls  g_stmScrollBarCls
#define mVScrollBar mScrollBar
#define mVScrollBarClass mScrollBarClass
#define g_stmVScrollBarCls  g_stmScrollBarCls


#define mScrollBarRendererHeader(clsName, parentClass) \
	mSliderRendererHeader(clsName, parentClass)

/**
 * \struct mScrollBarRenderer
 * \brief ScrollBar class's Renderer interface, derived from \ref mSliderRenderer.
 *
 * \sa mScrollBar, mScrollBarClass
 */
struct _mScrollBarRenderer
{
	mScrollBarRendererHeader(mScrollBar, mSlider)
};


/**
 * \def NCSP_SCRLBR_MAXPOS
 * \brief The value of the maximum position.
 */
#define NCSP_SCRLBR_MAXPOS		NCSP_SLIDER_MAXPOS
/**
 * \def NCSP_SCRLBR_MINPOS
 * \brief The value of the minimum position.
 */
#define NCSP_SCRLBR_MINPOS		NCSP_SLIDER_MINPOS
/**
 * \def NCSP_SCRLBR_CURPOS
 * \brief The value of the current position.
 */
#define NCSP_SCRLBR_CURPOS		NCSP_SLIDER_CURPOS
/**
 * \def NCSP_SCRLBR_LINESTEP
 * \brief The line step length.
 */
#define NCSP_SCRLBR_LINESTEP	NCSP_SLIDER_LINESTEP
/**
 * \def NCSP_SCRLBR_PAGESTEP
 * \brief The page step length.
 */
#define NCSP_SCRLBR_PAGESTEP	NCSP_SLIDER_PAGESTEP

#define NCSP_SCRLBR_MAX        (NCSP_SLIDER_MAX + 1)


/**
 * \def NCSS_SCRLBR_HORIZONTAL
 * \brief horizontal scrollbar.
 */
#define NCSS_SCRLBR_HORIZONTAL       NCSS_SLIDER_HORIZONTAL
/**
 * \def NCSS_SCRLBR_ARROWS
 * \brief indicate that the scrollbar has arrows or doesn't
 */
#define NCSS_SCRLBR_ARROWS           (1<<NCSS_SLIDER_SHIFT)
/**
 * \def NCSS_SCRLBR_LEFTDBLARROWS
 * \brief indicate that the scrollbar has two arrows in the left side
 * \note don't use the style with \ref NCSS_SCRLBR_RIGHTDBLARROWS
 */
#define NCSS_SCRLBR_LEFTDBLARROWS    (2<<NCSS_SLIDER_SHIFT)
/**
 * \def NCSS_SCRLBR_UPDBLARROWS
 * \brief indicate that the scrollbar has two arrows in the up side
 * \note don't use the style with \ref NCSS_SCRLBR_DOWNDBLARROWS
 */
#define NCSS_SCRLBR_UPDBLARROWS      NCSS_SCRLBR_LEFTDBLARROWS
/**
 * \def NCSS_SCRLBR_RIGHTDBLARROWS
 * \brief indicate that the scrollbar has two arrows in the right side
 * \note don't use the style with \ref NCSS_SCRLBR_LEFTDBLARROWS
 */
#define NCSS_SCRLBR_RIGHTDBLARROWS   (4<<NCSS_SLIDER_SHIFT)
/**
 * \def NCSS_SCRLBR_DOWNDBLARROWS
 * \brief indicate that the scrollbar has two arrows in the down side
 * \note don't use the style with \ref NCSS_SCRLBR_UPDBLARROWS
 */
#define NCSS_SCRLBR_DOWNDBLARROWS     NCSS_SCRLBR_RIGHTDBLARROWS
#define NCSS_SCRLBR_SHIFT            (3+NCSS_SLIDER_SHIFT)

/**
 * \def NCSS_SCRLBR_VERTICAL
 * \brief vertical scrollbar.
 */
#define NCSS_SCRLBR_VERTICAL   NCSS_SLIDER_VERTICAL

/**
 * \def NCSN_SCRLBR_CHANGED
 * \brief positon changed notify code.
 */
#define	NCSN_SCRLBR_CHANGED    NCSN_SLIDER_CHANGED

/**
 * \def NCSN_SCRLBR_REACHMAX
 * \brief reached the maximum position notify code.
 */
#define	NCSN_SCRLBR_REACHMAX   NCSN_SLIDER_REACHMAX

/**
 * \def NCSN_SCRLBR_REACHMIN
 * \brief reached the minimum position notify code.
 */
#define	NCSN_SCRLBR_REACHMIN   NCSN_SLIDER_REACHMIN

/**
 * \def NCSN_SCRLBR_STARTDRAG
 * \brief start drag notify code.
 */
//#define	NCSN_SCRLBR_STARTDRAG  NCSN_SLIDER_STARTDRAG

/**
 * \def NCSN_SCRLBR_STOPDRAG
 * \brief stop drag notify code.
 */
//#define	NCSN_SCRLBR_STOPDRAG   NCSN_SLIDER_STOPDRAG

#define	NCSN_SCRLBR_MAX        NCSN_SLIDER_MAX

/** @} end of Control_ScrollBar*/

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif /* _MGUI_NCSCTRL_SCRLBR_H */
#endif //_MGNCSCTRL_SCROLLBAR
