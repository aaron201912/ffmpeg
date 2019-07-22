/*!============================================================================
 * \file mgeff.h
 * \brief The interface relative headfile.
 * \author WanZheng
 * \version 1.0
 * \date 02/03/2010 04:48:51 PM
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
#ifndef _MGEFF_H_
#define _MGEFF_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "mgeff-common.h"
#include "mgeff-animation.h"
#include "mgeff-animationgroup.h"
#include "mgeff-motioncurve.h"
#ifdef _MGEFF_EFFECTOR
    #include "mgeff-effector-sink.h"
    #include "mgeff-effector-source.h"
    #include "mgeff-effector.h"
    #include "mgeff-window-animation.h"
#endif

#ifdef __cplusplus
}
#endif

#endif

