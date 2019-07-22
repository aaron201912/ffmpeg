/*!============================================================================
 * \file mgeff-animation.h
 * \brief A motion curve headfile.
 * \author Houhuihua
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
#ifndef _MGEFF_ANIMATION_H_
#define _MGEFF_ANIMATION_H_

#include <stdint.h>
#include "mgeff-motioncurve.h"

/**
 * \brief The handle to the animation.
 */
MGEFF_DECLARE_HANDLE(MGEFF_ANIMATION);

/**
 * \def MGEFF_INFINITE
 * \brief infinite value, can be used for set loopcount property.
 */
#define MGEFF_INFINITE -1

/**
 * \enum EffAnimProperty
 * \brief animation property
 */
enum EffAnimProperty {
    MGEFF_PROP_DURATION   = 0, /**< r/w duration of animation   */
    MGEFF_PROP_CURLOOP    = 1, /**< r   current loop            */
    MGEFF_PROP_LOOPCOUNT  = 2, /**< r/w loop count of animation */
    MGEFF_PROP_DIRECTION  = 3, /**< r/w direction of animation  */
    MGEFF_PROP_CURFRAME   = 4, /**< r   current frame           */
    MGEFF_PROP_STATE      = 5, /**< r   current state           */
    MGEFF_PROP_KEEPALIVE  = 6, /**< r/w keepalive when finished */
    MGEFF_PROP_MAX
};

/**
 * \enum EffState
 * \brief animation state
 */
enum EffState {
    MGEFF_STATE_STOPPED   = 0, /**< animation is stopped  */
    MGEFF_STATE_PAUSED    = 1, /**< animation is paused   */
    MGEFF_STATE_RUNNING   = 2, /**< animation is running  */
    MGEFF_STATE_READY     = 3, /**< animation is ready    */
    MGEFF_STATE_REMOVE    = 4  /**< animation is remove   */
};

/**
 * \enum EffDirection
 * \brief animation direction
 */
enum EffDirection {
    MGEFF_DIR_FORWARD     = 0, /**< forward  */
    MGEFF_DIR_BACKWARD    = 1  /**< backward */
};

/**
 * \enum EffVariantType
 * \brief animation variant
 */
enum EffVariantType{
    MGEFF_INT             = 0, /**< int       */
    MGEFF_FLOAT           = 1, /**< float     */
    MGEFF_DOUBLE          = 2, /**< double    */
    MGEFF_POINT           = 3, /**< POINT     */
    MGEFF_POINTF          = 4, /**< POINTF    */
    MGEFF_3DPOINT         = 5, /**< 3DPOINT   */
    MGEFF_3DPOINTF        = 6, /**< 3DPOINTF  */
    MGEFF_RECT            = 7, /**< RECT      */
    MGEFF_COLOR           = 8, /**< COLOR     */
    MGEFF_MAX
};

/**
 * \enum EffAnimationType
 * \brief group animation
 */
enum EffAnimationType {
    MGEFF_PARALLEL        = 1, /**< parallel animation   */
    MGEFF_SEQUENTIAL      = 2  /**< sequential animation */
};

/**
 * \typedef void (*MGEFF_FINISHED_CB)(MGEFF_ANIMATION handle)
 * \brief Type of animation finished callback function.
 *
 * The callback is going to be called,when animation is finished.
 *
 * \param handle animation handle
 */
typedef void (*MGEFF_FINISHED_CB)(MGEFF_ANIMATION handle);

/**
 * \typedef void (*MGEFF_STATECHANGED_CB)(MGEFF_ANIMATION handle,
 *          enum EffState newEffState, enum EffState oldEffState)
 * \brief Type of animation state change callback function.
 *
 * The callback is going to be called, when animation state specified by \a handle
 * is changed.
 *
 * \param handle animation handle.
 * \param newEffState new animation state.
 * \param oldEffState old animation state.
 */
typedef void (*MGEFF_STATECHANGED_CB)(MGEFF_ANIMATION handle,
        enum EffState newEffState, enum EffState oldEffState);

