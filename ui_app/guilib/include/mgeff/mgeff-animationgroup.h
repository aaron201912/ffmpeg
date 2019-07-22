/*!============================================================================
 * \file mgeff-animationgroup.h
 * \brief animation group headfile.
 * \author DongKai
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

#ifndef _MGEFF_ANIMATION_GROUP_H_
#define _MGEFF_ANIMATION_GROUP_H_

/**
 * \fn MGEFF_EXPORT mGEffAnimationCreateGroup(enum EffAnimationType type)
 * \brief Create mgeff animation group.
 *
 * This function Creates an animation group according to the \a type..
 *
 * \param type Animation group type. It can be the following values:
 *
 *      -MGEFF_PARALLEL
 *      Parallel animation group: all animation in the group will run
 *      simultaneously.
 *
 *      -MGEFF_SEQUENTIAL
 *      Sequential animation group: all animation in the group will run one
 *      after another as when they are added to this animation group.
 *
 * \return the handle to the newly created animation group.
 *
 * \sa mGEffAnimationCreateEx   mGEffAnimationCreate.
 */
MGEFF_ANIMATION MGEFF_EXPORT mGEffAnimationCreateGroup(enum EffAnimationType type);

/**
 * \fn void MGEFF_EXPORT mGEffAnimationAddToGroup(MGEFF_ANIMATION group,
 *             MGEFF_ANIMATION animation)
 * \brief Adds an animation to an animation group.
 *
 * This function Adds an animation specified by \a animation to an existing
 * animation group specified by \a group.
 *
 * \param group animation group to which new animation will be added.
 * \param animation the animation to be added.
 *
 * \sa mGEffAnimationCreateGroup.
 */
void MGEFF_EXPORT mGEffAnimationAddToGroup(MGEFF_ANIMATION group,
        MGEFF_ANIMATION animation);

#endif

