/*!============================================================================
 * \file mgeff-window-animation.h
 * \brief window animation headfile.
 * \author DongKai
 * \version 1.0
 * \date 03/18/2010 03:45:12 PM
 *
 \verbatim

    This file is part of mGEff, a component for MiniGUI.

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

#ifndef _MGEFF_WINDOW_ANIMATION_H_
#define _MGEFF_WINDOW_ANIMATION_H_

/**
 * \brief The handle to the motioncurve.
 */
MGEFF_DECLARE_HANDLE(MGEFF_WINDOW_ANIMATION_CONTEXT);

/**
 * \brief Structure defines windows animation context
 */
typedef struct _MGEFF_WindowAnimCtxt {
    HDC  src_dc;      /**< First frame of show/hide animation. */
    HDC  dst_dc;      /**< Last frame of show/hide animation. */
    HDC  sink_dc;     /**< The target DC for animation output */
    HWND hwnd;        /**< Animation window handle. */
    int  duration;    /**< duration of animation */
    int  curve_type;  /**< curve type */
} MGEFF_WindowAnimCtxt;

/**
 * \typedef void (*SET_BKGND_CB)(HDC hdc, RECT *rc)
 * \brief call back used when updating backgroudn.
 *
 * \param hdc The background DC, a DC on which background is drawn.
 * \param rc The rectangle to be updated in the background.
 */
typedef void (*SET_BKGND_CB)(HDC hdc, RECT *rc);

/**
 * \typedef void (*DRAW_WND_CB)(HDC hdc1, HDC hdc2, RECT *rc)
 * \brief call back used to drawing window when window is moving.
 *
 * \param hdc1 The DC holds the content of the animated window.
 * \param hdc2 The DC to be drawn.
 * \param rect The area to be drawn.
 */
typedef void (*DRAW_WND_CB)(HDC hdc1, HDC hdc2, RECT *rc);

/**
 * \typedef void (*RENDER_HANDLER)(MGEFF_EFFECTOR effector)
 * \brief call back used to handle animation property.
 *
 * \param The effector used to handle animation property.
 */
typedef void (*RENDER_HANDLER)(MGEFF_EFFECTOR effector);

/**
 * \typedef void (*MGEFF_WINDOWANIM_CB)(MGEFF_WindowAnimCtxt *ctxt);
 * \brief Get the window foreground content(means snapshot) DC.
 *
 * \param hwnd Which window will be snapshot.
 *
 * \return The DC of window snapshot content.
 */
typedef void (*MGEFF_WINDOWANIM_CB)(MGEFF_WindowAnimCtxt *ctxt);

/**
 * \fn HDC mGEffGetWindowForeground(HWND hwnd, BOOL set_foreground)
 *
 * \brief Get window foreground.
 *
 * This function return a DC which contain the foreground of the window
 * specified by \a hwnd.
 *
 * \param hwnd window handle.
 * \param set_foreground whether change the visible property of window.
 *
 * \return a DC contains the foreground of the window.
 *
 * \sa .
 */
HDC mGEffGetWindowForeground(HWND hwnd, BOOL set_foreground);

/**
 * \fn MGEFF_WINDOW_ANIMATION_CONTEXT mGEffCreateWindowAnimationContext(int time,
 *             int effector_type, int curve_type,
 *             SET_BKGND_CB set_bkgnd_cb, DRAW_WND_CB draw_wnd_cb)
 * \brief Create window animation context.
 *
 * This function creates a window animation context
 *
 * \param time duration of window animation.
 * \param effector_type the type of the effector used in window animation creation.
 * \param curve_type motion curve type of window animation.
 * \param set_bkgnd_cb the function for drawing background.
 * \param draw_wnd_cb function for drawing window.
 *
 * \return handle to the newly created window animation context.
 *
 * \note window animation context contains all the necessary information
 * to animate a window.
 *
 * \sa mGEffMoveWindowPreparation.
 */
MGEFF_WINDOW_ANIMATION_CONTEXT mGEffCreateWindowAnimationContext(int time,
        int effector_type, int curve_type,
        SET_BKGND_CB set_bkgnd_cb, DRAW_WND_CB draw_wnd_cb);

/**
 * \fn void mGEffDestroyWindowAnimationContext(MGEFF_WINDOW_ANIMATION_CONTEXT hctxt);
 * \brief Deletes window animation context.
 *
 * This function deletes a window animation context
 *
 * \param hctxt handle of the window animation context to be deleted.
 *
 * \note window animation context contains all the necessary information
 * to animate a window.
 *
 * \sa mGEffMoveWindowPreparation, mGEffCreateWindowAnimationContext.
 */
