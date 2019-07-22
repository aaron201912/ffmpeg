/*
 * \file 
 * \author FMSoft
 * \date 
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

#ifndef MSPEEDMETER_H
#define MSPEEDMETER_H

#define EXPORT /* TODO */

#ifdef __cplusplus
extern "C" {
#endif

/* 
 * Easy APIs
 */
EXPORT int QueryMouseMoveVelocity(float *v_x, float *v_y);

/* Proc for MiniGUI */
EXPORT int SpeedMeterProc(HWND hWnd, int message, WPARAM wParam, LPARAM lParam);

#ifdef NCSCTRL_WIDGET
/* Handler for mGNCS */
EXPORT BOOL SpeedMeterMessageHandler(mWidget*, int message, int x, int y, DWORD keyStatus);
#endif

#if 0
EXPORT int QueryMouseMovePath(POINT *points, unsigned int *times, int count);
#endif

/* 
 * Low level APIs
 */
/* DECLARE_HANDLE(SPEEDMETER) */
typedef struct _notused_mSpeedMeter_handle{ int not_used; } *SPEEDMETER;

EXPORT SPEEDMETER mSpeedMeter_create(int duration_ms, int precision_ms);
EXPORT void mSpeedMeter_destroy(SPEEDMETER handle);
EXPORT void mSpeedMeter_append(SPEEDMETER handle, int x, int y, unsigned int t);
EXPORT void mSpeedMeter_stop(SPEEDMETER handle);
EXPORT void mSpeedMeter_reset(SPEEDMETER handle);
EXPORT int mSpeedMeter_velocity(SPEEDMETER handle, float *v_x, float *v_y);
#if 0
EXPORT int  mSpeedMeter_getpath(SPEEDMETER handle, POINT *points, unsigned int *times, int count);
#endif

#ifdef __cplusplus
}
#endif

#endif /* MSPEEDMETER_H */
