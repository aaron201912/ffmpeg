/*!============================================================================
 * \file mgeff-effector.h
 * \brief Effector headfile.
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
#ifndef _MGEFF_EFFECTOR_H_
#define _MGEFF_EFFECTOR_H_

#include <stdint.h>

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>

#include "mgeff-common.h"
#include "mgeff-animation.h"
#include "mgeff-effector-source.h"
#include "mgeff-effector-sink.h"

/**
 * \brief The handle to the effector.
 */
MGEFF_DECLARE_HANDLE(MGEFF_EFFECTOR);

/**
 * \brief effector callback
 * \note you will implement them, if you want to define custom effector.
 */

/**
 * \typedef int (*MGEFF_EFFECTOR_SETPROPERTY)(MGEFF_EFFECTOR effector,
 *                  int property_id, int value)
 * \brief Type of effector set property callback function .
 *
 * \param effector effector handle.
 * \param property_id property id.
 * \param value property value.
 *
 * \return If success return 0, otherwise return -1.
 * */
typedef int (*MGEFF_EFFECTOR_SETPROPERTY)(MGEFF_EFFECTOR effector,
        int property_id, int value);

/**
 * \typedef int (*MGEFF_EFFECTOR_GETPROPERTY)(MGEFF_EFFECTOR effector,
 *                  int property_id, int* pValue)
 * \brief Type of effector get property callback functio.
 *
 * \param effector effector handle.
 * \param property_id property id.
 * \param pValue property value.
 *
 * \return If success return *pValue, otherwise return -1.
 * */
typedef int (*MGEFF_EFFECTOR_GETPROPERTY)(MGEFF_EFFECTOR effector,
        int property_id, int* pValue);

/**
 * \typedef void (*MGEFF_EFFECTOR_ANIM_ONDRAW)(MGEFF_ANIMATION animation,
 *                  MGEFF_EFFECTOR effector, HDC sink_dc, intptr_t id, void *value)
 * \brief Type of effector on draw callback function.
 *
 * It is going to be called every frame
 *
 * \param animation animation handle
 * \param effector effector handle
 * \param sink_dc sink dc
 * \param id animation id
 * \param value animation current value, startvalue <= value <= endvalue
 * */
typedef void (*MGEFF_EFFECTOR_ANIM_ONDRAW)(MGEFF_ANIMATION animation,
        MGEFF_EFFECTOR effector, HDC sink_dc, intptr_t id, void *value);

/**
 * \typedef void (*MGEFF_EFFECTOR_ONBEGINDRAW)(MGEFF_ANIMATION animation,
 *                  MGEFF_EFFECTOR effector)
 * \brief Type of effector begin draw callback function
 *
 * It is going to be called when you create an animation with calling
 * mGEffAnimationCreateWithEffector.
 *
 * \param animation animation handle
 * \param effector effector handle
 */
typedef void (*MGEFF_EFFECTOR_ONBEGINDRAW)(MGEFF_ANIMATION animation,
        MGEFF_EFFECTOR effector);

/**
 * \typedef void (*MGEFF_EFFECTOR_ONENDDRAW)(MGEFF_ANIMATION animation,
 *                  MGEFF_EFFECTOR effector)
 * \brief Type of effector end draw callback function
 *
 * It is going to be called when the animation specified by \a animation is finished.
 * \param animation animation handle
 * \param effector effector handle
 */
typedef void (*MGEFF_EFFECTOR_ONENDDRAW)(MGEFF_ANIMATION animation,
        MGEFF_EFFECTOR effector);

/**
 * \typedef MGEFF_EFFECTOR (*MGEFF_EFFECTOR_INIT)(MGEFF_EFFECTOR effector)
 * \brief Type of effector init callback function
 *
 * It is going to be called when you create an effector with calling
 * mGEffEffectorCreate or mGEffEffectorCreateEx.
 *
 * \param effector
 *
 * \return if success, return effector handle, otherwise NULL.
 * */
typedef MGEFF_EFFECTOR (*MGEFF_EFFECTOR_INIT)(MGEFF_EFFECTOR effector);

/**
 * \typedef void (*MGEFF_EFFECTOR_FINALIZE)(MGEFF_EFFECTOR effector)
 * \brief Type of effector finalize callback function
 *
 * It is going to be called when you delete the effector with calling
 * mGEffEffectorDelete.
 *
 * \param effector effector handle
 * */
typedef void (*MGEFF_EFFECTOR_FINALIZE)(MGEFF_EFFECTOR effector);