/**
 * \typedef void (*MGEFF_CURLOOPCHANGED_CB)(MGEFF_ANIMATION handle)
 * \brief Type of animation current loop changed callback function.
 *
 * The callback is going to be called, when animation's current loop is changed.
 *
 * \param handle animation handle.
 */
typedef void (*MGEFF_CURLOOPCHANGED_CB)(MGEFF_ANIMATION handle);

/**
 * \typedef void (*MGEFF_DIRCHANGED_CB)(MGEFF_ANIMATION handle)
 * \brief Type of animation direction changed callback function.
 *
 * The callback is going to be called, when animation's direction is changed.
 *
 * \param handle animation handle.
 */
typedef void (*MGEFF_DIRCHANGED_CB)(MGEFF_ANIMATION handle);

/**
 * \typedef void (*MGEFF_SETPROPERTY_CB)(MGEFF_ANIMATION handle,
 *                  void *target, intptr_t id, void *value)
 * \brief Type of animation set property function. This fuction is used to produce
 * every animation frame when animation is running
 *
 * The callback is going to be called every frame.
 *
 * \param handle animation handle.
 * \param target animation target.
 * \param id animation id
 * \param value animation current value,startvalue <= value <= endvalue
 */
typedef void (*MGEFF_SETPROPERTY_CB)(MGEFF_ANIMATION handle,
        void *target, intptr_t id, void *value);

/**
 * \typedef void (*MGEFF_CALCVALUE_CB)(MGEFF_ANIMATION handle,
 *                  void *s, void *e, void *v, float factor)
 * \brief Type of animation motion curve callback function.
 *
 * The callback is going to be called every frame, when the animation specified by \a handle
 * is created by mGEffAnimationCreateEx
 *
 * \param handle animation handle
 * \param s value
 * \param e value
 * \param v value
 * \param factor calc factor
 */
typedef void (*MGEFF_CALCVALUE_CB)(MGEFF_ANIMATION handle,
        void *s, void *e, void *v, float factor);

/**
 * \typedef (*MGEFF_VARMALLOC_CB)(MGEFF_ANIMATION handle)
 * \brief Type of animation memory allocating callback function.
 *
 * The callback is going to be called, when the animation is creating.
 * The animation created by mGEffAnimationCreateEx will specify the callback.
 *
 * \param handle animation handle
 *
 * \return address of memory
 */
typedef void *(*MGEFF_VARMALLOC_CB)(MGEFF_ANIMATION handle);

/**
 * \fn MGEFF_ANIMATION mGEffAnimationCreateEx(void *target,
 *             MGEFF_SETPROPERTY_CB setproperty, intptr_t id, int size,
 *             MGEFF_VARMALLOC_CB cb1, MGEFF_CALCVALUE_CB cb2);
 * \brief Create an animation.
 *
 * This funcion creates an animation for the target specified by \a target.
 * The animation's setproperty call back is specified by \a setproperty, \a id
 * is the identifier for the animation, the size of the animation property
 * varible is specified by \a size, memory allocating function used to
 * allocating memory for animation internal use is specified by \a cb1,
 * motion curve for the animation is specified by \a cb2.
 *
 * \param target The target to be animated.
 * \param setproperty The animation set property call back, this function is
 *        used to genareate every frame of the animation.
 * \param id The identifier of the animation, which may be used in SetProperty
 *        callback to distinguish different animation.
 * \param size The size of the animation property variable.
 * \param cb1 The memory allocating callback function.
 * \param cb2 The motion curve value calculating callback function.
 *
 * \return handle to the newly created animation
 *
 * \sa  mGEffAnimationCreate.
 */
MGEFF_EXPORT MGEFF_ANIMATION mGEffAnimationCreateEx(void *target,
        MGEFF_SETPROPERTY_CB setproperty, intptr_t id, int size,
        MGEFF_VARMALLOC_CB cb1, MGEFF_CALCVALUE_CB cb2);

