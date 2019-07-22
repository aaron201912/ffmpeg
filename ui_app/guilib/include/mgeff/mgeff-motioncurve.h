/*!============================================================================
 * \file mgeff-motioncurve.h
 * \brief A motion curve headfile.
 * \author DongKai
 * \version 1.0
 * \date 02/03/2010 05:18:25 PM
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

#ifndef _MGEFF_MOTIONCURVE_H_
#define _MGEFF_MOTIONCURVE_H_

/**
 * \brief The handle to the motioncurve.
 */
MGEFF_DECLARE_HANDLE(MGEFF_MOTIONCURVE);

/**
 * \typedef float (*MGEFF_MOTIONCURVE_CB)(float progress)
 * \brief Type of the value calculating callback of the motion curve fuction.
 * \param progress variable
 */
typedef float (*MGEFF_MOTIONCURVE_CB)(float progress);

/**
 * \enum EffCurveProperty
 * \brief the type of motion curve property
 */
enum EffCurveProperty {
    MGEFFCURVE_PROP_OVERSHOOT  = 0, /**< overshoot */
    MGEFFCURVE_PROP_PERIOD     = 1, /**< period */
    MGEFFCURVE_PROP_AMPLITUDE  = 2  /**< amplitude */
};

/**
 * \enum EffMotionType
 * \brief the type of predefined motion curve
 */