#define EFFECTORNAME_MAX_LEN 31     /**< effector name max length */

/**
 * \brief it contains whole info of an effector, if you want to custom an effector, implement it
 */
typedef struct _EFFECTOROPS {
    char                         name[EFFECTORNAME_MAX_LEN + 1]; /**< name of effector */
    enum EffVariantType          varianttype;                    /**< variant type */
    MGEFF_EFFECTOR_INIT          init;                           /**< init */
    MGEFF_EFFECTOR_FINALIZE      finalize;                       /**< finalize */
    MGEFF_EFFECTOR_ANIM_ONDRAW   ondraw;                         /**< on draw */
    MGEFF_EFFECTOR_ONBEGINDRAW   begindraw;                      /**< begin draw */
    MGEFF_EFFECTOR_ONENDDRAW     enddraw;                        /**< end draw */
    MGEFF_EFFECTOR_SETPROPERTY   setproperty;                    /**< set property */
    MGEFF_EFFECTOR_GETPROPERTY   getproperty;                    /**< get property */
} MGEFF_EFFECTOROPS;


#define MGEFF_MINOR_leafwindow          "leafwindow"             /**< leaf window */
#define MGEFF_MINOR_zip                 "zip"                    /**< zip */
#define MGEFF_MINOR_flip                "flip"                   /**< flip */
#define MGEFF_MINOR_cubicrotate         "cubicrotate"            /**< cubic rotate */
#define MGEFF_MINOR_alpha               "alphablending"          /**< alpha blending */
#define MGEFF_MINOR_scroll              "scrollpage"             /**< scroll page */
#define MGEFF_MINOR_zoom                "zoominzoomout"          /**< zoom */
#define MGEFF_MINOR_push                "pushpoll"               /**< push */
#define MGEFF_MINOR_clear               "clear"                  /**< clear */
#define MGEFF_MINOR_centersplit         "centersplit"            /**< center split */
#define MGEFF_MINOR_radarscan           "radarscan"              /**< radar scan */
#define MGEFF_MINOR_fivepointedstar     "fivepointedstar"        /**< five pointed star */
#define MGEFF_MINOR_roller              "roller"                 /**< roller */
#define MGEFF_MINOR_blockflip           "blockflip"              /**< block flip */
#define MGEFF_MINOR_es_cubicrotate      "opengles-cubicrotate"   /**< OpenGL ES cubic rotate */
#define MGEFF_MINOR_es_rectrotate       "opengles-rectrotate"    /**< OpenGL ES rect rotate */
#define MGEFF_MINOR_es_coverflow        "opengles-coverflow"     /**< OpenGL ES cover flow */
#define MGEFF_MINOR_mgplus_rotate       "mgplus-rotate"          /**< mGPlus rotate */
#define MGEFF_MINOR_mgplus_cubicrotate  "mgplus-cubicrotate"     /**< mGPlus cubic rotate */
#define MGEFF_MINOR_mgplus_flip         "mgplus-flip"            /**< mGPlus flip */