/**
 * \fn MGEFF_ANIMATION mGEffAnimationCreate(void *target,
 *             MGEFF_SETPROPERTY_CB setproperty, intptr_t id,
 *             enum EffVariantType varianttype);
 * \brief Creates an animation.
 *
 * This funcion creates an animation for the target specified by \a target.
 * The animation's setproperty call back is specified by \a setproperty, id is
 * the identifier for the animation,  and the property of the animation is
 * specified by varianttype.
 *
 * \param target The target to be animated.
 * \param setproperty The animation set property call back, this function is
 *      used to genareate every frame of the animation.
 * \param id The identifier of the animation, which may be used in SetProperty
 *      callback to distinguish different animation.
 * \param varianttype The data type of the property variable.
 *
 * \return handle to the newly created animation.
 *
 * \sa  mGEffAnimationCreateEx.
 */
MGEFF_EXPORT MGEFF_ANIMATION mGEffAnimationCreate(void *target,
        MGEFF_SETPROPERTY_CB setproperty, intptr_t id,
        enum EffVariantType varianttype);

/**
 * \fn void  mGEffAnimationDelete(MGEFF_ANIMATION handle)
 * \brief Deletes an animation.
 *
 * This function deletes an animation specified by \a handle.
 *
 * \param handle The animation to be deleted.
 *
 * \sa  mGEffAnimationCreate, mGEffAnimationCreateEx.
 */
MGEFF_EXPORT void mGEffAnimationDelete(MGEFF_ANIMATION handle);

/**
 * \fn void mGEffAnimationStop(MGEFF_ANIMATION handle)
 * \brief Stops a running animation.
 *
 * This function stops a running animation specified by \a handle.
 *
 * \param handle The animation to be deleted.
 *
 * \sa  mGEffAnimationAsyncRun, mGEffAnimationSyncRun.
 */
MGEFF_EXPORT void mGEffAnimationStop(MGEFF_ANIMATION handle);

/**
 * \fn void mGEffAnimationPause(MGEFF_ANIMATION handle)
 * \brief Pauses a running animation.
 *
 * This function pauses a running animation specified by \a handle.
 *
 * \param handle The animation to be paused.
 *
 * \sa  mGEffAnimationResume.
 */
MGEFF_EXPORT void mGEffAnimationPause(MGEFF_ANIMATION handle);

/**
 * \fn void mGEffAnimationResume(MGEFF_ANIMATION handle)
 * \brief Resumes a paused animation.
 *
 * This function resumes a paused animation specified by \a handle.
 *
 * \param handle The animation to be resumed.
 *
 * \sa  mGEffAnimationPause.
 */
MGEFF_EXPORT void mGEffAnimationResume(MGEFF_ANIMATION handle);

/**
 * \fn void mGEffAnimationSetProperty(MGEFF_ANIMATION handle,
 *             enum EffAnimProperty id, int value)
 * \brief Sets property of an animation.
 *
 * This function sets the property specified by \a id to new value
 * specified \a value.
 *
 * \param handle The handle of the animation.
 * \param id The identifier of the property.
 * \param value The new value to be set.
 *
 * \sa  mGEffAnimationGetProperty.
 */
MGEFF_EXPORT void mGEffAnimationSetProperty(MGEFF_ANIMATION handle,
        enum EffAnimProperty id, int value);

/**
 * \fn int mGEffAnimationGetProperty(MGEFF_ANIMATION handle,
 *             enum EffAnimProperty id);
 * \brief Gets property of an animation.
 *
 * This function gets the animation property specified by \a id.
 *
 * \param handle The handle of the animation.
 * \param id The identifier of the property.
 *
 * \return value The property value.
 *
 * \sa  mGEffAnimationSetProperty.
 */
MGEFF_EXPORT int mGEffAnimationGetProperty(MGEFF_ANIMATION handle,
        enum EffAnimProperty id);

/**
 * \fn void mGEffAnimationSetDuration(MGEFF_ANIMATION handle,
 *             int duration_ms).
 * \brief Sets the animation's duration.
 *
 * This function sets the animation's duration to new value specified
 * by \a duration_ms, the unit is millisecond.
 *
 * \param handle The handle of the animation.
 * \param duration_ms The new duration.
 *
 * \sa  mGEffAnimationGetDuration.
 */