void mGEffDestroyWindowAnimationContext(MGEFF_WINDOW_ANIMATION_CONTEXT hctxt);


/**
 * \fn HWND mGEffCreateMainWindow (PMAINWINCREATE pCreateInfo,
 *             MGEFF_WINDOW_ANIMATION_CONTEXT hctxt)
 * \brief create a main window and show the window with animation.
 *
 * This function creates a window according to \a pCreateInfo and show the
 * window with animation created according to \a hctxt.
 *
 * \param pCreateInfo main window creating info.
 * \param hctxt handle of the window animation context.

 * \note window animation context contains all the necessary information
 * to animate a window.
 *
 * \return handle of windows
 *
 * \sa mGEffMoveWindowPreparation, mGEffCreateWindowAnimationContext.
 */
HWND mGEffCreateMainWindow (PMAINWINCREATE pCreateInfo,
        MGEFF_WINDOW_ANIMATION_CONTEXT hctxt);


/**
 * \fn BOOL mGEffShowWindow (HWND hwnd, int iCmdShow,
 *             MGEFF_WINDOW_ANIMATION_CONTEXT hctxt)
 * \brief show main window with animation.
 *
 * This function shows or hide an window with animation. \a iCmdShow determins
 * show or hide the window and the \a hctxt is the handle of window animation
 * context based on which the window animation is created.
 *
 * \param hwnd the handle of the window.
 * \param iCmdShow show or hide the window.
 * \param hctxt handle of the window animation context
 *
 * \note window animation context contains all the necessary information
 * to animate a window.
 *
 * \return TRUE when succeed, otherwise FALSE.
 *
 * \sa mGEffMoveWindowPreparation, mGEffCreateWindowAnimationContext.
 */
BOOL mGEffShowWindow (HWND hwnd, int iCmdShow,
        MGEFF_WINDOW_ANIMATION_CONTEXT hctxt);

/**
 * \fn BOOL mGEffMoveWindow (HWND hWnd, int x, int y, int w, int h,
 *             BOOL fPaint, MGEFF_WINDOW_ANIMATION_CONTEXT hctxt);
 * \brief Moves window with animation.
 *
 * This function moves the window specified by \a hWnd with animaiton.
 * The end value of the animation is the window position and size specified
 * by \a x, \a y, \a w, \a h respectively. The context of the window
 * animation is specified by \a hctext.
 *
 * \param hWnd window handle.
 * \param x The x postion of the window when animation is finnished.
 * \param y The y postion of the window when animation is finnished.
 * \param w The w width of the window when animation is finnished.
 * \param h The h height of the window when animation is finnished.
 * \param fPaint Indicates whether the window should be repainted.
 * \param hctxt The window animation context.
 *
 * \return TRUE when succeed, otherwise FALSE.
 *
 * \sa mGEffCreateWindowAnimationContext.
 */
BOOL mGEffMoveWindow (HWND hWnd, int x, int y, int w, int h,
        BOOL fPaint, MGEFF_WINDOW_ANIMATION_CONTEXT hctxt);

/**
 * \fn BOOL ControlExtract(HWND mainHwnd, int ctrl_id, PMAINWINCREATE pCreateInfo)
 * \brief Extracts a control from main window, and create a
 * new main window take that control content.
 *
 * This function creates a window according to \a pCreateInfo which
 * contains the control specified by \a ctrl_id in window specifed by
 * \a mainHwnd.
 *
 * \param mainHwnd handle of the window which contains the control.
 * \param ctrl_id id of the control.
 * \param pCreateInfo main window creating info for creating new window containing the control.
 *
 * \return TRUE when succeed, otherwise FALSE.
 *
 * \sa WindowExtract.
 */
BOOL ControlExtract(HWND mainHwnd, int ctrl_id, PMAINWINCREATE pCreateInfo);

/**
 * \fn BOOL WindowExtract(HWND mainHwnd, RECT *rc, PMAINWINCREATE pCreateInfo)
 * \brief Extracts a rectangle from a main window, and create a
 * new main window take the content in that rectangle.
 *
 * This function creates a window according to \a pCreateInfo which
 * contains the content in rectangle specified by \a rc in main window specifed
 * by \a mainHwnd.
 *
 * \param mainHwnd handle of the main window from which then content will be
 * extracted.
 * \param rc the rectangle in which the content will be extract.
 * \param pCreateInfo main window creating info for creating new window
 * containing the extracted content.
 *
 * \return TRUE when succeed, otherwise FALSE.
 *
 * \sa ControlExtract.
 */
