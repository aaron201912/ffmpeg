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

#ifndef CHIPMUNK_UTILS_H
#define CHIPMUNK_UTILS_H

#include <chipmunk/chipmunk.h>
#include <chipmunk/chipmunk_structs.h>

typedef struct _cp_baffle_board{
    cpConstraint *spring;
    cpBody *bodies[2];
    cpShape *shapes[2];
} cp_baffle_board_t;

cp_baffle_board_t *create_baffle_board(cpSpace *space, int y_baseline, int y_height, int restLength, int x1, int x2, float stiffness, float damping);
cpShape *create_floor(cpSpace *space, int y_baseline, int x1, int x2);
cpShape *create_ball(cpSpace *space, int x, int y, int r, int m);
cpShape *create_block(cpSpace *space, int x1, int y1, int x2, int y2, int m);

void destroy_baffle_board(cpSpace *space, cp_baffle_board_t *board);
void destroy_static_shape(cpSpace *space, cpShape *shape);
void destroy_shape(cpSpace *space, cpShape *shape);

#endif /* CHIPMUNK_UTILS_H */
