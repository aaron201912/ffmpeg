/**
 * \file mtimer.h
 * \author dongjunjie
 * \date 2009/04/19
 *
 * This file include the base object defines
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

#ifndef _MGUI_NCSCTRL_TIMER_H
#define _MGUI_NCSCTRL_TIMER_H

#ifdef __cplusplus
extern "C"{
#endif

/**
 * \defgroup mTimer mTimer
 * @{
 */

/**
 * \def NCSCTRL_TIMER
 * \brief the class name of \ref mTimer
 */
#define NCSCTRL_TIMER  NCSCLASSNAME("timer")

typedef struct _mTimerClass mTimerClass;
typedef struct _mTimer mTimer;

#define mTimerClassHeader(clss, superCls) \
	mInvsbCompClassHeader(clss, superCls) \
	BOOL (*start)(clss *self); \
	void (*stop)(clss *self);  \
	HWND (*getParent)(clss *self);

/**
 * \struct mTimerClass
 * \brief the class of Timer object
 *
 *  - BOOL \b start (\a mTimer * self);\n
 *    Start the timer
 *    \param self - the this pointer
 *    \return TRUE - timer started sucess, FALSE - failed
 *
 *  - void \b stop (\a mTimer * self);\n
 *    \param self - the this pointer
 *
 *  - HWND \b getParent (\a mTimer * self);\n
 *    Get the handle of window who owner this timer
 *    \param self - the this pointer
 *    \return the owner's handle
 */

struct _mTimerClass {
	mTimerClassHeader(mTimer, mInvsbComp)
};

/**
 * \var g_stmTimerCls
 * \brief the singleton of \ref mTimerClass
 */
MGNCS_EXPORT extern mTimerClass g_stmTimerCls; //Class(mTimer);

#define mTimerHeader(clss) \
	mInvsbCompHeader(clss) \
	DWORD interval;

/**
 * \struct mTimer
 * \brief the timer member define
 *
 *  - DWORD \b interval - the interval of timer
 *
 * \sa mInvsbComp, mTimerClass
 */
struct _mTimer {
	mTimerHeader(mTimer)
};

/**
 * \enum ncsTimerProp
 * \brief the \ref mTimer's property define
 */
enum ncsTimerProp {
	NCSP_TIMER_INTERVAL = NCSP_IVCMPT_MAX + 1,
	NCSP_TIMER_MAX
};
/**
 * \var NCSP_TIMER_INTERVAL
 * \brief the interval of timer
 *    - Type : DWORD
 *    - ReadWrite
 *    - Restart the timer when set it
 */

/**
 * \typedef BOOL (*NCS_CB_ONTIMER)(mTimer* timer, DWORD total_count);
 * \brief the callback of event \a MSG_TIMER
 *
 * \param timer the sender of timer
 * \param total_count the total tick count from timer started
 *
 * \return TRUE - continue timer, FALSE - stop the timer
 */
typedef BOOL (*NCS_CB_ONTIMER)(mTimer* timer, DWORD total_count);

/** @} end of Timer */

#ifdef __cplusplus
}
#endif

#endif

