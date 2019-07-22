/*!============================================================================
 * \file mgeff-effector-sink.h
 * \brief Effecr sink headfile.
 * \author WanZheng
 * \version 1.0
 * \date 02/03/2010 05:22:39 PM
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
#ifndef _MGEFF_EFFECTORSINK_H_
#define _MGEFF_EFFECTORSINK_H_

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>

/**
 * \brief The handle to the effector sink.
 */
MGEFF_DECLARE_HANDLE(MGEFF_SINK);

/**
 * \enum MGEFF_SINK_TYPE
 * \brief sink type
 */
enum MGEFF_SINK_TYPE{
    MGEFF_SINK_HDC  = 0,
    MGEFF_SINK_HWND = 1,
    MGEFF_SINK_MAX
};

/**
 * \fn MGEFF_SINK mGEffCreateHDCSink(HDC hdc)
 * \brief Create sink from a DC.
 *
 * This function creates sink ready to be used by a effector from a DC
 * specified by \a hdc.
 *
 * \param hdc the underlying DC of the sink to be created.
 *
 * \return the handle of the created sink.
 *
 * \note sink can be considered as a canvas on which the effector draws.
 *
 * \sa mGEffCreateHDCSink.
 */
MGEFF_EXPORT MGEFF_SINK mGEffCreateHDCSink(HDC hdc);

/**
 * \fn MGEFF_SINK mGEffCreateHwndSink(HWND hwnd)
 * \brief Create sink from a window handle.
 *
 * This function creates sink ready to be used by a effector from a window
 * handle specified by \a hwnd.
 *
 * \param hwnd the underlying window of the sink to be created.
 *
 * \return the handle of the created sink.
 *
 * \note sink can be considered as a canvas on which the effector draws.
 *
 * \sa mGEffCreateHDCSink, mGEffEffectorCreateEx.
 */
MGEFF_EXPORT MGEFF_SINK mGEffCreateHwndSink(HWND hwnd);

/**
 * \fn void mGEffDeleteSink(MGEFF_SINK sink)
 * \brief Deletes a sink.
 *
 * This function deletes a sink specified by \a sink.
 *
 * \param sink The handle of the sink to be deleted.
 *
 * \sa mGEffCreateHDCSink, mGEffCreateHwndSink.
 */
MGEFF_EXPORT void mGEffDeleteSink(MGEFF_SINK sink);

/**
 * \fn void mGEffSetBufferSink(MGEFF_SINK handle, HDC hdc)
 * \brief Sets the content of a sink according to a DC.
 *
 * This function sets content of a sink specified by \a handle according to
 * a DC specified by \a hdc.
 *
 * \param handle The handle of the sink to be set.
 * \param hdc The handle of the DC used to set the sink.
 *
 * \sa mGEffDeleteSink.
 */
MGEFF_EXPORT void mGEffSetBufferSink(MGEFF_SINK handle, HDC hdc);

#endif