static inline void mGEffAnimationSetDuration(MGEFF_ANIMATION handle,
        int duration_ms)
{
    mGEffAnimationSetProperty(handle, MGEFF_PROP_DURATION, duration_ms);
}

/**
 * \fn int mGEffAnimationGetDuration(MGEFF_ANIMATION handle).
 * \brief Gets the animation's duration.
 *
 * This function gets the animation's duration. Tthe unit is millisecond.
 *
 * \param handle The handle of the animation.
 *
 * \return The duration in millisecond.
 *
 * \sa  mGEffAnimationSetDuration.
 */
static inline int mGEffAnimationGetDuration(MGEFF_ANIMATION handle)
{
    return mGEffAnimationGetProperty(handle, MGEFF_PROP_DURATION);
}

/**
 * \fn void mGEffAnimationSetStartValue(MGEFF_ANIMATION handle,
 *             const void *value)
 * \brief set the animation start value.
 *
 * This function set the start value of the animation specified by \a
 * handle to \a value.
 *
 * \param  handle the handle of the animation.
 * \param  value the start value to be set.
 *
 * \sa  mGEffAnimationGetStartValue, mGEffAnimationSetEndValue.
 */
MGEFF_EXPORT void mGEffAnimationSetStartValue(MGEFF_ANIMATION handle,
        const void *value);

/**
 * \fn const void *mGEffAnimationGetStartValue(MGEFF_ANIMATION handle)
 * \brief Get the animation start value.
 *
 * This function get the start value of the animation specified by \a handle
 *
 * \param  handle the handle of the animation.
 *
 * \return start value of the animation specified by \a handle
 *
 * \sa  mGEffAnimationSetStartValue.
 */
MGEFF_EXPORT const void *mGEffAnimationGetStartValue(MGEFF_ANIMATION handle);

/**
 * \fn void mGEffAnimationSetEndValue(MGEFF_ANIMATION handle,
 *             const void *value)
 * \brief set the animation end value.
 *
 * This function set the end value of the animation specified by \a handle
 * to \a value.
 *
 * \param  handle the handle of the animation.
 * \param  value the end value.
 *
 * \sa  mGEffAnimationGetEndValue, mGEffAnimationSetStartValue.
 */

MGEFF_EXPORT void mGEffAnimationSetEndValue(MGEFF_ANIMATION handle,
        const void *value);

/**
 * \fn const void *mGEffAnimationGetEndValue(MGEFF_ANIMATION handle)
 * \brief Get the animation end value.
 *
 * This function get the end value of the animation specified by \a handle.
 *
 * \param  handle The handle of the animation.
 *
 * \return The end value of the animation specified by \a handle.
 *
 * \sa  mGEffAnimationSetEndValue.
 */
MGEFF_EXPORT const void *mGEffAnimationGetEndValue(MGEFF_ANIMATION handle);


/**
 * \fn void mGEffAnimationSetFinishedCb(MGEFF_ANIMATION handle,
 *             MGEFF_FINISHED_CB cb)
 * \brief Sets the animation Finished callback function.
 *
 * This function sets the Finished callback of the animation specified by \a
 * handle to \a cb.
 *
 * \param handle The handle of the animation.
 * \param cb The Finished callback to be set.
 *
 * \sa  mGEffAnimationSetStateChangedCb, mGEffAnimationSetDirChangedCb.
 */
MGEFF_EXPORT void mGEffAnimationSetFinishedCb(MGEFF_ANIMATION handle,
        MGEFF_FINISHED_CB cb);

/**
 * \fn void mGEffAnimationSetStateChangedCb(MGEFF_ANIMATION handle,
 *             MGEFF_STATECHANGED_CB cb);
 * \brief Sets the animation StateChanged callback function.
 *
 * This function sets the StateChanged callback of the animation specified by \a
 * handle to \a cb.
 *
 * \param handle The handle of the animation.
 * \param cb The StateChanged callback to be set.
 *
 * \sa  mGEffAnimationSetDirChangedCb, mGEffAnimationSetFinishedCb.
 */
