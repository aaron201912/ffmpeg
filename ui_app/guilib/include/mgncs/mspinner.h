/**
 * \file mspinner.h
 * \author Wangjian<wangjian@minigui.org>
 * \date 2009/03/01
 *
 * This file includes the definition of mSpinner control.
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

#ifdef _MGNCSCTRL_SPINNER

#ifndef _MGUI_NCSCTRL_SPINNER_H
#define _MGUI_NCSCTRL_SPINNER_H

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

/**
 * \defgroup Control_Spinner mSpinner
 * @{
 */

/*
 * \def NCSCTRL_SPINNER
 * \brief the name of spinner control
*/
#define NCSCTRL_SPINNER   NCSCLASSNAME("spinner")

typedef struct _mSpinner            mSpinner;
typedef struct _mSpinnerClass       mSpinnerClass;
typedef struct _mSpinnerRenderer    mSpinnerRenderer;

#define mSpinnerHeader(clsName) \
    mWidgetHeader(clsName)      \
    HWND h_target;


/**
 * \struct mSpinner
 * \brief the spinner struct of spinner control, derived from \ref mWidget.
 *
 *   - \b h_target \n
 *
 *  \sa mSpinnerClass
 */
struct _mSpinner
{
	mSpinnerHeader(mSpinner)
};

#define mSpinnerClassHeader(clsName, parentClass) \
	mWidgetClassHeader(clsName, parentClass)

/**
 * \struct mSpinnerClass
 * \brief the VTable of \a mSpinner, derived from \ref mWidgetClass.
 *
 * \sa mWidgetClass
 */
struct _mSpinnerClass
{
	mSpinnerClassHeader(mSpinner, mWidget)
};

MGNCS_EXPORT extern mSpinnerClass g_stmSpinnerCls;

#define mHSpinner mSpinner
#define mHSpinnerClass mSpinnerClass
#define g_stmHSpinnerCls  g_stmSpinnerCls
#define mVSpinner mSpinner
#define mVSpinnerClass mSpinnerClass
#define g_stmVSpinnerCls  g_stmSpinnerCls



#define mSpinnerRendererHeader(clsName, parentClass)    \
	mWidgetRendererHeader(clsName, parentClass)

/**
 * \struct mSpinnerRenderer
 * \brief SpinBox class's Renderer interface, derived from \ref mWidgetRenderer.
 *
 * \sa mSpinner, mSpinnerClass
 */
struct _mSpinnerRenderer
{
	mSpinnerRendererHeader(mSpinner, mWidget)
};

/** SpinBox Property define:*/
enum mSpinnerProp {
    NCSP_SPNR_MAXPOS = NCSP_WIDGET_MAX + 1, /**< maximum value*/
    NCSP_SPNR_MINPOS,    /**< minimum value*/
    NCSP_SPNR_CURPOS,    /**< current value*/
    NCSP_SPNR_LINESTEP,  /**< line step length*/
    NCSP_SPNR_TARGET,    /**< target window's handler*/
    NCSP_SPNR_MAX
};

#define KS_SPINPOST             0x00010000


/**
 * \def NCSS_SPNR_VERTICAL
 * \brief vertical spinner
 */
#define NCSS_SPNR_VERTICAL       0x0000L

/**
 * \def NCSS_SPNR_HORIZONTAL
 * \brief horizontal spinner
 */
#define NCSS_SPNR_HORIZONTAL     ((0x0001L)<<(NCSS_WIDGET_SHIFT))

/**
 * \def NCSS_SPNR_AUTOLOOP
 * \brief auto loop spinner
 */
#define NCSS_SPNR_AUTOLOOP       ((0x0002L)<<(NCSS_WIDGET_SHIFT))

#define NCSS_SPNR_SHIFT (NCSS_WIDGET_SHIFT + 2)


/** SpinBox Notify Codes:*/
enum mSpinnerNotify {
	NCSN_SPNR_CHANGED = NCSN_WIDGET_MAX + 1, /**< value changed */
	NCSN_SPNR_REACHMAX,      /**< reached maximum value */
	NCSN_SPNR_REACHMIN,      /**< reached minimum value */
	NCSN_SPNR_MAX
};

//////////////////////////////////////////////
////////////------ status ------ /////////////
//////////////////////////////////////////////

#define NCSST_SPNR_NORMAL        0x0000
#define NCSST_SPNR_DISABLE       0x0001
#define NCSST_SPNR_HILIGHT       0x0002
#define NCSST_SPNR_PUSHED        0x0004

/** @} end of Control_Spinner*/

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif /* _MGUI_NCSCTRL_SPINNER_H */
#endif //_MGNCSCTRL_SPINNER