#define MGEFF_EFFECTOR_LEAFWINDOW           mGEffStr2Key(MGEFF_MINOR_leafwindow)         /**< leaf window */
#define MGEFF_EFFECTOR_ZIP                  mGEffStr2Key(MGEFF_MINOR_zip)                /**< zip */
#define MGEFF_EFFECTOR_FLIP                 mGEffStr2Key(MGEFF_MINOR_flip)               /**< flip */
#define MGEFF_EFFECTOR_CUBIC_ROTATE         mGEffStr2Key(MGEFF_MINOR_cubicrotate)        /**< cubic rotate */
#define MGEFF_EFFECTOR_ALPHA                mGEffStr2Key(MGEFF_MINOR_alpha)              /**< alpha */
#define MGEFF_EFFECTOR_SCROLL               mGEffStr2Key(MGEFF_MINOR_scroll)             /**< scroll */
#define MGEFF_EFFECTOR_ZOOM                 mGEffStr2Key(MGEFF_MINOR_zoom)               /**< zoom */
#define MGEFF_EFFECTOR_PUSH                 mGEffStr2Key(MGEFF_MINOR_push)               /**< push */
#define MGEFF_EFFECTOR_CLEAR                mGEffStr2Key(MGEFF_MINOR_clear)              /**< clear */
#define MGEFF_EFFECTOR_CENTERSPLIT          mGEffStr2Key(MGEFF_MINOR_centersplit)        /**< center split */
#define MGEFF_EFFECTOR_RADARSCAN            mGEffStr2Key(MGEFF_MINOR_radarscan)          /**< radar scan */
#define MGEFF_EFFECTOR_FIVEPOINTEDSTAR      mGEffStr2Key(MGEFF_MINOR_fivepointedstar)    /**< five pointed star */
#define MGEFF_EFFECTOR_ROLLER               mGEffStr2Key(MGEFF_MINOR_roller)             /**< roller */
#define MGEFF_EFFECTOR_BLOCKFLIP            mGEffStr2Key(MGEFF_MINOR_blockflip)          /**< block flip */
#define MGEFF_EFFECTOR_OPENGL_CUBICROTATE   mGEffStr2Key(MGEFF_MINOR_es_cubicrotate)     /**< OpenGL ES cubic rotate */
#define MGEFF_EFFECTOR_OPENGL_RECTROTATE    mGEffStr2Key(MGEFF_MINOR_es_rectrotate)      /**< OpenGL ES rect rotate */
#define MGEFF_EFFECTOR_OPENGL_COVERFLOW     mGEffStr2Key(MGEFF_MINOR_es_coverflow)       /**< OpenGL ES cover flow */
#define MGEFF_EFFECTOR_MGPLUS_ROTATE        mGEffStr2Key(MGEFF_MINOR_mgplus_rotate)      /**< mGPlus rotate */
#define MGEFF_EFFECTOR_MGPLUS_CUBIC_ROTATE  mGEffStr2Key(MGEFF_MINOR_mgplus_cubicrotate) /**< mGPlus cubic rotate */
#define MGEFF_EFFECTOR_MGPLUS_FLIP          mGEffStr2Key(MGEFF_MINOR_mgplus_flip)        /**< mGPlus flip */

/* #define MGEFF_EFFECTOR_MAX 0 */
/**
 * \brief effector property
 */
#define MGEFF_PROPERTY_DIRECTION      1    /**< property direction */
#define MGEFF_PROPERTY_AXIS           2    /**< property axis */
#define MGEFF_PROPERTY_LEAFROWS       4    /**< property leaf rows */
#define MGEFF_PROPERTY_LEAFVERTICAL   5    /**< property leaf vertical */
#define MGEFF_PROPERTY_ZOOM           6    /**< property zoom */
#define MGEFF_PROPERTY_RESOURCE       7    /**< property resource */
#define MGEFF_PROPERTY_BACKGROUND     8    /**< property background */
#define MGEFF_PROPERTY_PIECES         9    /**< property pieces */
#define MGEFF_PROPERTY_STARTANGLE     10   /**< property startangle */

/**
 * \enum LeafProperty
 * \brief LeafProperty
 */
enum LeafProperty {
    MGEFF_DIRECTION_LEFT2RIGHT = 0,
    MGEFF_DIRECTION_RIGHT2LEFT = 1,
    MGEFF_DIRECTION_TOP2BOTTOM = 2,
    MGEFF_DIRECTION_BOTTOM2TOP = 3
};

/**
 * \enum CenterSplitProperty
 * \brief CenterSplitProperty
 */
enum CenterSplitProperty {
    MGEFF_DIRECTION_CENTER2OUTSIDE_4 = 0,
    MGEFF_DIRECTION_OUTSIDE2CENTER_4 = 1,
    MGEFF_DIRECTION_CENTER2OUTSIDE_2 = 2,
    MGEFF_DIRECTION_OUTSIDE2CENTER_2 = 3
};

/**
 * \enum AxisProperty
 * \brief AxisProperty
 */
enum AxisProperty {
    MGEFF_AXIS_X = 1,
    MGEFF_AXIS_Y = 2,
    MGEFF_AXIS_Z = 3
};

/**
 * \enum RotateDirectionProperty
 * \brief RotateDirectionProperty
 */
enum RotateDirectionProperty {
    MGEFF_CLOCKWISE = 0,
    MGEFF_CLOCKINVERSE = 1
};

/**
 * \enum ZoomProperty
 * \brief ZoomProperty
 */
enum ZoomProperty {
    MGEFF_DIRECTION_CENTER = 0,
    MGEFF_DIRECTION_LUP2RDOWN = 1,
    MGEFF_DIRECTION_LDOWN2RUP = 2,
    MGEFF_DIRECTION_RDOWN2LUP = 3,
    MGEFF_DIRECTION_RUP2LDOWN = 4
};