enum EffMotionType {
    Linear, /**< Easing curve for a linear (t) function: velocity is constant. */
    InQuad, /**< Easing curve for a quadratic (t^2) function: accelerating from zero velocity. */
    OutQuad,/**< Easing curve for a quadratic (t^2) function: decelerating to zero velocity. */
    InOutQuad, /**< Easing curve for a quadratic (t^2) function: acceleration until halfway, then deceleration. */
    OutInQuad, /**< Easing curve for a quadratic (t^2) function: deceleration until halfway, then acceleration. */
    InCubic, /**< Easing curve for a cubic (t^3) function: accelerating from zero velocity. */
    OutCubic, /**< Easing curve for a cubic (t^3) function: decelerating from zero velocity. */
    InOutCubic, /**< Easing curve for a cubic (t^3) function: acceleration until halfway, then deceleration. */
    OutInCubic, /**< Easing curve for a cubic (t^3) function: deceleration until halfway, then acceleration. */
    InQuart, /**< Easing curve for a quartic (t^4) function: accelerating from zero velocity. */
    OutQuart, /**< Easing curve for a cubic (t^4) function: decelerating from zero velocity. */
    InOutQuart, /**< Easing curve for a cubic (t^4) function: acceleration until halfway, then deceleration. */
    OutInQuart, /**< Easing curve for a cubic (t^4) function: deceleration until halfway, then acceleration. */
    InQuint, /**< Easing curve for a quintic (t^5) easing in: accelerating from zero velocity. */
    OutQuint, /**< Easing curve for a cubic (t^5) function: decelerating from zero velocity. */
    InOutQuint, /**< Easing curve for a cubic (t^5) function: acceleration until halfway, then deceleration. */
    OutInQuint, /**< Easing curve for a cubic (t^5) function: deceleration until halfway, then acceleration. */
    InSine, /**< Easing curve for a sinusoidal (sin(t)) function: accelerating from zero velocity. */
    OutSine, /**< Easing curve for a sinusoidal (sin(t)) function: decelerating from zero velocity. */
    InOutSine, /**< Easing curve for a sinusoidal (sin(t)) function: acceleration until halfway, then decelerating. */
    OutInSine, /**< Easing curve for a sinusoidal (sin(t)) function: deceleration until halfway, then acceleration. */
    InExpo, /**< Easing curve for an exponential (2^t) function: accelerating from zero velocity. */
    OutExpo, /**< Easing curve for an exponential (2^t) function: decelerating from zero velocity. */
    InOutExpo, /**< Easing curve for an exponential (2^t) function: acceleration until halfway, then decelerating. */
    OutInExpo, /**< Easing curve for an exponential (2^t) function: deceleration until halfway, then acceleration. */
    InCirc, /**< Easing curve for a circular (sqrt(1-t^2)) function: accelerating from zero velocity. */
    OutCirc, /**< Easing curve for a circular (sqrt(1-t^2)) function: decelerating from zero velocity. */
    InOutCirc, /**< Easing curve for a circular (sqrt(1-t^2)) function: acceleration until halfway, then decelerating. */
    OutInCirc, /**< Easing curve for a circular (sqrt(1-t^2)) function: deceleration until halfway, then acceleration. */
    InElastic, /**< Easing curve for an elastic (exponentially decaying sine wave) function: accelerating
                 from zero velocity. The peak amplitude can be set with the amplitude parameter, and the
                 period of decay by the period parameter.*/
    OutElastic, /**< Easing curve for an elastic (exponentially decaying sine wave) function: decelerating
                  from zero velocity. The peak amplitude can be set with the amplitude parameter, and the
                  period of decay by the period parameter. */
    InOutElastic, /**< Easing curve for an elastic (exponentially decaying sine wave) function: acceleration
                    until halfway, then deceleration. */
    OutInElastic, /**< Easing curve for an elastic (exponentially decaying sine wave) function: deceleration
                    until halfway, then acceleration. */
    InBack, /**< Easing curve for a back (overshooting cubic function: (s+1)*t^3 - s*t^2) easing in: accelerating from zero velocity.*/
    OutBack, /**< Easing curve for a back (overshooting cubic function: (s+1)*t^3 - s*t^2) easing out: decelerating to zero velocity.*/
    InOutBack, /**< Easing curve for a back (overshooting cubic function: (s+1)*t^3 - s*t^2) easing in/out:
                 acceleration until halfway, then deceleration. */
    OutInBack, /**< Easing curve for a back (overshooting cubic easing: (s+1)*t^3 - s*t^2) easing out/in:
                 deceleration until halfway, then acceleration. */
    InBounce, /**< Easing curve for a bounce (exponentially decaying parabolic bounce) function: accelerating from zero velocity. */
    OutBounce, /**< Easing curve for a bounce (exponentially decaying parabolic bounce) function:decelerating from zero velocity. */
    InOutBounce, /**< Easing curve for a bounce (exponentially decaying parabolic bounce) function easing in/out:
                   acceleration until halfway, then deceleration. */
    OutInBounce, /**< Easing curve for a bounce (exponentially decaying parabolic bounce) function easing out/in:
                   deceleration until halfway, then acceleration.*/
    InCurve, /**< In Curve */
    OutCurve, /**< Out Curve */
    SineCurve, /**< Sine Curve */
    CosineCurve, /**< Cosine Curve */
    Custom,  /**< This is returned if the user specified a custom curve type. */
    NCurveTypes
};

/**
 * \fn void mGEffCurveSetProperty (MGEFF_MOTIONCURVE handle,
 *             enum EffCurveProperty, float value)
 * \brief set property of an animation.
 *
 * This function set the property specified by \a EffCurveProperty of the
 * motion curve specified by \a handle to a new value specified \a value.
 *
 * \param handle The motion curve handle.
 * \param EffCurveProperty The type of the property to be set.
 * \param value New property value.
 *
 * \sa mGEffAnimationAsyncRun, mGEffAnimationSyncRun.
 */
void mGEffCurveSetProperty (MGEFF_MOTIONCURVE handle,
        enum EffCurveProperty, float value);

/**
 * \fn float mGEffCurveGetProperty (MGEFF_MOTIONCURVE handle, enum EffCurveProperty)
 * \brief Get property of an animation.
 *
 * This function get the property value specified by \a EffCurveProperty of the
 * motion curve specified by \a handle.
 *
 * \param handle The motion curve handle .
 * \param EffCurveProperty The property type.
 * \return property value.
 *
 * \sa mGEffAnimationAsyncRun, mGEffAnimationSyncRun.
 */
float mGEffCurveGetProperty (MGEFF_MOTIONCURVE handle, enum EffCurveProperty);

#endif

