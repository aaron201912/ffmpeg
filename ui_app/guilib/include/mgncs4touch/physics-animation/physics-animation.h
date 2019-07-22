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

#ifndef PHYSICS_ANIMATION_H
#define PHYSICS_ANIMATION_H

#ifndef EXPORT
#   define EXPORT /* TODO */
#endif

#include <chipmunk/chipmunk.h>
#include <mgeff/mgeff.h>

#ifdef __cplusplus
extern "C" {
#endif

EXPORT MGEFF_ANIMATION phyanim_create(cpSpace *space, void *context,
        int (*onCalc)(MGEFF_ANIMATION anim, cpSpace *space, void *context),
        void (*onDraw)(MGEFF_ANIMATION anim, cpSpace *space, void *context));
EXPORT void phyanim_destroy(MGEFF_ANIMATION animation);
EXPORT cpSpace *phyanim_getspace(MGEFF_ANIMATION animation);

#ifdef __cplusplus
}
#endif

#endif /* PHYSICS_ANIMATION_H */