MGEFF_EXPORT void  mGEffAnimationSetStateChangedCb(MGEFF_ANIMATION handle,
        MGEFF_STATECHANGED_CB cb);

/**
 * \fn void mGEffAnimationSetCurLoopChangedCb(MGEFF_ANIMATION handle,
 *             MGEFF_CURLOOPCHANGED_CB cb);
 * \brief Sets the animation CurLoopChanged callback function.
 *
 * This function sets the CurLoopChanged callback of the animation specified by \a
 * handle to \a cb.
 *
 * \param handle The handle of the animation.
 * \param cb The CurLoopChanged callback to be set.
 *
 * \sa  mGEffAnimationSetDirChangedCb, mGEffAnimationSetFinishedCb.
 */
MGEFF_EXPORT void mGEffAnimationSetCurLoopChangedCb(MGEFF_ANIMATION handle,
        MGEFF_CURLOOPCHANGED_CB cb);

/**
 * \fn void mGEffAnimationSetDirChangedCb(MGEFF_ANIMATION handle,
 *             MGEFF_DIRCHANGED_CB cb);
 * \brief Sets the animation DirChanged callback function.
 *
 * This function sets the DirChanged callback of the animation specified by \a
 * handle to \a cb.
 *
 * \param handle The handle of the animation.
 * \param cb The DirChanged callback to be set.
 *
 * \sa  mGEffAnimationSetCurLoopChangedCb, mGEffAnimationSetFinishedCb.
 */
MGEFF_EXPORT void  mGEffAnimationSetDirChangedCb(MGEFF_ANIMATION handle,
        MGEFF_DIRCHANGED_CB cb);

/**
 * \fn void mGEffAnimationSetContext(MGEFF_ANIMATION handle,
 *             void *context)
 * \brief Sets the animation context.
 *
 * This function sets the context of the animation specified by \a handle to
 * \a context.
 *
 * \param handle The hanle of the animation.
 * \param  context The context to be set.
 *
 * \sa  mGEffAnimationGetContext.
 */
MGEFF_EXPORT void mGEffAnimationSetContext(MGEFF_ANIMATION handle,
        void *context);

/**
 * \fn void *mGEffAnimationGetContext(MGEFF_ANIMATION handle)
 * \brief Gets the animation context.
 *
 * This function gets the context of the animation specified by \a handle.
 *
 * \param handle The hanle of the animation.
 *
 * \return  The context of the animation.
 *
 * \sa  mGEffAnimationSetContext.
 */
MGEFF_EXPORT void *mGEffAnimationGetContext(MGEFF_ANIMATION handle);

/**
 * \fn void mGEffAnimationSetCurve(MGEFF_ANIMATION handle,
 *             enum EffMotionType type)
 * \brief Sets the motion cureve of the animation.
 *
 * This function sets the motion curve of the animation specified by \a handle
 * to the curve specified by \a type.
 *
 * \param handle The hanle of the animation.
 * \param type the type of the motion curve to be set.
 *
 * \note there are some predefined motion curves in mGEff and each one has a
 * unique type.
 *
 * \sa  mGEffAnimationSetCurveEx.
 */
MGEFF_EXPORT void mGEffAnimationSetCurve(MGEFF_ANIMATION handle,
        enum EffMotionType type);

/**
 * \fn void mGEffAnimationSetCurveEx(MGEFF_ANIMATION handle,
 *             MGEFF_MOTIONCURVE_CB cb)
 * \brief Sets the motion cureve of the animation.
 *
 * This function sets the motion curve of the animation specified by \a handle
 * to the curve specified by \a cb.
 *
 * \param handle The hanle of the animation.
 * \param cb the function of the curve be set.
 *
 * \note use this to set your own motion curve.
 *
 * \sa  mGEffAnimationSetCurve.
 */
MGEFF_EXPORT void  mGEffAnimationSetCurveEx(MGEFF_ANIMATION handle,
        MGEFF_MOTIONCURVE_CB cb);

