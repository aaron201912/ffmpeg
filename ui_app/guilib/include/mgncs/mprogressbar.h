/**
 * \file mprogressbar.h
 * \author Wangjian<wangjian@minigui.org>
 * \date 2009/03/01
 *
 * This file includes the definition of mProgressBar control.
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

#ifdef _MGNCSCTRL_PROGRESSBAR

#ifndef _MGUI_NCSCTRL_PROGRESSBAR_H
#define _MGUI_NCSCTRL_PROGRESSBAR_H

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

/**
 * \defgroup Control_Progressbar mProgressBar
 * @{
 */

/*
 * \def NCSCTRL_PROGRESSBAR
 * \brief the name of progressbar control
*/

#define NCSCTRL_PROGRESSBAR   NCSCLASSNAME("progressbar")

typedef struct _mProgressBar            mProgressBar;
typedef struct _mProgressBarClass       mProgressBarClass;
typedef struct _mProgressBarRenderer    mProgressBarRenderer;

#define mProgressBarHeader(Class)   \
    mWidgetHeader(Class)

 /**
 * \struct mProgressBar
 * \brief the control to indicate the processing of a work, derived from \ref mWidget.
 *
 * \sa mProgressBarRenderer, mProgressBarClass
 */
struct _mProgressBar
{
	mProgressBarHeader(mProgressBar)
};

#define mProgressBarClassHeader(clsName, parentClass) \
	mWidgetClassHeader(clsName, parentClass)          \
    int (*increase)(clsName *self, int delta);       \
    int (*stepIncrease)(clsName *self);

/**
 * \struct mProgressBarClass
 * \brief the VTable of \a mProgressBar, derived from \ref mWidgetClass.
 *
 * \sa mWidgetClass
 *
 *   - int \b increase (mProgressBar *self, int delta);\n
 *       increase the progressbar position.
 *       \param delta - the increase value
 *       \return 0 for succeed, other for failed.\n
 *
 *   - int \b stepIncrease (mProgressBar *self);\n
 *       increase the progressbar position by step.
 *       \return 0 for succeed, other for failed.\n
 *
 */
struct _mProgressBarClass
{
	mProgressBarClassHeader(mProgressBar, mWidget)
};

/**
 * \var g_stmProgressBarCls
 * \brief the instance of mProgressBarClass
 *
 * \sa mProgressBarClass
 */
MGNCS_EXPORT extern mProgressBarClass g_stmProgressBarCls;

#define mProgressBarRendererHeader(clsName, parentClass) \
	mWidgetRendererHeader(clsName, parentClass) \
    void (*drawProgress)(clsName* self, HDC hdc, const RECT *rc);

/**
 * \struct mProgressBarRenderer
 * \brief ProgressBar class's Renderer interface, derived from \ref mWidgetRenderer.
 *
 * \sa mProgressBar, mProgressBarClass
 *
 * - void \b drawProgress(\a mProgressBar *self, HDC hdc, const RECT *rc);\n
 *    draw progressbar
 *    \param hdc - the DC in which draw progressbar
 *    \param rc- the Rectange of the progressbar
 *
 */
struct _mProgressBarRenderer
{
	mProgressBarRendererHeader(mProgressBar, mWidget)
};

#define mHProgressBar mProgressBar
#define mHProgressBarClass mProgressBarClass
#define g_stmHProgressBarCls  g_stmProgressBarCls

#define mVProgressBar mProgressBar
#define mVProgressBarClass mProgressBarClass
#define g_stmVProgressBarCls  g_stmProgressBarCls

/**
 * \enum mProgressBarProp
 * \brief ProgressBar Properties define
 *
 */
enum mProgressBarProp
{
	/**
	 * The max pos of progress
	 * 	- Type int
	 * 	- ReadWrite
	 */
    NCSP_PROG_MAXPOS = NCSP_WIDGET_MAX + 1, /**< value for maximum position*/
	/**
	 * The min pos of progress
	 *   - Type int
	 *   - ReadWrite
	 */
    NCSP_PROG_MINPOS,                       /**< value for minimum position*/
	/**
	 * The current position of progress
	 * 	- Type int
	 * 	- ReadWrite
	 */
    NCSP_PROG_CURPOS,                       /**< value for current position*/
	/**
	 * The line step length of progress
	 *	- Type int
	 *	- ReadWrite
	 */
    NCSP_PROG_LINESTEP,                     /**< step length */
    NCSP_PROG_MAX
};

/**
 * \def NCSS_PRGBAR_HORIZONTAL
 * \brief horizontal progressbar
 */
#define NCSS_PRGBAR_HORIZONTAL          0x0000L

/**
 * \def NCSS_PRGBAR_VERTICAL
 * \brief vertical progressbar
 */
#define NCSS_PRGBAR_VERTICAL            (0x0001L<<NCSS_WIDGET_SHIFT)

/**
 * \def NCSS_PRGBAR_BLOCKS
 * \brief progressbar show with blocks
 */
#define NCSS_PRGBAR_BLOCKS              (0x0002L<<NCSS_WIDGET_SHIFT)

/**
 * \def NCSS_PRGBAR_SMOOTH
 * \brief progressbar show smooth
 */
#define NCSS_PRGBAR_SMOOTH              (0x0000L<<NCSS_WIDGET_SHIFT)

/**
 * \def NCSS_PRGBAR_SHOWPERCENT
 * \brief progressbar with percentage "0%~100%"
 */
#define NCSS_PRGBAR_SHOWPERCENT         (0x0004L<<NCSS_WIDGET_SHIFT)

#define NCSS_PRGBAR_SHIFT  (NCSS_WIDGET_SHIFT+5)

    /** @} end of Control_Progressbar */

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif /* _MGUI_NCSCTRL_PROGRESSBAR_H */
#endif //_MGNCSCTRL_PROGRESSBAR
