/*!============================================================================
 * \file mgeff-effector-source.h
 * \brief Effector source headfile.
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
#ifndef _MGEFF_EFFECTORSOURCE_H_
#define _MGEFF_EFFECTORSOURCE_H_

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>

/**
 * The handle to the effector source.
 */
MGEFF_DECLARE_HANDLE(MGEFF_SOURCE);

/**
 * \typedef void (*MGEFF_DELETEHDC_CB)(HDC hdc)
 * \brief delete source dc
 *
 * \param hdc source hdc
 */
typedef void (*MGEFF_DELETEHDC_CB)(HDC hdc);

/**
 * \fn MGEFF_SOURCE mGEffCreateSource(HDC hdc)
 * \brief Creates a source ready to be used by a effector.
 *
 * This function creates a effector source whose underlying DC is the DC
 * specified by \a hdc.
 *
 * \param hdc The handle of the DC.
 *
 * \return the handle fo the newly created effector source.
 *
 * \sa mGEffCreateSourceEx.
 */
MGEFF_EXPORT MGEFF_SOURCE mGEffCreateSource(HDC hdc);

/**
 * \fn MGEFF_SOURCE mGEffCreateSourceEx(HDC hdc, MGEFF_DELETEHDC_CB cb)
 * \brief Creates a source ready to be used by a effector and privies the
 * source with a callback to delete DC when the source is deleted.
 *
 * This function creates a effector source whose underlying DC is the DC
 * specified by \a hdc and the dc will be deleted using the callback specified
 * by \a cb when the source is deleted.
 *
 * \param hdc The handle of the DC.
 * \param cb The callback to delte the underlying dc when source is deleted.
 *
 * \return the handle fo the newly created effector source.
 *
 * \sa mGEffCreateSource.
 */
MGEFF_EXPORT MGEFF_SOURCE mGEffCreateSourceEx(HDC hdc, MGEFF_DELETEHDC_CB cb);

/**
 * \fn HDC mGEffGetSourceDC(MGEFF_SOURCE handle);
 * \brief Get HDC from the source handle.
 *
 * This function return DC from source specified by \a handle.
 *
 * \param handle The handle of the source.
 *
 * \return dc of source.
 *
 * \sa.
 */
MGEFF_EXPORT HDC mGEffGetSourceDC(MGEFF_SOURCE handle);

#endif