/**
 * \fn MGEFF_MOTIONCURVE mGEffAnimationGetCurve(MGEFF_ANIMATION handle)
 * \brief Gets the motion cureve of the animation.
 *
 * This function gets the motion curve of the animation specified by \a handle.
 *
 * \param handle The hanle of the animation.
 *
 * \return the handle of the motion curve of the animation.
 *
 * \sa  mGEffAnimationSetCurveEx, mGEffAnimationSetCurve.
 */
MGEFF_EXPORT MGEFF_MOTIONCURVE mGEffAnimationGetCurve(MGEFF_ANIMATION handle);

/**
 * \fn void *mGEffAnimationGetTarget(MGEFF_ANIMATION handle)
 * \brief Gets the target of the animation.
 *
 * This function gets the target of the animation specified by \a handle.
 *
 * \param handle The handle of the animation.
 *
 * \return the target of the animation specified by \a handle.
 *
 * \sa  mGEffAnimationCreate, mGEffAnimationCreateEx.
 */
MGEFF_EXPORT void *mGEffAnimationGetTarget(MGEFF_ANIMATION handle);

/**
 * \fn int mGEffAnimationAsyncRun(MGEFF_ANIMATION handle)
 * \brief Run the animation in the default schedular asynchronous.
 *
 * This function attachs the animation to the default schedular and start the animation.
 * The animation run immediately if schedular is already started, otherwise it will
 * wait until the schedular started.
 *
 * \param handle The handle of the animation to be attached.
 *
 * \return If succeed, return 0; otherwise return -1.
 *
 * \sa  mGEffAnimationSyncRun, mGEffAnimationSetPreporty.
 */
MGEFF_EXPORT int  mGEffAnimationAsyncRun(MGEFF_ANIMATION handle);

/**
 * \fn int mGEffAnimationSyncRun(MGEFF_ANIMATION handle)
 * \brief Run the animation in the default schedular synchronous.
 *
 * This function attachs the animation to the default schedular and start the animation.
 * The animation run immediately and return until the animation finished.
 *
 * \param handle The handle of the animation to be attached.
 *
 * \return If succeed, return 0; otherwise return -1.
 *
 * \sa  mGEffAnimationAsyncRun, mGEffAnimationSetPreporty.
 */
MGEFF_EXPORT int  mGEffAnimationSyncRun(MGEFF_ANIMATION handle);

/**
 * structure defines a all parameters to create a group animation ready to run.
 */
typedef struct _PropertyAnimationSetting {
    void                  *property;       /**< property              */
    enum EffVariantType    type;           /**< variant type          */
    const void            *start_value;    /**< start value           */
    const void            *end_value;      /**< end value             */
    enum EffMotionType     curve;          /**< motion type           */
    int                    duration;       /**< duration of animation */
    MGEFF_FINISHED_CB      cb;             /**< finished callback     */
} EffPropertyAnimationSetting;

/**
 * \fn MGEFF_ANIMATION mGEffCreatePropertyAnimation(const
 *             EffPropertyAnimationSetting *settings)
 * \brief Create an animation group ready to run.
 *
 * This function creates an animation group ready to run using the input param \a settings.
 *
 * \param settings parameters for create and setup an group animation, it contains one or 
 * more animation property.
 *
 * \return the handle of the created animation group.
 *
 * \sa  mGEffAnimationCreate, mGEffAnimationCreate, mGEffAnimationSetProperty.
 */
MGEFF_EXPORT MGEFF_ANIMATION mGEffCreatePropertyAnimation(const EffPropertyAnimationSetting *settings);

/**
 * \fn MGEFF_BOOL mGEffAnimationWait(void *hWnd, MGEFF_ANIMATION handle)
 * \brief Wait an animation.
 *
 * This function wait an animation specified by \a handle, it will
 * return util the special animation run over, it can receive other
 * message between the wait time.
 *
 * \param hWnd   the window handle of that animation's timer relative.
 * \param handle the handle of the animation to be wait.
 *
 * \sa  mGEffAnimationAsyncRun.
 */
MGEFF_EXPORT MGEFF_BOOL mGEffAnimationWait(void *hWnd, MGEFF_ANIMATION handle);

#endif