/**
 * \enum ZoomTypeProperty
 * \brief ZoomTypeProperty
 */
enum ZoomTypeProperty {
    MGEFF_ZOOMIN,   /**< zoom in   */
    MGEFF_ZOOMOUT   /**< zoom out  */
};

/**
 * \fn unsigned long mGEffStr2Key (const char* str)
 * \brief Generate hash key of string.
 *
 * This function return the hash key of specified by \a string.
 *
 * \param str The input string.
 *
 * \return The hash key of the special string
 *
 * \sa mGEffEffectorCreateEx, mGEffEffectorCreate.
 */
MGEFF_EXPORT unsigned long mGEffStr2Key (const char* str);

/**
 * \fn MGEFF_EFFECTOR mGEffEffectorCreate(unsigned long key)
 * \brief Creates a effector.
 *
 * This function creates a effector of certain type specified by \a type.
 *
 * \param key the hash key of the effector.
 *
 * \return handle the handle of the effector.
 *
 * \sa mGEffEffectorDelete.
 */
MGEFF_EXPORT MGEFF_EFFECTOR mGEffEffectorCreate(unsigned long key);

/**
 * \fn void mGEffEffectorDelete(MGEFF_EFFECTOR handle)
 * \brief Deletes a effector.
 *
 * This function deletes a effector specified by \a handle. source sink
 * and destination sink will also be deleted.
 *
 * \param handle the handle of the effector.
 *
 * \sa mGEffEffectorCreate.
 */
MGEFF_EXPORT void mGEffEffectorDelete(MGEFF_EFFECTOR handle);

/**
 * \fn void mGEffEffectorDeleteWithoutSink(MGEFF_EFFECTOR handle)
 * \brief Deletes a effector.
 *
 * This function deletes a effector specified by \a handle. source sink
 * and destination sink will NOT be deleted.
 *
 * \param handle the handle of the effector.
 *
 * \sa mGEffEffectorDelete.
 */
MGEFF_EXPORT void mGEffEffectorDeleteWithoutSink(MGEFF_EFFECTOR handle);


/**
 * \fn int mGEffEffectorAppendSource(MGEFF_EFFECTOR effector,
 *             MGEFF_SOURCE source)
 * \brief Appends a source to effector.
 *
 * This function appends a source specified by \a source to a effector
 * specified by \a effector.
 *
 * \note a effector usually has two sources.
 *
 * \param effector the handle of the effector to which source is appended.
 * \param source the source to be appended.
 *
 * \sa mGEffEffectorSetSink.
 */
MGEFF_EXPORT int mGEffEffectorAppendSource(MGEFF_EFFECTOR effector,
        MGEFF_SOURCE source);

/**
 * \fn int mGEffEffectorSetSink(MGEFF_EFFECTOR effector, MGEFF_SINK sink)
 * \brief Sets the sink of a effector.
 *
 * This function sets the sink of a effector specified by \a effector to sink
 * specified by \a sink.
 *
 * \note a effector contains one and only one sink which is used to present the
 * result the effector produce.
 *
 * \param effector the handle of the effector to which source is appended.
 * \param sink source the source to be appended.
 *
 * \sa mGEffEffectorGetSink.
 */
MGEFF_EXPORT int mGEffEffectorSetSink(MGEFF_EFFECTOR effector, MGEFF_SINK sink);

/**
 * \fn MGEFF_SINK mGEffEffectorGetSink(MGEFF_EFFECTOR effector);
 * \brief Gets the sink of a effector.
 *
 * This function gets the sink of a effector specified by \a effector.
 *
 * \param effector the handle of the effector.
 *
 * \return sink the handle of the sink of the effector.
 *
 * \sa mGEffEffectorSetSink.
 */
MGEFF_EXPORT MGEFF_SINK mGEffEffectorGetSink(MGEFF_EFFECTOR effector);

/**
 * \fn HDC mGEffEffectorGetSourceDC(MGEFF_EFFECTOR effector, int index)
 * \brief Gets the underlying DC of the source of a effector.
 *
 * This function gets the DC of the source of a effector specified by \a
 * effector by index specified by \a index.
 *
 * \note the sources of a effector are indexed according to the timing
 * when they are appended to the effector.
 *
 * \param effector the handle of the effector.
 * \param index of the source in the effector.
 *
 * \return the underlying DC of the source.
 *
 * \sa .
 */
MGEFF_EXPORT HDC mGEffEffectorGetSourceDC(MGEFF_EFFECTOR effector, int index);