BOOL WindowExtract(HWND mainHwnd, RECT *rc, PMAINWINCREATE pCreateInfo);

/**
 * \fn BOOL mGEffShowWindowEx (HWND hwnd, int iCmdShow,
        MGEFF_WINDOW_ANIMATION_CONTEXT hctxt, MGEFF_WINDOWANIM_CB cb)
 *
 * \brief show main window with animation which has a user defined window
 * animation callback function.
 *
 * This function shows or hide an window with animation. \a iCmdShow determins
 * show or hide the window. \a hctxt is the handle of window animation
 * context based on which the window animation is created and \a cb is the window
 * animation callback.
 *
 * \param hwnd the handle of the window.
 * \param iCmdShow show or hide the window.
 * \param hctxt handle of the window animation context.
 * \param cb user defined window animation callback.
 *
 * \return TRUE if success, else return FALSE
 *
 * \sa mGEffShowWindow.
 */
BOOL mGEffShowWindowEx (HWND hwnd, int iCmdShow,
        MGEFF_WINDOW_ANIMATION_CONTEXT hctxt, MGEFF_WINDOWANIM_CB cb);

/**
 * \fn HWND mGEffCreateMainWindowEx(PMAINWINCREATE pCreateInfo,
 *             MGEFF_WINDOW_ANIMATION_CONTEXT hctxt, MGEFF_WINDOWANIM_CB cb)
 * \brief show main window with animation which has a user defined window
 * animation callback function.
 *
 * This function shows or hide an window with animation. \a iCmdShow determins
 * show or hide the window. \a hctxt is the handle of window animation
 * context based on which the window animation is created and \a cb is the window
 * animation callback.
 *
 * \param pCreateInfo information of create window.
 * \param hctxt handle of the window animation context.
 * \param cb user defined window animation callback.
 *
 * \return handle of windows
 *
 * \sa mGEffShowWindow.
 */
HWND mGEffCreateMainWindowEx(PMAINWINCREATE pCreateInfo,
        MGEFF_WINDOW_ANIMATION_CONTEXT hctxt, MGEFF_WINDOWANIM_CB cb);

/**
 * \fn RENDER_HANDLER mGEffSetEffectorPropertyHandler(MGEFF_WINDOW_ANIMATION_CONTEXT hctxt,
 *             RENDER_HANDLER new_cb);
 * \brief Sets a new animation handle.
 *
 * This function sets a new animation handler specified by \a new_cb to animation
 * context specified by \a hctxt.
 *
 * \param hctxt handle of the window animation context to be set.
 * \param new_cb new window animation handler
 *
 * \note window animation context contains the necessary information
 * to animate a window.
 *
 * \sa mGEffCreateWindowAnimationContext.
 */
RENDER_HANDLER mGEffSetEffectorPropertyHandler(MGEFF_WINDOW_ANIMATION_CONTEXT hctxt,
        RENDER_HANDLER new_cb);

/**
 * \fn MGEFF_ANIMATION mGEffMoveWindowPreparation(HWND hWnd,
 *             int x, int y, int w, int h,
 *             BOOL fPaint, MGEFF_WINDOW_ANIMATION_CONTEXT hctxt);
 * \brief create an window animation ready to run.
 *
 * This function creates an animation for the window specified by \a hWnd.
 * The end value of the animation is the window position and size specified
 * by \a x, \a y, \a w, \a h respectively. The context of the window animation
 * is specified by \a hctext.
 *
 * \param hWnd window handle.
 * \param x The x postion of the window when animation is finnished.
 * \param y The y postion of the window when animation is finnished.
 * \param w The w width of the window when animation is finnished.
 * \param h The h height of the window when animation is finnished.
 * \param fPaint Indicates whether the window should be repainted.
 * \param hctxt The window animation context.
 *
 * \return a hanle of the animation for the window.
 *
 * \sa mGEffCreateWindowAnimationContext.
 */
MGEFF_ANIMATION mGEffMoveWindowPreparation(HWND hWnd,
        int x, int y, int w, int h,
        BOOL fPaint, MGEFF_WINDOW_ANIMATION_CONTEXT hctxt);

#endif