/**
 * \fn int mGEffEffectorSetProperty(MGEFF_EFFECTOR handle,
 *             int property_id, int value)
 * \brief Sets the property of a effector.
 *
 * This function sets the property specified by \a property_id  of a effector
 * specified by \a effector to new value specified by \a value.
 *
 * \param handle the handle of the effector.
 * \param property_id the identifier of the property.
 * \param value new value of the property.
 * \return 0 when successful, otherwise -1;
 *
 * \sa mGEffEffectorAppendSource.
 */
MGEFF_EXPORT int mGEffEffectorSetProperty(MGEFF_EFFECTOR handle,
        int property_id, int value);

/**
 * \fn int mGEffEffectorGetProperty(MGEFF_EFFECTOR handle,
 *            int property_id, int *pValue)
 * \brief Sets the property of a effector.
 *
 * This function gets the property specified by \a property_id  of a effector
 * specified by \a effector.
 *
 * \param handle the handle of the effector.
 * \param property_id the identifier of the property.
 * \param pValue the address of the property value.
 *
 * \return 0 when successful, otherwise -1;
 *
 * \sa mGEffEffectorSetProperty.
 */
MGEFF_EXPORT int mGEffEffectorGetProperty(MGEFF_EFFECTOR handle,
        int property_id, int *pValue);

/**
 * \fn MGEFF_ANIMATION mGEffAnimationCreateWithEffector(MGEFF_EFFECTOR effector);
 * \brief Creates a animation with a effector.
 *
 * This function creates an animation whose SetProperty callback is a effector
 * specified by \a effector and the id of the animation is specified by \a id.
 *
 * \param effector the handle of the effector.
 *
 * \return the handle of the newly created animaiton.
 *
 * \sa mGEffAnimationCreate, mGEffEffectorCreate.
 */
MGEFF_EXPORT MGEFF_ANIMATION mGEffAnimationCreateWithEffector(MGEFF_EFFECTOR effector);

/**
 * \fn int mGEffFillEffectorAnimation(HWND hwnd, HDC src1_dc, HDC src2_dc,
 *             HDC dst_dc, unsigned long key, int duration)
 * \brief Fills a DC with a predefined effector.
 *
 * This function creates and run an animation whose SetProperty callback is a
 * effector specified by \a type. The sources of the effector are specified
 * by \a src1_dc and \a src2_dc respectively. The destination DC of the
 * effector is specified by \a dst_dc.  The duration of the animation is
 * specified by \a duration.
 *
 * \param hwnd The window handle in animation thread.
 * \param src1_dc The first DC of the effector.
 * \param src2_dc The secondary DC of the effector.
 * \param dst_dc The destination DC of the effector.
 * \param key The hash key of the effector to be used.
 * \param duration The duaration of the animation to be run.
 *
 * \return 0 if successful, otherwise -1;
 *
 * \sa mGEffAnimationCreate, mGEffEffectorCreate.
 */
MGEFF_EXPORT int mGEffFillEffectorAnimation(HWND hwnd, HDC src1_dc, HDC src2_dc,
        HDC dst_dc, unsigned long key, int duration);

/**
 * \fn MGEFF_EFFECTOR mGEffEffectorCreateEx(HDC src1_dc, HDC src2_dc,
 *             HDC dst_dc, unsigned long key)
 * \brief Creates a effector of certain type with given source DCs and
 * destination DC.
 *
 * This function creates a effector with source DCs specified by \a src1_dc
 * and by \a src2_dc and the destination DC specified by \a dst_dc. The
 * effector type is specified by \a type.
 * *
 * \param src1_dc The first DC of the effector.
 * \param src2_dc The secondary DC of the effector.
 * \param dst_dc The destination DC of the effector.
 * \param key The hash key of the effector to be used.
 *
 * \return The handle to the newly created effector.
 *
 * \sa mGEffAnimationCreate, mGEffEffectorCreate.
 */
MGEFF_EXPORT MGEFF_EFFECTOR mGEffEffectorCreateEx(HDC src1_dc, HDC src2_dc,
        HDC dst_dc, unsigned long key);

/**
 * \fn MGEFF_EFFECTOR mGEffEffectorCreateWithoutSink(HDC src1_dc,
 *             HDC src2_dc, unsigned long key)
 * \brief Creates a effector of certain type with given source DCs but
 * WITHOUT a destination DC.
 *
 * This function creates a effector with source DCs specified by \a src1_dc
 * and by \a src2_dc. The effector type is specified by \a type.
 * *
 * \param src1_dc The first DC of the effector.
 * \param src2_dc The secondary DC of the effector.
 * \param key The hash key of the effector to be used.
 *
 * \return The handle to the newly created effector.
 *
 * \sa mGEffEffectorCreateEx, mGEffEffectorCreate.
 */
MGEFF_EXPORT MGEFF_EFFECTOR mGEffEffectorCreateWithoutSink(HDC src1_dc,
        HDC src2_dc, unsigned long key);

/**
 * \fn MGEFF_BOOL mGEffEffectorRegister(const MGEFF_EFFECTOROPS *info)
 * \brief Register specail effector info.
 *
 * This function register a effector with specified by \a info,
 * Before user create effector it must be register first.
 *
 * \param info The effector info.
 *
 * \return MGEE_TRUE if UnRegister success, else return MGEFF_FALSE.
 *
 * \sa mGEffEffectorUnRegister.
 */
MGEFF_EXPORT MGEFF_BOOL mGEffEffectorRegister(const MGEFF_EFFECTOROPS *info);

/**
 * \fn MGEFF_BOOL mGEffEffectorUnRegister(const MGEFF_EFFECTOROPS *info)
 * \brief UnRegister specail effector info.
 *
 * This function unregister a effector with specified by \a info.
 *
 * \param info The effector info.
 *
 * \return MGEE_TRUE if UnRegister success, else return MGEFF_FALSE.
 *
 * \sa mGEffEffectorRegister.
 */
MGEFF_EXPORT MGEFF_BOOL mGEffEffectorUnRegister(const MGEFF_EFFECTOROPS *info);

/**
 * \fn MGEFF_ANIMATION mGEffCreateRollerAnimation(HDC dc_rollup,
 *             HDC dc_expand, HDC dc_dst);
 * \brief create a roller animation from specail dc.
 *
 * This function create a roller animation group with specified by \a dc_rollup,
 * and \a dc_expand, the background is white.
 *
 * \param dc_rollup The rollup dc.
 * \param dc_expand The expand dc.
 * \param dc_dst The output dc.
 *
 * \return return animation handle.
 *
 * \sa .
 */
MGEFF_EXPORT MGEFF_ANIMATION mGEffCreateRollerAnimation(HDC dc_rollup,
        HDC dc_expand, HDC dc_dst);

/**
 * \fn void mGEffEffectorSetContext(MGEFF_EFFECTOR handle,
 *             void *context)
 * \brief Sets the effector context.
 *
 * This function sets the context of the effector specified by \a handle to
 * \a context.
 *
 * \param handle The hanle of the effector.
 * \param  context The context to be set.
 *
 * \note
 * \sa mGEffEffectorGetContext.
 */
MGEFF_EXPORT void mGEffEffectorSetContext(MGEFF_EFFECTOR handle,
        void *context);

/**
 * \fn void *mGEffEffectorGetContext(MGEFF_EFFECTOR handle)
 * \brief Gets the effector context.
 *
 * This function gets the context of the effector specified by \a handle.
 *
 * \param handle The hanle of the effector.
 * \return  The context of the effector.
 *
 * \note
 * \sa mGEffEffectorSetContext.
 */
MGEFF_EXPORT void *mGEffEffectorGetContext(MGEFF_EFFECTOR handle);

/**
 * \fn MGEFF_SOURCE mGEffEffectorGetSource(MGEFF_EFFECTOR handle,
 *             int index)
 * \brief Gets the effector source.
 *
 * This function gets the source of effector specified by \a index of
 * source, first source add to effector is zero, next increase sequence.
 *
 * \param handle The hanle of the effector.
 * \param index  The index of the effector source.
 *
 * \return  The source handle of the source.
 *
 * \sa
 */
MGEFF_EXPORT MGEFF_SOURCE mGEffEffectorGetSource(MGEFF_EFFECTOR handle,
        int index);

/**
 * \fn MGEFF_EFFECTOROPS *mGEffEffectorGetOps(unsigned long key)
 * \brief Gets the effector MGEFF_EFFECTOROPS ops.
 *
 * This function gets the ops of the effector specified by hash \a key.
 *
 * \param key The hash key of the effector.
 *
 * \return  The ops of the effector.
 *
 * \sa
 */
MGEFF_EXPORT MGEFF_EFFECTOROPS *mGEffEffectorGetOps(unsigned long key);

#endif

