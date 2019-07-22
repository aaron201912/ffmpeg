/*
 * \file mgplus.h
 * \author FMSoft
 * \date 2008/12/02
 *
 * \brief This file includes macro definitions, typedefs and function 
 *        interfaces of mGPlus component. mGPlus is an advanced 2D graphics
 *        rendering engine for MiniGUI V3.0 or later.
 *
 \verbatim

    mGPlus is a component that expands and strengthens
    MiniGUI graphics function. It mainly provides support
    for 2D vector graphics and advanced graphics algorithm,
    such as the path, gradient filling, anti-aliases,
    color composition, and so on.

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


/*! \mainpage mGPlus V1.2.4 API Reference
 *
 * \section mainpage Introduction
 *
 */

#ifndef _MGPLUS_H
#define _MGPLUS_H

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/fixedmath.h>

#ifndef MGPLUS_MAJOR_VERSION
#   undef PACKAGE
#   undef VERSION
#   undef PACKAGE_BUGREPORT
#   undef PACKAGE_NAME
#   undef PACKAGE_STRING
#   undef PACKAGE_TARNAME
#   undef PACKAGE_VERSION
#   ifdef __MGPLUS_LIB__
#       if defined(__CMAKE_PROJECT__) || defined(WIN32)
#           include "mgplusconfig.h"
#       else
#           include "../mgplusconfig.h"
#       endif
#   else
#       include "mgplusconfig.h"
#   endif
#endif

#if defined (WIN32) && !defined(__NODLL__)
  #if defined (__MGPLUS_LIB__)
     #define MGPLUS_EXPORT       __declspec(dllexport)
  #else
     #define MGPLUS_EXPORT       __declspec(dllimport)
  #endif
#else
  #define MGPLUS_EXPORT
#endif

/**
 * \defgroup macros_types Macros and Data Types
 * @{
 */

#define MP_INV_HANDLE   0

/**
 * \var typedef DWORD HGRAPHICS
 * \brief Data type of graphic.
 *
 */
typedef DWORD HGRAPHICS;

/**
 * \var typedef DWORD HPATH
 * \brief Data type of path.
 *
 */
typedef DWORD HPATH;

/**
 * \var typedef DWORD HBRUSH
 * \brief Data type of brush.
 *
 */
typedef DWORD HBRUSH;

/**
 * \var typedef DWORD HPEN
 * \brief Data type of pen.
 *
 */
typedef DWORD HPEN;

/**
 * \var typedef DWORD HFONT
 * \brief Data type of font.
 *
 */
typedef DWORD HFONT;

/**
 * \var typedef DWORD ARGB 
 * \brief Data type of color.
 *
 */
typedef DWORD ARGB;

/**
 * MPPOINT structure.
 */
typedef struct _MPPOINT
{
    float x;
    float y;
}MPPOINT;

/**
 * \var typedef enum MPStatus
 * \brief Status.
 */
typedef enum _MPStatus
{
    MP_OK = 0,
    MP_GENERIC_ERROR = 1,
    MP_INVALID_PARAMETER = 2,
    MP_OUT_OF_MEMORY = 3,
    MP_INDEX_NOT_MATCH = 4,
    MP_NOT_ENOUGH_FIGURE_MEMORY = 5,
    MP_NOT_IMPLEMENTED = 6,
    MP_NOT_INITIALIZED = 7,
    MP_ABORTED = 8,
    MP_FILE_NOT_FOUNT = 9,
    MP_VALUE_OVERFLOW = 10,
    MP_ACCESS_DENIED = 11,
    MP_UNKNOWN_IMAGE_FORMAT = 12,
    MP_FONT_FAMILY_NOT_FOUND = 13,
    MP_FONT_STYLE_NOT_FOUND = 14,
    MP_NOT_TRUETYPE_FONT = 15,
    MP_UNSUPPORTED_VERSION = 16
}MPStatus;

/**
 * \var typedef enum MPSmoothingMode
 * \brief Smoothing mode.
 */
typedef enum _MPSmoothingMode
{
    /* Smoothing mode with speed.*/
    MP_SMOOTHING_SPEED = 0,    
    /* Smoothing mode with quality.*/
    MP_SMOOTHING_QUALITY = 1,  
}MPSmoothingMode;


/**
 * \var typedef enum MPGlyphRendering
 * \brief Glyph rendering  mode.
 */
typedef enum _MPGlyphRendering
{
    /* retrieves the glyph bitmap direct from ft2. */
    MP_GLYPH_REN_NATIVE_MONO,              
    MP_GLYPH_REN_NATIVE_GRAY8,         
    /* retrieves the curve data points in the rasterizer's native
     * format and uses the font's design units. 
     */
    MP_GLYPH_REN_OUTLINE,
    /* retrieves the glyph bitmap from agg render. */
    MP_GLYPH_REN_AGG_MONO,
    MP_GLYPH_REN_AGG_GRAY8,
}MPGlyphRendering;

/**
 * \var typedef enum PATH_VERTEX_COMMANDS
 * \brief Path vertex's command.
 */
typedef enum _PATH_VERTEX_COMMANDS
{
    PATH_CMD_STOP     = 0,
    PATH_CMD_MOVE_TO  = 1,
    PATH_CMD_LINE_TO  = 2,
    PATH_CMD_CURVE3   = 3,
    PATH_CMD_CURVE4   = 4,
    PATH_CMD_CURVEN   = 5,
    PATH_CMD_CATROM   = 6,
    PATH_CMD_UBSPLINE = 7,
    PATH_CMD_END_POLY = 0x0F,
    PATH_CMD_MASK     = 0x0F
}PATH_VERTEX_COMMANDS;

/**
 * \var typedef enum MPOrientation
 * \brief Path vertex's command.
 */
typedef enum _MPOrientation
{
    MP_ORIENTATION_CW = 0,
    MP_ORIENTATION_CCW
}MPOrientation;

/**
 * \var typedef enum MPCompositingMode
 * \brief Compositing mode.
 */
typedef enum _MPCompositingMode
{
    /* Porter-Duff Clear rule. Both the color and the alpha of the 
     * destination are cleared. Neither the source nor the 
     * destination is used as input.
     */
    MP_COMP_OP_CLEAR = 0,
    /* Porter-Duff Source rule. The source is copied to the destination. 
     * The destination is not used as input.
     */
    MP_COMP_OP_SRC,
    /* Porter-Duff Destination rule, The destination is left untouched.
     */
    MP_COMP_OP_DST,
    /* Porter-Duff Source Over Destination rule. The source is composited 
     * over the destination.
     */
    MP_COMP_OP_SRC_OVER,
    /* Porter-Duff Destination Over Source rule. The destination is composited
     * over the source and the result replaces the destination.
     */
    MP_COMP_OP_DST_OVER,
    /* Porter-Duff Source In Destination rule.The part of the source lying 
     * inside of the destination replaces the destination.
     */
    MP_COMP_OP_SRC_IN,
    /* Porter-Duff Destination In Source rule. The part of the destination lying 
     * inside of the source replaces the destination. 
     */
    MP_COMP_OP_DST_IN,
    /* Porter-Duff Source Held Out By Destination rule. The part of the source 
     * lying outside of the destination replaces the destination. 
     */
    MP_COMP_OP_SRC_OUT,
    /* Porter-Duff Destination Held Out By Source rule. The part of the 
     * destination lying outside of the source replaces the destination.
     */
    MP_COMP_OP_DST_OUT,
    /* Porter-Duff Source Atop Destination rule, The part of the source 
     * lying inside of the destination is composited onto the destination . 
     */
    MP_COMP_OP_SRC_ATOP,
    /* Porter-Duff Destination Atop Source rule The part of the destination 
     * lying inside of the source is composited over the source and replaces 
     * the destination (). 
     */
    MP_COMP_OP_DST_ATOP,
    /* Porter-Duff Source Xor Destination rule, The part of the source that lies 
     * outside of the destination is combined with the part of the destination 
     * that lies outside of the source. 
     */
    MP_COMP_OP_XOR,
}MPCompositingMode;

/**
 * \var typedef enum MPInterpolationMode
 * \brief Interpolation mode.
 */
typedef enum _MPInterpolationMode
{
    MP_INTERPOLATION_MODE_INVALID = -1,          //reserved
    MP_INTERPOLATION_MODE_DEFAULT = 0,           //reserved
    MP_INTERPOLATION_MODE_LOW_QUALITY = 1,       //reserved
    MP_INTERPOLATION_MODE_HIGH_QUALITY = 2,      //reserved
    MP_INTERPOLATION_MODE_PERSPECTIVE,
    MP_INTERPOLATION_MODE_BILINEAR,
    MP_INTERPOLATION_MODE_AFFINE,
    MP_INTERPOLATION_MODE_BICUBIC,               //reserved
    MP_INTERPOLATION_MODE_NEARESTNEIGHBOR,       //reserved
    MP_INTERPOLATION_MODE_HIGH_QUALITY_BILINEAR, //reserved
    MP_INTERPOLATION_MODE_HIGH_QUALITY_BICUBIC   //reserved
}MPInterpolationMode;

/**
 * \var typedef enum MPRenderingControl
 * \brief Rendering control mode.
 */
typedef enum _MPRenderingControl
{
    MP_RENDER_SPEED   = 0,
    MP_RENDER_QUALITY = 1,
}MPRenderingControl;

/**
 * \var typedef enum MPTextRenderingHint
 * \brief Text rendering hint.
 */
typedef enum _MPTextRenderingHint
{
    MP_TEXT_RENDER_HINT_ANTIALIAS_ON   = 0,
    MP_TEXT_RENDER_HINT_ANTIALIAS_OFF  = 1,
}MPTextRenderingHint;

/**
 * \var typedef enum MPPathRenderingHint
 * \brief Path rendering hint.
 */
typedef enum _MPPathRenderingHint
{
    MP_PATH_RENDER_HINT_ANTIALIAS_ON   = 0,
    MP_PATH_RENDER_HINT_ANTIALIAS_OFF  = 1,
}MPPathRenderingHint;

/**
 * MPMatrix structure.
 */
typedef struct _MPMatrix
{   
    /* transformation matrix.
     * [ sx , shy, 0]
     * | shx,  sy, 0]
     * [ tx,  ty,  0]
     */
    double sx, shy, shx, sy, tx, ty;
}MPMatrix;

/**
 * GLYPHMETRICS structure.
 */
typedef struct _GLYPHMETRICS {
    /* Specifies the x- and y-coordinates of the upper left 
     * corner of the smallest rectangle that completely encloses the glyph.   
     */
    unsigned char bbox_x; 
    unsigned char bbox_y; 
    /* Specifies the width of the smallest rectangle that 
     * completely encloses the glyph (its box).  
     */
    unsigned char bbox_w; 
    unsigned char bbox_h; 
    /* Specifies the horizontal/vertical distance from the origin of the current 
     * character cell to the origin of the next character cell. 
     */
    short adv_x; 
    short adv_y; 
} GLYPHMETRICS, *LPGLYPHMETRICS; 

/**
 * \var typedef enum GLYPH_DATE_TYPE
 * \brief Glyph data type.
 */
typedef enum _GLYPH_DATE_TYPE
{
    GLYPH_DATA_INVALID = 0,
    /* glyph is monochrome.*/
    GLYPH_DATA_MONO,
    /* glyph is anti-aliasing.*/
    GLYPH_DATA_GRAY8,
    /* glyph is outline.*/
    GLYPH_DATA_OUTLINE
}GLYPH_DATE_TYPE;

/**
 * GLYPHDATA structure.
 */
typedef struct _GLYPHDATA 
{
    /* glyph index of ttf font.*/
    unsigned int    glyph_index;
    /* glyph render data.*/
    BYTE*           data;
    unsigned int    data_size;
    GLYPH_DATE_TYPE data_type;
} GLYPHDATA, *LPGLYPHDATA;

/**
 * \var typedef enum MPFillMode
 * \brief Path fill mode.
 */
typedef enum _MPFillMode
{
    /* non zero fill mode.*/ 
    MP_PATH_FILL_MODE_WINDING=0,    
    /* odd fill mode.*/ 
    MP_PATH_FILL_MODE_ALTERNATE,
}MPFillMode;

/**
 * \var typedef enum LINE_CAP_E
 * \brief Linear mode.
 */
typedef enum _LINE_CAP_E
{
    /*
     * Indicates a flat line cap style. See the class overview for an
     * illustration.
     */
    CAP_BUTT = 0,
    /*
     * Indicates a rounded line cap style. See the class overview for an
     * illustration.
     */
    CAP_ROUND = 1,
    /*
     * Indicates a square line cap style. See the class overview for an
     * illustration.
     */
    CAP_SQUARE = 2,
}LINE_CAP_E;

/**
 * \var typedef enum LINE_JOIN_E
 * \brief Join mode.
 */
typedef enum _LINE_JOIN_E
{
    /*
     * Indicates a mitered line join style.
     */
    JOIN_MITER = 0,
    /*
     * Indicates a rounded line join style. 
     */
    JOIN_ROUND = 1,
    /*
     * Indicates a bevelled line join style.
     */
    JOIN_BEVEL = 2,
    /* reseverd,not used now, but surport by mgplus.*/
    JOIN_MILTER_REVERT = 3, 
    JOIN_MILTER_ROUND = 4
}LINE_JOIN_E;

/**
 * \var typedef enum MPBrushType
 * \brief Brush type.
 */
typedef enum _MPBrushType
{
    /* Brush the path with the single color.*/
    MP_BRUSH_TYPE_SOLIDCOLOR = 0,   
    /* Brush the path with the hatchfill.*/
    MP_BRUSH_TYPE_HATCHFILL = 1,    
    /* Brush the path with the texturefill.*/
    MP_BRUSH_TYPE_TEXTUREFILL = 2,  
    /* Brush the path with the path gradient.*/
    MP_BRUSH_TYPE_PATHGRADIENT = 3, 
    /* Brush the path with linear gradient.*/
    MP_BRUSH_TYPE_LINEARGRADIENT    
}MPBrushType;

/**
 * \var typedef enum MPLinearGradientMode
 * \brief Linear gradient mode.
 */
typedef enum _MPLinearGradientMode
{
    /* gradient horizontal.*/
    MP_LINEAR_GRADIENT_MODE_HORIZONTAL = 0,  
    /* gradient vertica.*/
    MP_LINEAR_GRADIENT_MODE_VERTICAL,        
    /* gradient forwarddiagonal.*/
    MP_LINEAR_GRADIENT_MODE_FORWARDDIAGONAL, 
    /* gradient backwarddiagonal.*/
    MP_LINEAR_GRADIENT_MODE_BACKWARDDIAGONAL 
}MPLinearGradientMode;

    /** @} end of macros_types */

    /**
    * \defgroup Graphics Graphics 
    *
    * MGPlus maintains some graphics defines and operations
    *
    * @{
    */

/**
 * \fn HGRAPHICS MGPlusGraphicCreateFromDC (HDC hdc)
 * \brief Create Graphic’s buffer through hdc.
 *
 * This function creates MGPlusGraphics from specical  hdc and 
 * returns the create status.
 * 
 * \param hdc         The handle of hdc.
 *
 * \return HGRAPHICS when succeed to create, or NULL indicates an error.
 *
 * \sa MGPlusDeleteGraphics
 */
MGPLUS_EXPORT HGRAPHICS MGPlusGraphicCreateFromDC (HDC hdc);

/**
 * \fn HGRAPHICS MGPlusGraphicCreateWithoutCanvas (HDC hdc)
 * \brief Create graphic from hdc and use hdc's surface as canvas.
 *
 * This function creates MGPlusGraphics from specical hdc, but use hdc's 
 * surface as canvas of thr Graphics.
 * 
 * \param hdc         The handle of hdc.
 *
 * \return HGRAPHICS when succeed to create, or NULL indicates an error.
 *
 * \sa MGPlusDeleteGraphics
 */
MGPLUS_EXPORT HGRAPHICS MGPlusGraphicCreateWithoutCanvas(HDC hdc);

/**
 * \fn MGPLUS_EXPORT HDC MGPlusGetGraphicDC (HGRAPHICS graphic)
 * \brief Get graphic’s buffer hdc.
 *
 * This function get graphic’s buffer hdc.
 * 
 * \param graphic    The MGPlusGraphics pointer.
 *
 * \return HDC if success, else return MP_GENERIC_ERROR.
 *
 * \sa MGPlusDeleteGraphics
 */
MGPLUS_EXPORT HDC MGPlusGetGraphicDC (HGRAPHICS graphic);

/**
 * \fn HGRAPHICS MGPlusGraphicCreate (int width, int height)
 * \brief Create graphic.
 *
 * This function creates MGPlusGraphics from specical size.
 * 
 * \param width       The width of the  graphics's surface
 * \param height      The height of the  graphics's surface
 *
 * \return HGRAPHICS when succeed to create, or else NULL indicates an error.
 *
 * \sa MGPlusDeleteGraphics
 */
MGPLUS_EXPORT HGRAPHICS MGPlusGraphicCreate (int width, int height);

/**
 * \fn MGPLUS_EXPORT int MGPlusSaveHG (HGRAPHICS hg)
 * \brief Save the graphic to stack.
 *
 * This function saves the graphic to stack,
 * supported by mGPlus V1.2.1 or upper.
 * 
 * \param hg          The Graphic handle.
 *
 * \return If succee, return states, else return zero.
 *
 * \sa MGPlusResotreHG  
 */
MGPLUS_EXPORT int MGPlusSaveHG (HGRAPHICS hg);

/**
 * \fn HGRAPHICS MGPlusRestoreHG (HGRAPHICS hg, int saved_hg)
 * \brief Restore the graphic. 
 *
 * This function restores graphic from saved stack,
 * supported by mGPlus V1.2.1 or upper.
 * 
 * \param hg          The Graphic handle.
 * \param saved_hg    The save_hg Specifies the saved state to be restored. If this 
 *        parameter is positive, saved_hg represents a specific instance 
 *        of the state to be restored. If this parameter is negative, 
 *        saved_dc represents an instance relative to the current state. 
 *        For example, -1 restores the most recently saved state.
 *
 * \return If the function succeeds, the return value is nonzero. If the 
 *         function fails, the return value is zero.
 *
 * \sa MGPlusSaveHG 
 */
MGPLUS_EXPORT BOOL MGPlusRestoreHG (HGRAPHICS hg, int saved_hg);

/**
 * \fn MPStatus MGPlusGraphicCopyFromDC (HGRAPHICS graphic, HDC hdc, int sx,
 *      int sy, int sw, int sh, int dx, int dy)
 * \brief Copy the hdc content to graphic. 
 *
 * This function copys the hdc content to graphic.
 * 
 * \param graphic   The MGPlusGraphics pointer.
 * \param hdc       The copy hdc.
 * \param sx        The x coordinate of the upper-left corner of the rectangle 
 * in the source DC.
 * \param sy        The y coordinate of the upper-left corner of the rectangle 
 * in the source DC.
 * \param sw        The width of the source rectangle.
 * \param sh        The height of the source rectangle.
 * \param dx        The x coordinate of the upper-left corner of the rectangle 
 * in the destination DC.
 * \param dy        The y coordinate of the upper-left corner of the rectangle 
 * in the destination DC.
 *
 * \sa MGPlusDeleteGraphics
 */
MGPLUS_EXPORT MPStatus MGPlusGraphicCopyFromDC (HGRAPHICS graphic, HDC hdc, int sx,
        int sy, int sw, int sh, int dx, int dy);

/**
 * \fn MPStatus MGPlusGraphicCopy (HGRAPHICS src_gs, HGRAPHICS dst_gs)
 * \brief Copy the src graphic to dst.
 *
 * This function copies the src graphic to dst.
 *
 * \param src_gs      The source MGPlusGraphics pointer.
 * \param dst_gs      The dest MGPlusGraphics pointer.
 *
 * \return MP_GENERIC_ERROR indicates an error.
 *
 * \sa MGPlusDeleteGraphics
 */
MGPLUS_EXPORT MPStatus MGPlusGraphicCopy (HGRAPHICS src_gs, HGRAPHICS dst_gs);

/**
 * \fn MPStatus MGPlusGraphicBlend (HGRAPHICS src_gs, HGRAPHICS dst_gs)
 * \brief Blend between Graphic’s buffer.
 *
 * This function Blends source MGPlusGraphics from dest MGPlusGraphics.
 *
 * \param src_gs      The source MGPlusGraphics pointer.
 * \param dst_gs      The dest MGPlusGraphics pointer.
 *
 * \return MP_GENERIC_ERROR indicates an error.
 *
 * \sa MGPlusDeleteGraphics
 */
MGPLUS_EXPORT MPStatus MGPlusGraphicBlend (HGRAPHICS src_gs, HGRAPHICS dst_gs);

/**
 * \fn MPStatus MGPlusGraphicClearEx (HGRAPHICS graphics, RECT* rect, ARGB color)
 * \brief Use color to clear graphics with the specific rect. 
 *
 * This function uses color to clear graphics with the specific rect,
 * supported by mGPlus V1.2.1 or upper.
 * 
 * \param graphics   The MGPlusGraphics pointer
 * \param rect       The specific rect 
 * \param color      The clear color
 * \return Clear Status, MP_GENERIC_ERROR indicates an error.
 *
 * \sa MGPlusDeleteGraphics
 */
MGPLUS_EXPORT MPStatus MGPlusGraphicClearEx (HGRAPHICS graphics, RECT* rect, ARGB color);

#define MGPlusGraphicClear(graphics, color) MGPlusGraphicClearEx (graphics, NULL, color)

/**
 * \fn MPStatus MGPlusGraphicSave (HGRAPHICS graphics, HDC hdc, int sx, 
 * int sy, int sw, int sh, int dx, int dy)
 * \brief Create Graphic’s buffer through hdc.
 *
 * This function creates MGPlusGraphics from specical  pix format and 
 * returns the create status.
 * 
 * \param graphics  The MGPlusGraphics pointer.
 * \param hdc       The dst hdc.
 * \param sx        The x coordinate of the upper-left corner of the rectangle 
 *                  in the source DC.
 * \param sy        The y coordinate of the upper-left corner of the rectangle 
 *                  in the source DC.
 * \param sw        The width of the source rectangle.
 * \param sh        The height of the source rectangle.
 * \param dx        The x coordinate of the upper-left corner of the rectangle 
 *                  in the destination DC.
 * \param dy        The y coordinate of the upper-left corner of the rectangle 
 *                  in the destination DC.
 *
 * \return Create Status, MP_GENERIC_ERROR indicates an error.
 *
 * \sa MGPlusDeleteGraphics
 */
MGPLUS_EXPORT MPStatus MGPlusGraphicSave (HGRAPHICS graphics, HDC hdc, 
        int sx, int sy, int sw, int sh, int dx, int dy);

/**
 * \fn MPStatus MGPlusGraphicDelete (HGRAPHICS graphics)
 * \brief Delete the graphic.
 *
 * This function deletes MGPlusGraphics  
 * 
 * \param graphics    The MGPlusGraphics pointer.
 *
 * \return Delete Status, MP_GENERIC_ERROR indicates an error.
 *
 * \sa MGPlusGraphicsCreate
 */
MGPLUS_EXPORT MPStatus MGPlusGraphicDelete (HGRAPHICS graphics);

    /** @} end of graphics */


    /**
    * \defgroup Compositing Compositing 
    *
    * MGPlus maintains some Compositing defines and operations
    *
    * @{
    */
/**
 * \fn MPStatus MGPlusSetCompositingMode (HGRAPHICS graphics, 
 *           MPCompositingMode compositingMode)
 * \brief Set the compositing mode of graphic.
 *
 * This function sets the compositing mode of graphic  
 * 
 * \param graphics         The MGPlusGraphics pointer.
 * \param compositingMode  The compositing mode.
 *
 * \return Set Status, MP_GENERIC_ERROR indicates an error.
 *
 * \sa MGPlusSetCompositingMode
 */
MGPLUS_EXPORT MPStatus MGPlusSetCompositingMode (HGRAPHICS graphics, 
         MPCompositingMode composite_mode);

/**
 * \fn MPStatus MGPlusGetCompositingMode (HGRAPHICS graphics, 
 *           MPCompositingMode* compositingMode)
 * \brief Get the compositing mode of graphic.
 *
 * This function gets the compositing mode of graphic  
 * 
 * \param graphics            The MGPlusGraphics pointer.
 * \param compositingMode     The compositing mode pointer.
 *
 * \return Get Status, MP_GENERIC_ERROR indicates an error.
 *
 * \sa MGPlusGetCompositingMode
 */
MGPLUS_EXPORT MPStatus MGPlusGetCompositingMode (HGRAPHICS graphics, 
        MPCompositingMode* composite_mode);

    /** @} end of Compositing */

    /**
    * \defgroup Render Render Mode 
    *
    * MGPlus maintains some rendermode defines and operations
    *
    * @{
    */

/**
 * \fn MPStatus MGPlusGetSmoothingMode (HGRAPHICS graphics, MPSmoothingMode* value)
 *
 * \brief Set the smoothing mode of graphic.
 *
 * This function gets the smoothing mode.  
 * 
 * \param graphics     The MGPlusGraphics pointer.
 * \param value        The smoothing mode.
 *
 * \return Set Status, MP_GENERIC_ERROR indicates an error.
 *
 * \sa MGPlusSetSmoothingMode
 */
MGPLUS_EXPORT MPStatus MGPlusGetSmoothingMode (HGRAPHICS graphics, MPSmoothingMode* value);

/**
 * \fn MPStatus MGPlusSetSmoothingMode (HGRAPHICS graphics, 
 *                       MPSmoothingMode smoothingMode)
 * \brief Set the smoothing mode of graphic.
 *
 * This function sets the smoothing mode of graphic  
 * 
 * \param graphics      The MGPlusGraphics pointer.
 * \param smoothingMode The smoothing mode .
 *
 * \return Set Status, MP_GENERIC_ERROR indicates an error.
 *
 * \sa MGPlusGetSmoothingMode
 */
MGPLUS_EXPORT MPStatus MGPlusSetSmoothingMode (HGRAPHICS graphics, MPSmoothingMode value);

/**
 * \fn MPStatus MGPlusSetPathRenderingHint (HGRAPHICS graphics, 
 *                MPPathRenderingHint value)
 * \brief Set the path anti-alias value of graphic.
 *
 * This function gets the compositing mode of graphic  
 * 
 * \param graphics      The MGPlusGraphics pointer.
 * \param value         The anti-alias value.
 *
 * \return Set Status, MP_GENERIC_ERROR indicates an error.
 *
 * \sa MGPlusGetPathRenderingHint 
 */
MGPLUS_EXPORT MPStatus MGPlusSetPathRenderingHint (HGRAPHICS graphics, 
        MPPathRenderingHint value);

/**
 * \fn  MPStatus MGPlusGetPathRenderingHint (HGRAPHICS graphics, 
        MPPathRenderingHint* value);
 * \brief Get the path anti-alias value of graphic.
 *
 * This function gets the path anti-alias value of graphic  
 * 
 * \param graphics      The MGPlusGraphics pointer.
 * \param value         The path anti-alias value pointer.
 *
 * \return Get Status, MP_GENERIC_ERROR indicates an error.
 *
 * \sa MGPlusSetPathRenderingHint 
 */
MGPLUS_EXPORT MPStatus MGPlusGetPathRenderingHint (HGRAPHICS graphics, 
        MPPathRenderingHint* value);

/**
 * \fn MPStatus MGPlusSetTextRenderingHint (HGRAPHICS graphics, 
 *                MPTextRenderingHint value)
 * \brief Set the text rendering mode of graphic.
 *
 * This function sets the text rendering mode of graphic  
 * 
 * \param graphics     The MGPlusGraphics pointer.
 * \param value        The text rendering mode.
 *
 * \return Get Status, MP_GENERIC_ERROR indicates an error.
 *
 * \sa MGPlusGetTextRenderingHint
 */
MGPLUS_EXPORT MPStatus MGPlusSetTextRenderingHint (HGRAPHICS graphics, 
        MPTextRenderingHint value);

/**
 * \fn MPStatus MGPlusGetTextRenderingHint (HGRAPHICS graphics, 
 *                MPTextRenderingHint* value)
 *
 * \brief Get the text rendering mode of graphic.
 *
 * This function gets the text rendering mode of graphic  
 * 
 * \param graphics     The MGPlusGraphics pointer.
 * \param value        The text rendering mode pointer.
 *
 * \return Get Status, MP_GENERIC_ERROR indicates an error.
 *
 * \sa MGPlusSetTextRenderingHint
 */
MGPLUS_EXPORT MPStatus MGPlusGetTextRenderingHint (HGRAPHICS graphics, 
        MPTextRenderingHint* value);

/**
 * \fn MPStatus MGPlusSetInterpolationMode (HGRAPHICS graphics, 
 *                MPInterpolationMode value)
 * \brief Set the interpolation mode of graphic.
 *
 * This function sets the interpolation mode of graphic  
 * 
 * \param graphics    The MGPlusGraphics pointer.
 * \param value       The interpolation mode.
 *
 * \return Get Status, MP_GENERIC_ERROR indicates an error.
 *
 * \sa MGPlusGetInterpolationMode
 */
MGPLUS_EXPORT MPStatus MGPlusSetInterpolationMode (HGRAPHICS graphics, 
        MPInterpolationMode mode);

/**
 * \fn MPStatus MGPlusGetInterpolationMode (HGRAPHICS graphics, 
 *        MPInterpolationMode* value)
 * \brief Get the interpolation mode of graphic.
 *
 * This function gets the interpolation mode of graphic  
 * 
 * \param graphics    The MGPlusGraphics pointer.
 * \param value       The interpolation mode pointer.
 *
 * \return Get Status, MP_GENERIC_ERROR indicates an error.
 *
 * \sa MGPlusSetInterpolationMode
 */
MGPLUS_EXPORT MPStatus MGPlusGetInterpolationMode (HGRAPHICS graphics, 
        MPInterpolationMode* mode);

    /** @} end of rendermode */

    /**
    * \defgroup Worldtransform Worldtransform 
    *
    * MGPlus maintains some worldtransform defines and operations
    *
    * @{
    */

/**
 * \fn MPStatus MGPlusSetWorldTransform (HGRAPHICS graphics, 
 * MPMatrix *matrix)
 * \brief Set the world transform of graphic.
 *
 * This function sets the world transform of graphic  
 * 
 * \param graphics    The MGPlusGraphics pointer.
 * \param matrix      The MPMatrix pointer.
 *
 * \return Get Status, MP_GENERIC_ERROR indicates an error.
 *
 * \sa MGPlusSetWorldTransform
 */
MGPLUS_EXPORT MPStatus MGPlusSetWorldTransform (HGRAPHICS graphics, 
        MPMatrix *matrix);

/**
 * \fn MPStatus MGPlusWorldTransform (HGRAPHICS graphics, 
 * MPMatrix *matrix)
 * \brief Multi the special matrix with matrix of graphic.
 *
 * This function sets the world transform of graphic,
 * supported by mGPlus V1.2.1 or upper.
 * 
 * \param graphics    The MGPlusGraphics pointer.
 * \param matrix      The MPMatrix pointer.
 *
 * \return Get Status, MP_GENERIC_ERROR indicates an error.
 *
 * \sa MGPlusSetWorldTransform
 */
MGPLUS_EXPORT MPStatus MGPlusWorldTransform (HGRAPHICS graphics, MPMatrix *matrix);

/**
 * \fn MPStatus MGPlusGetWorldTransform (HGRAPHICS graphics, 
 * MPMatrix *matrix)
 * \brief Get the world transform of graphic.
 *
 * This function sets the world transform of 
 * graphic, supported by mGPlus V1.2.1 or upper.  
 * 
 * \param graphics    The MGPlusGraphics pointer.
 * \param matrix      The MPMatrix pointer.
 *
 * \return Get Status, MP_GENERIC_ERROR indicates an error.
 *
 * \sa MGPlusSetWorldTransform
 */
MGPLUS_EXPORT MPStatus MGPlusGetWorldTransform (HGRAPHICS graphics, MPMatrix *matrix);

/**
 * \fn MPStatus MGPlusResetWorldTransform (HGRAPHICS graphics)
 * \brief Reset the world transform of graphic.
 *
 * This function resets the world transform of graphic  
 * 
 * \param graphics    The MGPlusGraphics pointer.
 *
 * \return Get Status, MP_GENERIC_ERROR indicates an error.
 *
 * \sa MGPlusSetWorldTransform
 */
MGPLUS_EXPORT MPStatus MGPlusResetWorldTransform (HGRAPHICS graphics);

/**
 * \fn MPStatus MGPlusTranslateWorldTransform (HGRAPHICS graphics, 
 *    float dx, float dy)
 * \brief Translate the world transform of graphic.
 *
 * This function translates the world transform of graphic  
 * 
 * \param graphics    The MGPlusGraphics pointer.
 * \param dx          The translate x distance.
 * \param dy          The translate y distance.
 *
 * \return Get Status, MP_GENERIC_ERROR indicates an error.
 *
 * \sa MGPlusSetWorldTransform
 */
MGPLUS_EXPORT MPStatus MGPlusTranslateWorldTransform (HGRAPHICS graphics,
        float dx, float dy);

/**
 * \fn MPStatus MGPlusScaleWorldTransform (HGRAPHICS graphics, 
 *     float sx, float sy)
 * \brief Scale the world transform of graphic.
 *
 * This function scales the world transform of graphic  
 * 
 * \param graphics    The MGPlusGraphics pointer.
 * \param sx          the x scaling factor.
 * \param sy          the y scaling factor.
 *
 * \return Get Status, MP_GENERIC_ERROR indicates an error.
 *
 * \sa MGPlusSetWorldTransform
 */
MGPLUS_EXPORT MPStatus MGPlusScaleWorldTransform (HGRAPHICS graphics, 
        float sx, float sy);

/**
 * \fn MPStatus MGPlusRotateWorldTransform (HGRAPHICS graphics, 
        float angle)
 * \brief Rotate the world transform of graphic.
 *
 * This function rotates the world transform of graphic. 
 * 
 * \param graphics    The MGPlusGraphics pointer.
 * \param angle       The rotation angle.
 *
 * \return Get Status, MP_GENERIC_ERROR indicates an error.
 *
 * \sa MGPlusSetWorldTransform
 */
MGPLUS_EXPORT MPStatus MGPlusRotateWorldTransform (HGRAPHICS graphics, 
        float angle);

/**
 * \fn MPStatus MGPlusPathTransform (HPATH path)
 * \brief  Transform the path using its matrix.
 *
 * This function transforms the path using its matrix. 
 *
 * \param path    The matrix path. 
 *
 * \return if success return TRUE, else return FALSE.
 */ 
MGPLUS_EXPORT MPStatus MGPlusPathTransform (HPATH path);

    /** @} end of worldtransform */

    /**
    * \defgroup Rrender Render 
    *
    * MGPlus maintains some render defines and operations
    *
    * @{
    */

/**
 * \fn MPStatus MGPlusDrawLine (HGRAPHICS graphics, HPEN pen, 
                              float x1, float y1, float x2, float y2)
 * \brief Draw a line on the graphic.
 *
 * This function draws a line on the graphic. 
 * 
 * \param graphics  The MGPlusGraphics pointer.
 * \param pen       The draw pen.
 * \param x1        The start x of line.
 * \param y1        The start y of line.
 * \param x2        The end x of line.
 * \param y2        The end y of line.
 *
 * \return Get Status, MP_GENERIC_ERROR indicates an error.
 *
 * \sa MGPlusDrawLineI
 */
MGPLUS_EXPORT MPStatus MGPlusDrawLine (HGRAPHICS graphics, HPEN pen, 
        float x1, float y1, float x2, float y2);

/**
 * \fn MPStatus MGPlusDrawLineI (HGRAPHICS graphics, HPEN pen, 
                              int x1, int y1, int x2, int y2)
 * \brief Draw a line on the graphic.
 *
 * This function draws a line on the graphic. 
 * 
 * \param graphics  The MGPlusGraphics pointer.
 * \param pen       The draw pen.
 * \param x1        The start x of line
 * \param y1        The start y of line.
 * \param x2        The end x of line.
 * \param y2        The end y of line.
 *
 * \return Get Status, MP_GENERIC_ERROR indicates an error.
 *
 * \sa MGPlusDrawLine
 */
MGPLUS_EXPORT MPStatus MGPlusDrawLineI (HGRAPHICS graphics, HPEN pen, 
        int x1, int y1, int x2, int y2);

/**
 * \fn MPStatus MGPlusDrawArc (HGRAPHICS graphics, HPEN pen, 
 * float x, float y, float width, float height, float startangle, 
 * float sweepangle)
 * \brief Draw a arc on the graphic.
 *
 * This function draws a arc on the graphic. 
 * 
 * \param graphics       The MGPlusGraphics pointer.
 * \param pen            The draw pen.
 * \param x              The x-coordinate of the upper-left corner of the bounding rectangle 
 *                       for the ellipse that contains the arc.
 * \param y              The y-coordinate of the upper-left corner of the bounding rectangle 
 *                       for the ellipse that contains the arc.
 * \param width          The width of the bounding rectangle for the ellipse.
 * \param height         The height of the bounding rectangle for the ellipse.
 * \param startangle     The start angle.
 * \param sweepangle     specifies the angle, between the starting point (startangle) 
 *                       and the ending point of the arc.
 *
 * \return Get Status, MP_GENERIC_ERROR indicates an error.
 *
 * \sa MGPlusDrawArcI
 */
MGPLUS_EXPORT MPStatus MGPlusDrawArc (HGRAPHICS graphics, HPEN pen,
        float x, float y, float width, float height, float startangle, 
        float sweepangle);

/**
 * \fn MPStatus MGPlusDrawArcI (HGRAPHICS graphics, HPEN pen, int x, int y,
                    int width, int height, float startangle, float sweepangle)
 * \brief Draw a arc on the graphic.
 *
 * This function draws a arc on the graphic. 
 * 
 * \param graphics       The MGPlusGraphics pointer.
 * \param pen            The draw pen.
 * \param x              The x-coordinate of the upper-left corner of the bounding rectangle 
 *                       for the ellipse that contains the arc.
 * \param y              The y-coordinate of the upper-left corner of the bounding rectangle 
 *                       for the ellipse that contains the arc.
 * \param width          The width of the bounding rectangle for the ellipse.
 * \param height         The height of the bounding rectangle for the ellipse.
 * \param startangle     The start angle.
 * \param sweepangle     Specifies the angle, between the starting point (startangle) 
 *                       and the ending point of the arc.
 * \return Get Status, MP_GENERIC_ERROR indicates an error.
 *
 * \sa MGPlusDrawArc
 */
MGPLUS_EXPORT MPStatus MGPlusDrawArcI (HGRAPHICS graphics, HPEN pen,
        int x, int y, int width, int height, float startAngle, 
        float sweepAngle);

/**
 * \fn MPStatus MGPlusFillArc (HGRAPHICS graphics, HBRUSH brush, 
 * float x, float y,float width, float height, float startangle, 
 * float sweepangle)
 * \brief Fill a arc on the graphic.
 *
 * This function fills a arc on the graphic. 
 * 
 * \param graphics       The MGPlusGraphics pointer.
 * \param brush          The fill brush.
 * \param x              The x-coordinate of the upper-left corner of the bounding rectangle 
 *                       for the ellipse that contains the arc.
 * \param y              The y-coordinate of the upper-left corner of the bounding rectangle 
 *                       for the ellipse that contains the arc.
 * \param width          The width of the bounding rectangle for the ellipse.
 * \param height         The height of the bounding rectangle for the ellipse.
 * \param startangle     The start angle.
 * \param sweepangle     Specifies the angle, between the starting point (startangle) 
 *                       and the ending point of the arc.
 *
 * \return Get Status, MP_GENERIC_ERROR indicates an error.
 *
 * \sa MGPlusFillArcI
 */
MGPLUS_EXPORT MPStatus MGPlusFillArc (HGRAPHICS graphics, HBRUSH brush,
        float x, float y, float width, float height, float startangle, 
        float sweepangle);

/**
 * \fn MPStatus MGPlusFillArcI (HGRAPHICS graphics, HBRUSH brush, int x, int y,
                    int width, int height, float startangle, float sweepangle)
 * \brief Fill a arc on the graphic.
 *
 * This function fills a arc on the graphic. 
 * 
 * \param graphics       The MGPlusGraphics pointer.
 * \param brush          The fill brush.
 * \param x              The x-coordinate of the upper-left corner of the bounding rectangle 
 *                       for the ellipse that contains the arc.
 * \param y              The y-coordinate of the upper-left corner of the bounding rectangle 
 *                       for the ellipse that contains the arc.
 * \param width          The width of the bounding rectangle for the ellipse.
 * \param height         The height of the bounding rectangle for the ellipse.
 * \param startangle     The start angle.
 * \param sweepangle     Specifies the angle, between the starting point (startangle) 
 *                       and the ending point of the arc.
 * \return Get Status, MP_GENERIC_ERROR indicates an error.
 *
 * \sa MGPlusFillArc
 */
MGPLUS_EXPORT MPStatus MGPlusFillArcI (HGRAPHICS graphics, HBRUSH brush,
        int x, int y, int width, int height, float startAngle,
        float sweepAngle);

/**
 * \fn MPStatus MGPlusDrawBezier (HGRAPHICS graphics, HPEN pen, float x1, float y1,
                                float x2, float y2, float x3, float y3, float x4, float y4)
 * \brief Draw a bezier on the graphic.
 *
 * This function draws a bezier on the graphic. 
 * 
 * \param graphics  The MGPlusGraphics pointer.
 *
 * \param pen       The draw pen.
 *
 * \param x1        The start x of bezier.
 * \param y1        The start y of bezier.
 * \param x2        The first control x of bezier.
 * \param y2        The first control y of bezier.
 * \param x3        The second control x of bezier.
 * \param y3        The second control y of bezier.
 * \param x4        The end x of bezier.
 * \param y4        The end y of bezier.
 *
 * \return Get Status, MP_GENERIC_ERROR indicates an error.
 *
 * \sa MGPlusDrawBezierI
 */
MGPLUS_EXPORT MPStatus MGPlusDrawBezier (HGRAPHICS graphics, HPEN pen,
        float x1, float y1, float x2, float y2, float x3, float y3,
        float x4, float y4);

/**
 * \fn MPStatus MGPlusDrawBezierI (HGRAPHICS graphics, HPEN pen, 
 * int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4)
 * \brief Draw a bezier on the graphic.
 *
 * This function draws a bezier on the graphic. 
 * 
 * \param graphics  The MGPlusGraphics pointer.
 * \param pen       The draw pen.
 * \param x1        The start x of bezier.
 * \param y1        The start y of bezier.
 * \param x2        The first control x of bezier.
 * \param y2        The first control y of bezier.
 * \param x3        The second control x of bezier.
 * \param y3        The second control y of bezier.
 * \param x4        The end x of bezier.
 * \param y4        The end y of bezier.
 *
 * \return Get Status, MP_GENERIC_ERROR indicates an error.
 *
 * \sa MGPlusDrawBezier
 */
MGPLUS_EXPORT MPStatus MGPlusDrawBezierI (HGRAPHICS graphics, HPEN pen, 
        int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4);

/**
 * \fn MPStatus MGPlusFillBezier (HGRAPHICS graphics, HBRUSH brush, float x1, float y1,
                                float x2, float y2, float x3, float y3, float x4, float y4)
 * \brief Fill a bezier on the graphic.
 *
 * This function fills a bezier on the graphic. 
 * 
 * \param graphics  The MGPlusGraphics pointer.
 *
 * \param brush     The fill brush.
 *
 * \param x1        The start x of bezier.
 * \param y1        The start y of bezier.
 * \param x2        The first control x of bezier.
 * \param y2        The first control y of bezier.
 * \param x3        The second control x of bezier.
 * \param y3        The second control y of bezier.
 * \param x4        The end x of bezier.
 * \param y4        The end y of bezier.
 *
 * \return Get Status, MP_GENERIC_ERROR indicates an error.
 *
 * \sa MGPlusFillBezierI
 */
MGPLUS_EXPORT MPStatus MGPlusFillBezier (HGRAPHICS graphics, HBRUSH brush,
        float x1, float y1, float x2, float y2, float x3, float y3,
        float x4, float y4);

/**
 * \fn MPStatus MGPlusFillBezierI (HGRAPHICS graphics, HBRUSH brush, 
 * int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4)
 * \brief Fill a bezier on the graphic.
 *
 * This function fills a bezier on the graphic. 
 * 
 * \param graphics  The MGPlusGraphics pointer.
 * \param brush     The fill brush.
 * \param x1        The start x of bezier.
 * \param y1        The start y of bezier.
 * \param x2        The first control x of bezier.
 * \param y2        The first control y of bezier.
 * \param x3        The second control x of bezier.
 * \param y3        The second control y of bezier.
 * \param x4        The end x of bezier.
 * \param y4        The end y of bezier.
 *
 * \return Get Status, MP_GENERIC_ERROR indicates an error.
 *
 * \sa MGPlusFillBezier
 */
MGPLUS_EXPORT MPStatus MGPlusFillBezierI (HGRAPHICS graphics, HBRUSH brush, 
        int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4);

/**
 * \fn MPStatus MGPlusFillRectangle (HGRAPHICS graphics, HBRUSH brush, 
 * float x, float y, float width, float height)
 * \brief Draw a rectangle on the graphic.
 *
 * This function draws a rectangle on the graphic. 
 * 
 * \param graphics  The MGPlusGraphics pointer.
 * \param brush     The fill brush.
 * \param x         The left x of the rectangle.
 * \param y         The top y of the rectangle.
 * \param width     The width of the rectangle.
 * \param height    The height of the rectangle.
 *
 * \return Get Status, MP_GENERIC_ERROR indicates an error.
 *
 * \sa MGPlusFillRectangleI
 */
MGPLUS_EXPORT MPStatus MGPlusFillRectangle (HGRAPHICS graphics, HBRUSH brush, 
        float x, float y, float width, float height);

/**
 * \fn MPStatus MGPlusFillRectangleI (HGRAPHICS graphics, HPEN pen, 
 * int x, int y, int width, int height)
 * \brief Draw a rectangle on the graphic.
 *
 * This function draws a rectangle on the graphic. 
 * 
 * \param graphics  The MGPlusGraphics pointer.
 * \param brush     The fill brush.
 * \param x         The left x of the rectangle.
 * \param y         The top y of the rectangle.
 * \param width     The width of the rectangle.
 * \param height    The height of the rectangle.
 *
 * \return Get Status, MP_GENERIC_ERROR indicates an error.
 *
 * \sa MGPlusFillRectangle
 */
MGPLUS_EXPORT MPStatus MGPlusFillRectangleI (HGRAPHICS graphics, HBRUSH brush, 
        int x, int y, int width, int height);

/**
 * \fn MPStatus MGPlusDrawRectangle (HGRAPHICS graphics, HPEN pen, 
 * float x, float y, float width, float height)
 * \brief Draw a rectangle on the graphic.
 *
 * This function draws a rectangle on the graphic. 
 * 
 * \param graphics  The MGPlusGraphics pointer.
 * \param pen       The draw pen.
 * \param x         The left x of the rectangle.
 * \param y         The top y of the rectangle.
 * \param width     The width of the rectangle.
 * \param height    The height of the rectangle.
 *
 * \return Get Status, MP_GENERIC_ERROR indicates an error.
 *
 * \sa MGPlusDrawRectangleI
 */
MGPLUS_EXPORT MPStatus MGPlusDrawRectangle (HGRAPHICS graphics, HPEN pen, 
        float x, float y, float width, float height);

/**
 * \fn MPStatus MGPlusDrawRectangleI (HGRAPHICS graphics, HPEN pen, 
 * int x, int y, int width, int height)
 * \brief Draw a rectangle on the graphic.
 *
 * This function draws a rectangle on the graphic. 
 * 
 * \param graphics  The MGPlusGraphics pointer.
 * \param pen       The draw pen.
 * \param x         The left x of the rectangle.
 * \param y         The top y of the rectangle.
 * \param width     The width of the rectangle.
 * \param height    The height of the rectangle.
 *
 * \return Get Status, MP_GENERIC_ERROR indicates an error.
 *
 * \sa MGPlusDrawRectangle
 */
MGPLUS_EXPORT MPStatus MGPlusDrawRectangleI (HGRAPHICS graphics, HPEN pen, 
        int x, int y, int width, int height);

/**
 * \fn MPStatus MGPlusFillRectangle (HGRAPHICS graphics, HBRUSH brush, 
 * float x, float y, float width, float height)
 * \brief Draw a rectangle on the graphic.
 *
 * This function draws a rectangle on the graphic. 
 * 
 * \param graphics  The MGPlusGraphics pointer.
 * \param brush     The fill brush.
 * \param x         The left x of the rectangle.
 * \param y         The top y of the rectangle.
 * \param width     The width of the rectangle.
 * \param height    The height of the rectangle.
 *
 * \return Get Status, MP_GENERIC_ERROR indicates an error.
 *
 * \sa MGPlusFillRectangleI
 */
MGPLUS_EXPORT MPStatus MGPlusFillRectangle (HGRAPHICS graphics, HBRUSH brush, 
        float x, float y, float width, float height);

/**
 * \fn MPStatus MGPlusFillRectangleI (HGRAPHICS graphics, HPEN pen, 
 * int x, int y, int width, int height)
 * \brief Draw a rectangle on the graphic.
 *
 * This function draws a rectangle on the graphic. 
 * 
 * \param graphics  The MGPlusGraphics pointer.
 * \param brush     The fill brush.
 * \param x         The left x of the rectangle.
 * \param y         The top y of the rectangle.
 * \param width     The width of the rectangle.
 * \param height    The height of the rectangle.
 *
 * \return Get Status, MP_GENERIC_ERROR indicates an error.
 *
 * \sa MGPlusFillRectangle
 */
MGPLUS_EXPORT MPStatus MGPlusFillRectangleI (HGRAPHICS graphics, HBRUSH brush, 
        int x, int y, int width, int height);

/**
 * \fn MPStatus MGPlusDrawRoundRectEx (HGRAPHICS graphics, HPEN pen, 
 * float x, float y, float width, float height, float rx, float ry)
 * \brief Draw a roundrect on the graphic.
 *
 * This function draws a roundrect on the graphic. 
 * 
 * \param graphics  The MGPlusGraphics pointer.
 * \param pen       The draw pen.
 * \param x         The left x of the rectangle.
 * \param y         The top y of the rectangle.
 * \param width     The width of the rectangle.
 * \param height    The height of the rectangle.
 * \param rx        The x-radius of round rect arc.
 * \param ry        The y-radius of round rect arc.
 *
 * \return Get Status, MP_GENERIC_ERROR indicates an error.
 *
 * \sa MGPlusDrawRoundRectI
 */
MGPLUS_EXPORT MPStatus MGPlusDrawRoundRectEx (HGRAPHICS graphics, HPEN pen, 
        float x, float y, float width, float height, float rx, float ry);

#define MGPlusDrawRoundRect(graphics, pen, x, y, width, height, r) \
    MGPlusDrawRoundRectEx (graphics, pen, x, y, width, height, r, r)

/**
 * \fn MPStatus MGPlusDrawRoundRectIEx (HGRAPHICS graphics, HPEN pen, 
 * int x, int y, int width, int height, int rx, int ry)
 * \brief Draw a roundrect on the graphic.
 *
 * This function draws a roundrect on the graphic. 
 * 
 * \param graphics  The MGPlusGraphics pointer.
 * \param pen       The draw pen.
 * \param x         The left x of the rectangle.
 * \param y         The top y of the rectangle.
 * \param width     The width of the rectangle.
 * \param height    The height of the rectangle.
 * \param rx        The x-radius of round rect arc.
 * \param ry        The y-radius of round rect arc.
 *
 * \return Get Status, MP_GENERIC_ERROR indicates an error.
 *
 * \sa MGPlusDrawRectangle
 */
MGPLUS_EXPORT MPStatus MGPlusDrawRoundRectIEx (HGRAPHICS graphics, HPEN pen, 
        int x, int y, int width, int height, int rx, int ry);

#define MGPlusDrawRoundRectI(graphics, pen, x, y, width, height, r) \
    MGPlusDrawRoundRectIEx (graphics, pen, x, y, width, height, r, r)

/**
 * \fn MPStatus MGPlusFillRoundRectEx (HGRAPHICS graphics, HBRUSH brush, 
 * float x, float y, float width, float height, float rx, float ry)
 * \brief Draw a round rect on the graphic.
 *
 * This function draws a round rect on the graphic. 
 * 
 * \param graphics  The MGPlusGraphics pointer.
 * \param brush     The fill brush.
 * \param x         The left x of the rectangle.
 * \param y         The top y of the rectangle.
 * \param width     The width of the rectangle.
 * \param height    The height of the rectangle.
 * \param rx        The x-radius of round rect arc.
 * \param ry        The y-radius of round rect arc.
 *
 * \return Get Status, MP_GENERIC_ERROR indicates an error.
 *
 * \sa MGPlusFillRoundRectI
 */
MGPLUS_EXPORT MPStatus MGPlusFillRoundRectEx (HGRAPHICS graphics, HBRUSH brush, 
        float x, float y, float width, float height, float rx, float ry);

#define MGPlusFillRoundRect(graphics, brush, x, y, width, height, r) \
    MGPlusFillRoundRectEx (graphics, brush, x, y, width, height, r, r)

/**
 * \fn MPStatus MGPlusFillRoundRectIEx (HGRAPHICS graphics, HPEN pen, 
 * int x, int y, int width, int height, int rx, int ry)
 * \brief Draw a round rect on the graphic.
 *
 * This function draws a round rect on the graphic. 
 * 
 * \param graphics  The MGPlusGraphics pointer.
 * \param brush     The fill brush.
 * \param x         The left x of the rectangle.
 * \param y         The top y of the rectangle.
 * \param width     The width of the rectangle.
 * \param height    The height of the rectangle.
 * \param rx        The x-radius of round rect arc.
 * \param ry        The y-radius of round rect arc.
 *
 * \return Get Status, MP_GENERIC_ERROR indicates an error.
 *
 * \sa MGPlusFillRoundRect
 */
MGPLUS_EXPORT MPStatus MGPlusFillRoundRectIEx (HGRAPHICS graphics, HBRUSH brush, 
        int x, int y, int width, int height, int rx, int ry);

#define MGPlusFillRoundRectI(graphics, brush, x, y, width, height, r) \
        MGPlusFillRoundRectIEx (graphics, brush, x, y, width, height, r, r)

/**
 * \fn MPStatus MGPlusDrawEllipse (HGRAPHICS graphics, HPEN pen, 
 * float cx, float cy, float rx, float ry)
 * \brief Draw a ellipse on the graphic.
 *
 * This function draws a ellipse on the graphic. 
 * 
 * \param graphics  The MGPlusGraphics pointer.
 * \param pen       The draw pen.
 * \param cx        The X position of the center of the ellipse.
 * \param cy        The Y position of the center of the ellipse.
 * \param rx        The x-radius of the ellipse.
 * \param ry        The y-radius of the ellipse.
 *
 * \return Get Status, MP_GENERIC_ERROR indicates an error.
 *
 * \sa MGPlusDrawEllipseI
 */
MGPLUS_EXPORT MPStatus MGPlusDrawEllipse (HGRAPHICS graphics, HPEN pen, 
        float cx, float cy, float rx, float ry);

/**
 * \fn MPStatus MGPlusDrawEllipseI (HGRAPHICS graphics, HPEN pen, 
 *      int cx, int cy, int rx, int ry)
 *
 * \brief Draw a ellipse on the graphic.
 *
 * This function draws a ellipse on the graphic. 
 * 
 * \param graphics  The MGPlusGraphics pointer.
 * \param pen       The draw pen.
 * \param cx        The X position of the center of the ellipse.
 * \param cy        The Y position of the center of the ellipse.
 * \param rx        The x-radius of the ellipse.
 * \param ry        The y-radius of the ellipse.
 *
 * \return Get Status, MP_GENERIC_ERROR indicates an error.
 *
 * \sa MGPlusDrawEllipse
 */
MGPLUS_EXPORT MPStatus MGPlusDrawEllipseI (HGRAPHICS graphics, HPEN pen, 
        int cx, int cy, int rx, int ry);

/**
 * \fn MPStatus MGPlusFillEllipse (HGRAPHICS graphics, HBRUSH brush, 
 * float cx, float cy, float rx, float ry)
 * \brief Fill a ellipse on the graphic.
 *
 * This function fills a ellipse on the graphic. 
 * 
 * \param graphics  The MGPlusGraphics pointer.
 * \param brush     The fill brush.
 * \param cx        The X position of the center of the ellipse.
 * \param cy        The Y position of the center of the ellipse.
 * \param rx        The x-radius of the ellipse.
 * \param ry        The y-radius of the ellipse.
 *
 * \return Get Status, MP_GENERIC_ERROR indicates an error.
 *
 * \sa MGPlusFillEllipseI
 */
MGPLUS_EXPORT MPStatus MGPlusFillEllipse (HGRAPHICS graphics, HBRUSH brush, 
        float cx, float cy, float rx, float ry);

/**
 * \fn MPStatus MGPlusFillEllipseI (HGRAPHICS graphics, HBRUSH brush, 
 *      int cx, int cy, int rx, int ry)
 *
 * \brief Draw a ellipse on the graphic.
 *
 * This function draws a ellipse on the graphic. 
 * 
 * \param graphics  The MGPlusGraphics pointer.
 * \param brush     The fill brush.
 * \param cx        The X position of the center of the ellipse.
 * \param cy        The Y position of the center of the ellipse.
 * \param rx        The x-radius of the ellipse.
 * \param ry        The y-radius of the ellipse.
 *
 * \return Get Status, MP_GENERIC_ERROR indicates an error.
 *
 * \sa MGPlusFillEllipse
 */
MGPLUS_EXPORT MPStatus MGPlusFillEllipseI (HGRAPHICS graphics, HBRUSH brush, 
        int cx, int cy, int rx, int ry);

/**
 * \fn MPStatus MGPlusSetClipPath (HGRAPHICS graphics, HPATH path)
 *
 * \brief Set the path as a clip on the graphic.
 *
 * This function set the path as a clip on the graphic,
 * supported by mGPlus V1.2.1 or upper. 
 * 
 * \param graphics  The MGPlusGraphics pointer.
 * \param path      The clip path.
 *
 * \return Get Status, MP_GENERIC_ERROR indicates an error.
 *
 * \sa MGPlusFillPath
 */
MGPLUS_EXPORT MPStatus MGPlusSetClipPath (HGRAPHICS graphics, HPATH path);

/**
 * \fn MPStatus MGPlusFillPath (HGRAPHICS graphics, HBRUSH brush, 
 * HPATH path)
 *
 * \brief Fill the path with a brush on the graphic.
 *
 * This function fills the path with a brush on the graphic. 
 * 
 * \param graphics  The MGPlusGraphics pointer.
 * \param brush     The fill brush.
 * \param path      The path need to be filled.
 *
 * \return Get Status, MP_GENERIC_ERROR indicates an error.
 *
 * \sa MGPlusDrawPath
 */
MGPLUS_EXPORT MPStatus MGPlusFillPath (HGRAPHICS graphics, HBRUSH brush,
        HPATH path);

/**
 * \fn MPStatus MGPlusDrawPath (HGRAPHICS graphics, HPEN pen, 
 * HPATH path)
 *
 * \brief Draw the path with a pen on the graphic.
 *
 * This function draws the path with a pen on the graphic. 
 * 
 * \param graphics  The MGPlusGraphics pointer.
 * \param pen       The draw pen.
 * \param path      The path need to be drew.
 *
 * \return Get Status, MP_GENERIC_ERROR indicates an error.
 *
 * \sa MGPlusFillPath
 */
MGPLUS_EXPORT MPStatus MGPlusDrawPath (HGRAPHICS graphics, HPEN pen,
        HPATH path);

/**
 * \fn MPStatus MGPlusGraphicLoadBitmap (HGRAPHICS graphics, 
 * int n_index, PBITMAP p_bitmap);
 *
 * \brief Load the p_bitmap to graphic's inner resource.
 *
 * This function loads the p_bitmap to graphic's inner resource. 
 * 
 * \param graphics  The MGPlusGraphics pointer.
 * \param n_index   The resource number.
 * \param p_bitmap  The bitmap need to be drew.
 *
 * \return Get Status, MP_GENERIC_ERROR indicates an error.
 *
 * \sa MGPlusGraphicLoadBitmapFromFile
 */
MGPLUS_EXPORT MPStatus MGPlusGraphicLoadBitmap (HGRAPHICS graphics, int n_index,
        PBITMAP p_bitmap);

/**
 * \fn MPStatus MGPlusGraphicLoadBitmapFromFile (HGRAPHICS graphics,
 * int n_index, char* file); 
 *
 * \brief Load the file to graphic's inner resource.
 *
 * This function loads the file to graphic's inner resource. 
 * 
 * \param graphics  The MGPlusGraphics pointer.
 * \param n_index   The resource number.
 * \param file      The bitmap path.
 *
 * \return Get Status, MP_GENERIC_ERROR indicates an error.
 *
 * \sa MGPlusGraphicLoadBitmap
 */
MGPLUS_EXPORT MPStatus MGPlusGraphicLoadBitmapFromFile (HGRAPHICS graphics,
        int n_index, char* file);

/**
 * \fn MPStatus MGPlusGraphicUnLoadBitmap (HGRAPHICS graphics, 
 * int n_index);
 *
 * \brief UnLoad the p_bitmap to graphic's inner resource.
 *
 * This function unloads the p_bitmap to graphic's inner resource. 
 * 
 * \param graphics  The MGPlusGraphics pointer.
 * \param n_index   The resource number.
 *
 * \return Get Status, MP_GENERIC_ERROR indicates an error.
 *
 * \sa MGPlusGraphicLoadBitmap
 */
MGPLUS_EXPORT MPStatus MGPlusGraphicUnLoadBitmap (HGRAPHICS graphics, int n_index);

/**
 * \fn MPStatus MGPlusGraphicUnLoadBitmap (HGRAPHICS graphics, 
 * int n_index);
 *
 * \brief get the p_bitmap to graphic's inner resource.
 *
 * This function get the p_bitmap to graphic's inner resource. 
 * 
 * \param graphics  The MGPlusGraphics pointer.
 * \param n_index   The resource number.
 *
 * \return Get Status, MP_GENERIC_ERROR indicates an error.
 *
 * \sa MGPlusGraphicLoadBitmap
 */
MGPLUS_EXPORT PBITMAP MGPlusGraphicGetBitmap (HGRAPHICS graphics, int n_index);

/**
 * \fn MPStatus MGPlusSetImageAlpha (HGRAPHICS graphics, int alpha) 
 * 
 * \brief Set image alpha of graphic.
 *
 * This function set the image alpha of graphic. 
 * 
 * \param graphics  The MGPlusGraphics pointer.
 * \param alpha     The image alpha.
 *
 * \return Get Status, MP_GENERIC_ERROR indicates an error.
 *
 * \sa MGPlusGetImageAlpha
 */
MGPLUS_EXPORT MPStatus MGPlusSetImageAlpha (HGRAPHICS graphics, int alpha);

/**
 * \fn MPStatus MGPlusGetImageAlpha (HGRAPHICS graphics, int* alpha) 
 * 
 * \brief Get image alpha of graphic.
 *
 * This function get the image alpha of graphic. 
 * 
 * \param graphics  The MGPlusGraphics pointer.
 * \param alpha     The image alpha.
 *
 * \return Get Status, MP_GENERIC_ERROR indicates an error.
 *
 * \sa MGPlusSetImageAlpha
 */
MGPLUS_EXPORT MPStatus MGPlusGetImageAlpha (HGRAPHICS graphics, int* alpha);

/**
 * \fn MPStatus MGPlusDrawImageWithPoints (HGRAPHICS graphics, 
 *       int n_index, const MPPOINT* point, int count) 
 * \brief Draw the image with a series of points on the graphic.
 *
 * This function draws the image with a series of points on the graphic. 
 * 
 * \param graphics  The MGPlusGraphics pointer.
 * \param n_index   The image index.
 * \param point     The points pointer.
 * \param count     The points count.
 *
 * \return Get Status, MP_GENERIC_ERROR indicates an error.
 *
 * \sa MGPlusDrawImageWithPath 
 */
MGPLUS_EXPORT MPStatus MGPlusDrawImageWithPoints (HGRAPHICS graphics, 
        int n_index, const MPPOINT* point, int count);

/**
 * \fn MPStatus MGPlusDrawImage (HGRAPHICS graphics, 
 * int n_index, int x, int y, int w, int h)
 * 
 * \brief Draw the image with on the graphic.
 *
 * This function draws the image with a series of points on the graphic. 
 * 
 * \param graphics  The MGPlusGraphics pointer.
 * \param n_index   The graphic's inner resource index.
 * \param x         The x coordinate of the upper-left corner of the box.
 * \param y         The y coordinate of the upper-left corner of the box.
 * \param w         The width of the box. Can be zero, means the width
 *                      or the height will be equal to the width or the height of 
 *                  The BITMAP object.
 * \param h         The height of the box. Can be zero, means the width
 *                      or the height will be equal to the width or the height of 
 *                      the BITMAP object.
 *
 * \return Get Status, MP_GENERIC_ERROR indicates an error.
 *
 * \sa MGPlusDrawImageWithPath 
 */
MGPLUS_EXPORT MPStatus MGPlusDrawImage (HGRAPHICS graphics, int n_index,
        int x, int y, int w, int h);

/**
 * \fn MPStatus MGPlusDrawImageWithPath (HGRAPHICS graphics, 
 * int n_index, HPATH path)
 *
 * \brief Draw the image with a path on the graphic.
 *
 * This function draws the image with a path on the graphic. 
 * 
 * \param graphics  The MGPlusGraphics pointer.
 * \param n_index   The graphic's inner resource index.
 * \param path      The path need to be filled.
 *
 * \return Get Status, MP_GENERIC_ERROR indicates an error.
 *
 * \sa MGPlusDrawImageWithPoints
 */
MGPLUS_EXPORT MPStatus MGPlusDrawImageWithPath (HGRAPHICS graphics, 
        int n_index, HPATH path);

    /** @} end of render*/

    /**
    * \defgroup Path Path 
    *
    * MGPlus maintains some path defines and operations
    *
    * @{
    */

/**
 * \fn HPATH MGPlusPathCreate (MPFillMode brushMode)
 * \brief Create a path.
 *
 * This function creates a path, and return the HPATH. 
 * 
 * \param brushMode    The path filled mode.
 *
 * \return If success, it returns the HPATH.
 *
 * \sa MGPlusPathDelete
 */
MGPLUS_EXPORT HPATH MGPlusPathCreate (MPFillMode brushMode);

/**
 * \fn MPStatus MGPlusPathDelete (HPATH path)
 * \brief Delete the HPATH.
 *
 * This function deletes the path. 
 * 
 * \param path        The path need to be deleted.
 *
 * \return Get Status, MP_GENERIC_ERROR indicates an error.
 *
 * \sa MGPlusPathCreate
 */
MGPLUS_EXPORT MPStatus MGPlusPathDelete (HPATH path);

/**
 * \fn MPStatus MGPlusPathReset (HPATH path)
 * \brief Reset the path.
 *
 * This function resets the path. 
 * 
 * \param path        The path need to be reset.
 *
 * \return Get Status, MP_GENERIC_ERROR indicates an error.
 */
MGPLUS_EXPORT MPStatus MGPlusPathReset (HPATH path);

/**
 * \fn MPStatus MGPlusPathSetAllOrientation (HPATH path, MPOrientation orientation)
 * \brief Set all path orientation.
 *
 * This function set all path orientation. 
 * 
 * \param path         The path need to be set.
 * \param orientation  The path direction.
 *
 * \return Get Status, MP_GENERIC_ERROR indicates an error.
 */
MGPLUS_EXPORT MPStatus MGPlusPathSetAllOrientation (HPATH path,
        MPOrientation orientation);

/**
 * \fn MPStatus MGPlusPathGetVertex (HPATH path, int idx, 
 *                   double* x, double* y, int* cmd)
 * \brief Get the special idx vertex of the path.
 *
 * This function gets the key points count of the path. 
 * 
 * \param path         The path pointer.
 * \param idx          The vertex idx.
 * \param x            The pointer of get vertex's x coord .
 * \param y            The pointer of get vertex's y coord .
 * \param cmd          The vertex cmd type of point.
 *
 * \return Get Status, MP_GENERIC_ERROR indicates an error.
 */
MGPLUS_EXPORT MPStatus MGPlusPathGetVertex (HPATH path, int idx, 
        double* x, double* y, int* cmd);

/**
 * \fn MPStatus MGPlusPathGetPointCount(HPATH path, int* count)
 * \brief Get the key points count of the path.
 *
 * This function gets the key points count of the path. 
 * 
 * \param path         The path pointer.
 * \param count        The path count pointer.
 *
 * \return Get Status, MP_GENERIC_ERROR indicates an error.
 */
MGPLUS_EXPORT MPStatus MGPlusPathGetPointCount (HPATH path, int* count);

/**
 * \fn MPStatus MGPlusPathGetPoints (HPATH path, int* count, MPPOINT** pt)
 * \brief Get the key points coor of the path.
 *
 * This function gets the key points coor of the path. 
 * 
 * \param path         The path pointer.
 * \param count        The path count pointer.
 * \param pt           The path point coor pointer.
 *
 * \return Get Status, MP_GENERIC_ERROR indicates an error.
 */
MGPLUS_EXPORT MPStatus MGPlusPathGetPoints (HPATH path, int* count, MPPOINT** pt);

/**
 * \fn MPStatus MGPlusPathAddLine (HPATH path, float x1, float y1, 
 * float x2, float y2)
 * 
 * \brief Add a line to a path.
 *
 * This function adds a line to a path. 
 * 
 * \param path         The path pointer.
 * \param x1           The start x of the line.
 * \param y1           The start y of the line.
 * \param x2           The end x of the line.
 * \param y2           The end y of the line.
 *                     
 * \return Get Status, MP_GENERIC_ERROR indicates an error.
 *
 * \sa MGPlusPathAddLineI
 */
MGPLUS_EXPORT MPStatus MGPlusPathAddLine (HPATH path, float x1, float y1, 
        float x2, float y2); 

/**
 * \fn MPStatus MGPlusPathAddLineI (HPATH path, int x1, int y1, 
 * int x2, int y2)
 *
 * \brief Add a line to a path.
 *
 * This function adds a line to a path. 
 * 
 * \param path         The path pointer.
 * \param x1           The start x of the line.
 * \param y1           The start y of the line.
 * \param x2           The end x of the line.
 * \param y2           The end y of the line.
 *
 * \return Get Status, MP_GENERIC_ERROR indicates an error.
 *
 * \sa MGPlusPathAddLine
 */
MGPLUS_EXPORT MPStatus MGPlusPathAddLineI (HPATH path, int x1, int y1, 
        int x2, int y2);

/**
 * \fn MPStatus MPStatus MGPlusPathAddLines (HPATH path, const MPPOINT* points, 
        int count)
 * 
 * \brief Add a series lines to a path.
 *
 * This function adds a series lines to a path. 
 * 
 * \param path         The path pointer.
 * \param points       The points pointer.
 * \param count        The number of points.
 *                     
 * \return Get Status, MP_GENERIC_ERROR indicates an error.
 *
 * \sa MGPlusAddPathLine
 */
MGPLUS_EXPORT MPStatus MGPlusPathAddLines (HPATH path, const MPPOINT* points, 
        int count);

/**
 * \fn MPStatus MGPlusPathAddArc (HPATH path, float cx, float cy, 
 * float rx, float ry, float startangle, float sweepangle)
 *
 * \brief Add a arc to a path.
 *
 * This function adds a arc to a path. 
 * 
 * \param path         The path pointer.
 * \param cx           The x position of the center of ellipse that contains the arc.
 * \param cy           The y position of the center of ellipse that contains the arc.
 * \param rx           The x-radius of the ellipse.
 * \param ry           The y-radius of the ellipse.
 * \param startangle   The start angle.
 * \param sweepangle   The specifies the angle, between the starting point (startangle) 
 *
 * \return Get Status, MP_GENERIC_ERROR indicates an error.
 *
 * \sa MGPlusPathAddArcI
 */
MGPLUS_EXPORT MPStatus MGPlusPathAddArc (HPATH path, float cx, float cy, float rx, 
        float ry, float startAngle, float sweepAngle);

/**
 * \fn MPStatus MGPlusPathAddArcI (HPATH path, int cx, int cy, 
 * int rx, int ry, int startangle, int sweepangle)
 *
 * \brief Add a arc to a path.
 *
 * This function adds a arc to a path. 
 * 
 * \param path         The path pointer.
 * \param cx           The x position of the center of ellipse that contains the arc.
 * \param cy           The y position of the center of ellipse that contains the arc.
 * \param rx           The x-radius of the ellipse.
 * \param ry           The y-radius of the ellipse.
 * \param startangle   The start angle.
 * \param sweepangle   The specifies the angle, between the starting 
 *                         point(startangle)
 * \return Get Status, MP_GENERIC_ERROR indicates an error.
 *
 * \sa MGPlusPathAddArc
 */
MGPLUS_EXPORT MPStatus MGPlusPathAddArcI (HPATH path, int cx, int cy, 
        int rx, int ry, int startAngle, int sweepAngle);

/**
 * \fn MPStatus MGPlusPathAddBezier (HPATH path, float x1, float y1,
            float x2, float y2, float x3, float y3, float x4, float y4)
 *
 * \brief Add a bezier to the path .
 *
 * This function adds a bezier to the path. 
 * 
 * \param path        The path pointer.
 * \param x1          The start x of bezier.
 * \param y1          The start y of bezier.
 * \param x2          The first control x of bezier.
 * \param y2          The first control y of bezier.
 * \param x3          The second control x of bezier.
 * \param y3          The second control y of bezier.
 * \param x4          The end x of bezier.
 * \param y4          The end y of bezier.
 *
 * \return Get Status, MP_GENERIC_ERROR indicates an error.
 *
 * \sa MGPlusDrawBezierI
 */
MGPLUS_EXPORT MPStatus MGPlusPathAddBezier (HPATH path, float x1, float y1, 
        float x2, float y2, float x3, float y3, float x4, float y4);

/**
 * \fn MPStatus MGPlusPathAddBezierI (HPATH path, int x1, int y1,
                int x2, int y2, int x3, int y3, int x4, int y4)
 *
 * \brief Add a bezier to the path .
 *
 * This function adds a bezier to the path. 
 * 
 * \param path      The path pointer.
 * \param x1        The start x of bezier.
 * \param y1        The start y of bezier.
 * \param x2        The first control x of bezier.
 * \param y2        The first control y of bezier.
 * \param x3        The second control x of bezier.
 * \param y3        The second control y of bezier.
 * \param x4        The end x of bezier.
 * \param y4        The end y of bezier.
 *
 * \return Get Status, MP_GENERIC_ERROR indicates an error.
 *
 * \sa MGPlusDrawBezier
 */
MGPLUS_EXPORT MPStatus MGPlusPathAddBezierI (HPATH path, int x1, int y1, 
        int x2, int y2, int x3, int y3, int x4, int y4);

/**
 * \fn MPStatus MGPlusPathAddRoundRectEx (HPATH path, int x, int y, 
 *      int width, int height, int rx, int ry)
 *
 * \brief Add a round rect to the path .
 *
 * This function adds a bezier to the path. 
 * 
 * \param path      The path pointer.
 * \param x         The left x of the rectangle.
 * \param y         The top  y of the rectangle.
 * \param width     The width  of the rectangle.
 * \param height    The height of the rectangle.
 * \param rx        The radius width of round rect arc.
 * \param ry        The radius height of round rect arc.
 *                 
 * \return Get Status, MP_GENERIC_ERROR indicates an error.
 *                 
 * \sa MGPlusDrawBezier
 */
MGPLUS_EXPORT MPStatus MGPlusPathAddRoundRectEx (HPATH path, int x, int y, 
        int width, int height, int rx, int ry);

#define MGPlusPathAddRoundRect(path, x, y, width, height, r) \
    MGPlusPathAddRoundRectEx (path, x, y, width, height, r, r)

/**
 * \fn MPStatus MPStatus MGPlusPathArcto (HPATH path, double x1, 
 * double y1, double x2, double y2, double radius);
 *
 * \brief Add a arc to a path and two tangent line of arc.
 *
 * This function add a arc to a path and two tangent line of arc,
 * first line through subpath last vectors to (x1, y1), second line 
 * through one arc point to (x2, y2).
 * supported by mGPlus V1.2.1 or upper.
 * 
 * ---------(last vector) ---- (x1,y1) ---- (point of tangent).
 *
 *                                      arc
 *
 *                                         |(point of tangent).
 *                                         |
 *                                         | 
 *                                         (x2,y2)
 *
 *
 * \param path         The path pointer.
 * \param x1           The x coordinat of first point .
 * \param y1           The y coordinat of first point .
 * \param x2           The x coordinat of second point .
 * \param y2           The y coordinat of second point .
 * \param radius       The arc radius.
 *
 * \return Get Status, MP_GENERIC_ERROR indicates an error.
 *
 * \sa MGPlusPathArctoI
 */
MGPLUS_EXPORT MPStatus MGPlusPathArcto (HPATH path, double x1, double y1,
        double x2, double y2, double radius);

/**
 * \fn MPStatus MPStatus MGPlusPathArctoI (HPATH path, int x1, 
 * int y1, int x2, int y2, int radius);
 *
 * \brief Add a arc to a path and two tangent line through 
 * subpath last vectors, (x1, y1), (x2, y2).
 *
 * This function add a arc to a path and two tangent line through 
 * subpath last vectors, (x1, y1), (x2, y2),
 * supported by mGPlus V1.2.1 or upper.
 * 
 * \param path         The path pointer.
 * \param x1           The x coordinat of first point .
 * \param y1           The y coordinat of first point .
 * \param x2           The x coordinat of second point .
 * \param y2           The y coordinat of second point .
 * \param radius       The arc radius.
 *
 * \return Get Status, MP_GENERIC_ERROR indicates an error.
 *
 * \sa MGPlusPathArcto
 */
MGPLUS_EXPORT MPStatus MGPlusPathArctoI (HPATH path, int x1, int y1,
        int x2, int y2, int radius);

/**
 * \fn MPStatus MGPlusPathMoveto (HPATH path, float x, float y) 
 *
 * \brief Move the path to a new point.
 *
 * This function moves the path to a new point. 
 * 
 * \param path      The MGPlusPath pointer.
 * \param x         The left x of the point.
 * \param y         The top y of the point.
 *                  
 * \return Get Status, MP_GENERIC_ERROR indicates an error.
 *
 * \sa MGPlusPathMovetoI
 */
MGPLUS_EXPORT MPStatus MGPlusPathMoveto (HPATH path, float x, float y);

/**
 * \fn MPStatus MGPlusPathMovetoI (HPATH path, int x, int y) 
 *
 * \brief Move the path to a new point.
 *
 * This function moves the path to a new point. 
 * 
 * \param path      The MGPlusPath pointer.
 * \param x         The left x of the point.
 * \param y         The top y of the point.
 *                  
 * \return Get Status, MP_GENERIC_ERROR indicates an error.
 *
 * \sa MGPlusPathMoveto
 */
MGPLUS_EXPORT MPStatus MGPlusPathMovetoI (HPATH path, int x, int y);

/**
 * \fn MPStatus MGPlusPathLineto (HPATH path, float x, float y) 
 *
 * \brief Line the path to a new point.
 *
 * This function lines the path to a new point. 
 * 
 * \param path      The MGPlusPath pointer.
 * \param x         The left x of the point.
 * \param y         The top y of the point.
 *                  
 * \return Get Status, MP_GENERIC_ERROR indicates an error.
 *
 * \sa MGPlusPathLinetoI
 */
MGPLUS_EXPORT MPStatus MGPlusPathLineto (HPATH path, float x, float y);

/**
 * \fn MPStatus MGPlusPathLinetoI (HPATH path, int x, int y) 
 *
 * \brief Line the path to a new point.
 *
 * This function lines the path to a new point. 
 * 
 * \param path      The MGPlusPath pointer.
 * \param x         The left x of the point.
 * \param y         The top y of the point.
 *                  
 * \return Get Status, MP_GENERIC_ERROR indicates an error.
 *
 * \sa MGPlusPathLineto
 */
MGPLUS_EXPORT MPStatus MGPlusPathLinetoI (HPATH path, int x, int y);

/**
 * \fn MPStatus MGPlusPathBezierto (HPATH path, float x1,
 * float y1, float x2, float y2, float x3, float y3) 
 *
 * \brief Add a bezier curve to the last vector.
 *
 * This function adds a bezier curve to the last vector,
 * supported by mGPlus V1.2.1 or upper. 
 * 
 * \param path      The MGPlusPath pointer.
 * \param x1        The first control point x coordinate.
 * \param y1        The first control point y coordinate.
 * \param x2        The second control point x coordinate.
 * \param y2        The second control point y coordinate.
 * \param x3        The last point x coordinate.
 * \param y3        The last point y coordinate.
 *                  
 * \return Get Status, MP_GENERIC_ERROR indicates an error.
 *
 * \sa MGPlusPathBeziertoI
 */
MGPLUS_EXPORT MPStatus MGPlusPathBezierto (HPATH path, float x1, float y1,
        float x2, float y2, float x3, float y3);

/**
 * \fn MPStatus MGPlusPathBeziertoI (HPATH path, int x1,
 * int y1, int x2, int y2, int x3, int y3) 
 *
 * \brief Add a bezier curve to the last vector.
 *
 * This function adds a bezier curve to the last vector,
 * supported by mGPlus V1.2.1 or upper. 
 * 
 * \param path      The MGPlusPath pointer.
 * \param x1        The first control point x coordinate.
 * \param y1        The first control point y coordinate.
 * \param x2        The second control point x coordinate.
 * \param y2        The second control point y coordinate.
 * \param x3        The last point x coordinate.
 * \param y3        The last point y coordinate.
 *                  
 * \return Get Status, MP_GENERIC_ERROR indicates an error.
 *
 * \sa MGPlusPathBezierto
 */
MGPLUS_EXPORT MPStatus MGPlusPathBeziertoI (HPATH path, int x1, int y1,
        int x2, int y2, int x3, int y3);

/**
 * \fn MPStatus MGPlusPathQuadraticto (HPATH path, float x1,
 * float y1, float x2, float y2) 
 *
 * \brief Add a quadratic curve to the last vector.
 *
 * This function adds a quadratic curve to the last vector
 * supported by mGPlus V1.2.1 or upper. 
 * 
 * \param path      The MGPlusPath pointer.
 * \param x1        The control point x coordinate.
 * \param y1        The control point y coordinate.
 * \param x2        The last point x coordinate.
 * \param y2        The last point y coordinate.
 *                  
 * \return Get Status, MP_GENERIC_ERROR indicates an error.
 *
 * \sa MGPlusPathQuadratictoI
 */
MGPLUS_EXPORT MPStatus MGPlusPathQuadraticto (HPATH path, float x1,
        float y1, float x2, float y2);

/**
 * \fn MPStatus MGPlusPathQuadratictoI (HPATH path, int x1,
 * int y1, int x2, int y2) 
 *
 * \brief Add a quadratic curve to the last vector.
 *
 * This function adds a quadratic curve to the last vector
 * supported by mGPlus V1.2.1 or upper. 
 * 
 * \param path      The MGPlusPath pointer.
 * \param x1        The control point x coordinate.
 * \param y1        The control point y coordinate.
 * \param x2        The last point x coordinate.
 * \param y2        The last point y coordinate.
 *                  
 * \return Get Status, MP_GENERIC_ERROR indicates an error.
 *
 * \sa MGPlusPathQuadraticto
 */
MGPLUS_EXPORT MPStatus MGPlusPathQuadratictoI (HPATH path, int x1, 
        int y1, int x2, int y2); 

/**
 * \fn MPStatus MGPlusPathAddRectangle (HPATH path, float x, float y, 
 * float width, float height)
 *
 * \brief Add a rectangle to the path.
 *
 * This function adds a rectangle to the path. 
 * 
 * \param path      The MGPlusPath pointer.
 * \param x         The left x of the rectangle.
 * \param y         The top y of the rectangle.
 * \param width     The width of the rectangle.
 * \param height    The height of the rectangle.
 *                  
 * \return Get Status, MP_GENERIC_ERROR indicates an error.
 *
 * \sa MGPlusPathAddRectangleI
 */
MGPLUS_EXPORT MPStatus MGPlusPathAddRectangle (HPATH path, float x, float y, 
        float width, float height);

/**
 * \fn MPStatus MGPlusPathAddRectangleI (HPATH path, int x, int y, 
 * int width, int height)
 * 
 * \brief Add a rectangle to the path.
 *
 * This function adds a rectangle to the path. 
 * 
 * \param path      The path pointer.
 * \param x         The left x of the rectangle.
 * \param y         The top y of the rectangle.
 * \param width     The width of the rectangle.
 * \param height    The height of the rectangle.
 *
 * \return Get Status, MP_GENERIC_ERROR indicates an error.
 *
 * \sa MGPlusPathAddRectangle
 */
MGPLUS_EXPORT MPStatus MGPlusPathAddRectangleI (HPATH path, int x, int y, 
        int width, int height);

/**
 * \fn MPStatus MGPlusPathAddEllipse (HPATH path, float cx, float cy, 
 * float rx, float ry, BOOL clockwise)
 *
 * \brief Add a ellipse to a path.
 *
 * This function adds a ellipse to a path. 
 * 
 * \param path      The path pointer.
 * \param cx        The x position of the center of the ellipse.
 * \param cy        The y position of the center of the ellipse.
 * \param rx        The x-radius of the ellipse.
 * \param ry        The y-radius of the ellipse.
 * \param clockwise The direction, if TRUE, it is clockwise, else counter-clockwise.
 *
 * \return Get Status, MP_GENERIC_ERROR indicates an error.
 *
 * \sa MGPlusPathAddEllipseI
 */
MGPLUS_EXPORT MPStatus MGPlusPathAddEllipse (HPATH path, float cx, float cy, 
        float rx, float ry, BOOL clockwise);

/**
 * \fn MPStatus MGPlusPathAddEllipseI (HPATH path, int cx, int cy, 
 * int rx, int ry, BOOL clockwise)
 *
 * \brief Add a ellipse to a path.
 *
 * This function adds a ellipse to a path. 
 * 
 * \param path      The path pointer.
 * \param cx        The x position of the center of the ellipse.
 * \param cy        The y position of the center of the ellipse.
 * \param rx        The x-radius of the ellipse.
 * \param ry        The y-radius of the ellipse.
 * \param clockwise The direction if TRUE, it is clockwise, else counter-clockwise.
 *
 * \return Get Status, MP_GENERIC_ERROR indicates an error.
 *
 * \sa MGPlusPathAddEllipse
 */
MGPLUS_EXPORT MPStatus MGPlusPathAddEllipseI (HPATH path, int cx, int cy, 
        int rx, int ry, BOOL clockwise);

/**
 * \fn MPStatus MGPlusPathAddCurve (HPATH path, const MPPOINT* points, 
 * int count)
 *
 * \brief Add a curve to a path.
 *
 * This function adds a curve to a path. 
 * 
 * \param path      The path pointer.
 * \param points    The points pointer.
 * \param count     The points count.
 *
 * \return Get Status, MP_GENERIC_ERROR indicates an error.
 */
MGPLUS_EXPORT MPStatus MGPlusPathAddCurve (HPATH path, const MPPOINT* points, 
        int count);

/**
 * \fn MPStatus MGPlusPathAddPath (HPATH path, HPATH add_path) 
 *
 * \brief copy a path to the other path.
 *
 * This function copies a path to the other path. 
 * 
 * \param path      The dst path pointer.
 * \param add_path  The src path pointer.
 *
 * \return Get Status, MP_GENERIC_ERROR indicates an error.
 */
MGPLUS_EXPORT MPStatus MGPlusPathAddPath (HPATH path, HPATH add_path);

/**
 * \fn MPStatus MGPlusPathStartFigure (HPATH path)
 * \brief Add a new sub path.
 *
 * This function adds a new sub path. 
 * 
 * \param path      The path pointer.
 *
 * \return Get Status, MP_GENERIC_ERROR indicates an error.
 *
 * \sa MGPlusPathCloseFigure
 */
MGPLUS_EXPORT MPStatus MGPlusPathStartFigure (HPATH path);

/**
 * \fn MPStatus MGPlusPathCloseFigure (HPATH path)
 * \brief Close the current sub path.
 *
 * This function closes the current sub path. 
 * 
 * \param path      The path pointer.
 *
 * \return Get Status, MP_GENERIC_ERROR indicates an error.
 *
 * \sa MGPlusPathStartFigure
 */
MGPLUS_EXPORT MPStatus MGPlusPathCloseFigure (HPATH path);

/**
 * \fn MPStatus MGPlusPathRotateAroundPoint (HPATH path,
        const MPPOINT* pt, float angle)
 * \brief  Transform the special path to rotating around the point.
 *
 * \param path            The path. 
 * \param pt              The translate center point. 
 * \param angle           The the rotation angle.
 *
 * \return if success return MP_OK, else return MP_GENERIC_ERROR.
 */ 
MGPLUS_EXPORT MPStatus MGPlusPathRotateAroundPoint (HPATH path,
        const MPPOINT* pt, float angle);

/**
 * \fn MPStatus MGPlusPathSetTransform (HPATH path, MPMatrix* matrix)
 * \brief Set the transform matrix to the special path.
 *
 * \param path            The path. 
 * \param matrix          The translate matrix. 
 *
 * \return If success return MP_OK, else return MP_GENERIC_ERROR.
 */ 
MGPLUS_EXPORT MPStatus MGPlusPathSetTransform (HPATH path, MPMatrix* matrix);

/**
 * \fn MPStatus MGPlusPathGetTransform (HPATH path, MPMatrix* matrix)
 * \brief  Get the special path's transform matrix .
 *
 * This funciton gets the special path's transform matrix,
 * supported by mGPlus V1.2.1 or upper.
 * \param path            The path. 
 * \param matrix          The translate matrix. 
 *
 * \return if success return MP_OK, else return MP_GENERIC_ERROR.
 */ 
MGPLUS_EXPORT MPStatus MGPlusPathGetTransform (HPATH path, MPMatrix* matrix);

/**
 * \fn MPStatus MGPlusPathResetTransform (HPATH path);
 * \brief  Reset the transform matrix of the special path.
 *
 * \param path        The operate path. 
 *
 * \return if success return MP_OK, else return MP_GENERIC_ERROR.
 */ 
MGPLUS_EXPORT MPStatus MGPlusPathResetTransform (HPATH path);

/**
 * \fn MPStatus MGPlusPathScale(HPATH path, float sx, float sy);

 * \brief  Scale the special path.
 *
 * \param path        The operate path. 
 * \param sx          The x scaling factor.
 * \param sy          The y scaling factor.
 *
 * \return if success return MP_OK, else return MP_GENERIC_ERROR.
 */ 
MGPLUS_EXPORT MPStatus MGPlusPathScale(HPATH path, float sx, float sy);

/**
 * \fn MPStatus MGPlusPathRotate (HPATH path, float angle);
 * \brief  Scale the special path.
 *
 * \param path        path.
 * \param angle       The rotation angle.
 *
 * \return if success return MP_OK, else return MP_GENERIC_ERROR.
 */ 
MGPLUS_EXPORT MPStatus MGPlusPathRotate (HPATH path, float angle);

/**
 * \fn MPStatus MGPlusPathTranslate(HPATH path, float dx, float dy);
 * \brief  Scale the special path.
 *
 * \param path      The path.
 * \param dx        The dx translation distance.
 * \param dy        The dy translation distance.
 *
 * \return if success return MP_OK, else return MP_GENERIC_ERROR.
 */
MGPLUS_EXPORT MPStatus MGPlusPathTranslate(HPATH path, float dx, float dy);

    /** @} end of path */

    /**
    * \defgroup Pen Pen
    *
    * MGPlus maintains some pen defines and operations
    *
    * @{
    */

/**
 * \fn HPEN MGPlusPenCreate (int width, ARGB rgba)
 * \brief Create a pen.
 *
 * This function creates a pen. 
 * 
 * \param width           The width of the pen.
 * \param rgba            The rgba of the pen.
 *
 * \return Get Status, MP_GENERIC_ERROR indicates an error.
 *
 * \sa MGPlusPenDelete
 */
MGPLUS_EXPORT HPEN MGPlusPenCreate (int width, ARGB rgba);

/**
 * \fn MPStatus MGPlusPenSetColor (HPEN pen, ARGB rgba)
 * \brief Set the pen color.
 *
 * This function sets the pen color. 
 * 
 * \param pen             The pen.
 * \param rgba            The rgba of the pen.
 *
 * \return Get Status, MP_GENERIC_ERROR indicates an error.
 *
 * \sa MGPlusPenDelete
 */
MGPLUS_EXPORT MPStatus MGPlusPenSetColor (HPEN pen, ARGB rgba);

/**
 * \fn MPStatus MGPlusPenSetJoinStyle (HPEN pen, LINE_JOIN_E line_cap)
 * \brief Set the pen line join style.
 *
 * This function sets the pen join style. 
 * 
 * \param pen             The pen.
 * \param line_cap        The line join style of the pen.
 *
 * \return Get Status, MP_GENERIC_ERROR indicates an error.
 *
 * \sa MGPlusPenDelete
 */
MGPLUS_EXPORT MPStatus MGPlusPenSetJoinStyle (HPEN pen, LINE_JOIN_E line_join);

/**
 * \fn MPStatus MGPlusPenSetCapStyle (HPEN pen, LINE_CAP_E line_join);
 * \brief Set the pen line cap style.
 *
 * This function sets the pen line cap style. 
 * 
 * \param pen             The pen.
 * \param line_join       The line cap style of the pen.
 *
 * \return Get Status, MP_GENERIC_ERROR indicates an error.
 *
 * \sa MGPlusPenDelete
 */
MGPLUS_EXPORT MPStatus MGPlusPenSetCapStyle (HPEN pen, LINE_CAP_E line_cap);

/**
 * \fn MPStatus MGPlusPenSetDashes (HPEN pen, int dash_phase, 
 *         const unsigned char* dash_list, int dash_len);
 * \brief Set the pen dashes.
 *
 * This function sets the pen dash line style. 
 * 
 * \param pen             The pen.
 * \param dash_phase      The dash phase of pen.
 * \param dash_list       The dash vector value.
 * \param dash_len        The length of vector.
 *
 * \return Get Status, MP_GENERIC_ERROR indicates an error.
 *
 * \sa MGPlusPenDelete
 */
MGPLUS_EXPORT MPStatus MGPlusPenSetDashes (HPEN pen, int dash_phase, 
        const unsigned char* dash_list, int dash_len);

/**
 * \fn MPStatus MGPlusPenSetWidth (HPEN pen, int width);
 * \brief Set the pen width.
 *
 * This function sets the pen width. 
 * 
 * \param pen             The pen.
 * \param width           The width of the pen.
 *
 * \return Get Status, MP_GENERIC_ERROR indicates an error.
 *
 * \sa MGPlusPenDelete
 */
MGPLUS_EXPORT MPStatus MGPlusPenSetWidth (HPEN pen, int width);

/**
 * \fn MPStatus MGPlusPenDelete (HPEN pen)
 * \brief Delete a pen.
 *
 * This function deletes a pen. 
 * 
 * \param pen            The pen pointer.
 *
 * \return Get Status, MP_GENERIC_ERROR indicates an error.
 *
 * \sa MGPlusPenCreate
 */
MGPLUS_EXPORT MPStatus MGPlusPenDelete (HPEN pen);

    /** @} end of pen */

    /**
    * \defgroup Brush Brush
    *
    * MGPlus maintains some brush defines and operations
    *
    * @{
    */

/**
 * \fn HBRUSH MGPlusBrushCreate (MPBrushType type)
 * \brief Create a brush.
 *
 * This function creates a brush. 
 * 
 * \param type     The MPBrushType type.
 *
 * \return , INVALID_HANDLE indicates an error.
 *
 * \sa MGPlusBrushDelete
 */
MGPLUS_EXPORT HBRUSH MGPlusBrushCreate (MPBrushType type);

/**
 * \fn MPStatus MGPlusBrushDelete (HBRUSH brush)
 * \brief Delete a brush.
 *
 * This function deletes a brush. 
 * 
 * \param brush    The bursh pointer.
 *
 * \return Get Status, MP_GENERIC_ERROR indicates an error.
 *
 * \sa MGPlusBrushCreate
 */
MGPLUS_EXPORT MPStatus MGPlusBrushDelete (HBRUSH brush);

/**
 * \fn MPStatus MGPlusSetSolidBrushColor (HBRUSH brush, ARGB rgba)
 * \brief Set color with solid brush.
 *
 * This function sets color with solid brush. 
 * 
 * \param brush           The bursh pointer.
 * \param rgba            The color. 
 *
 * \return Get Status, MP_GENERIC_ERROR indicates an error.
 */
MGPLUS_EXPORT MPStatus MGPlusSetSolidBrushColor (HBRUSH brush, ARGB rgba);

/**
 * \fn MPStatus MGPlusGetSolidBrushColor (HBRUSH brush, ARGB* rgba)
 * \brief Set color with solid brush.
 *
 * This function sets color with solid brush. 
 * 
 * \param brush           The bursh pointer.
 * \param rgba            The color pointer. 
 *
 * \return Get Status, MP_GENERIC_ERROR indicates an error.
 */
MPStatus MGPlusGetSolidBrushColor (HBRUSH brush, ARGB* rgba);

/**
 * \fn MPStatus MGPlusSetTextureBrushImage (HBRUSH brush, 
 * BITMAP* bitmap)
 *
 * \brief Set image with texture brush.
 *
 * This function sets image with texture brush. 
 * 
 * \param brush           The bursh pointer.
 * \param bitmap          The BITMAP pointer.
 *
 * \return Get Status, MP_GENERIC_ERROR indicates an error.
 */
MGPLUS_EXPORT MPStatus MGPlusSetTextureBrushImage (HBRUSH brush, 
        BITMAP* bitmap);

/**
 * \fn MPStatus MGPlusSetPathGradientBrushCenterPoint (HBRUSH brush, 
 * MPPOINT* point)
 *
 * \brief Set the center point with path gradient brush.
 *
 * This function sets the center point with path gradient brush. 
 * 
 * \param brush           The bursh pointer.
 * \param point           The center point pointer.
 *
 * \return Get Status, MP_GENERIC_ERROR indicates an error.
 *
 * \sa MGPlusSetPathGradientBrushCenterColor 
 */
MGPLUS_EXPORT MPStatus MGPlusSetPathGradientBrushCenterPoint (HBRUSH brush, 
        MPPOINT* point);

/**
 * \fn MPStatus MGPlusSetPathGradientBrushCenterColor (HBRUSH brush, 
 *       ARGB rgba)
 * \brief Set the center point color with path gradient brush.
 *
 * This function sets the center point color with path gradient brush. 
 * 
 * \param brush           The bursh pointer.
 * \param rgba            The center point color.
 *
 * \return Get Status, MP_GENERIC_ERROR indicates an error.
 *
 * \sa MGPlusSetPathGradientBrushCenterPoint 
 */
MGPLUS_EXPORT MPStatus MGPlusSetPathGradientBrushCenterColor (HBRUSH brush, 
        ARGB rgba);

/**
 * \fn MPStatus MGPlusSetPathGradientBrushSurroundColors (HBRUSH brush, 
 *       ARGB* rgba, int count)
 * \brief Set the surround color with path gradient brush.
 *
 * This function sets the center point color with path gradient brush. 
 * 
 * \param brush           The bursh pointer.
 * \param rgba            The surround points color.
 * \param count           The surround color number.
 *
 * \return Get Status, MP_GENERIC_ERROR indicates an error.
 *
 * \sa MGPlusSetPathGradientBrushCenterColor 
 */
MGPLUS_EXPORT MPStatus MGPlusSetPathGradientBrushSurroundColors (HBRUSH brush, 
        ARGB* rgba, int count);

/**
 * \fn MPStatus MGPlusSetPathGradientBrushSurroundRect (HBRUSH brush, 
 *       RECT* rect)
 *
 * \brief Set the single color rect with path gradient brush.
 *
 * This function sets the single color rect with path gradient brush. 
 * 
 * \param brush           The bursh pointer.
 * \param rect            The rect pointer.
 *
 * \return Get Status, MP_GENERIC_ERROR indicates an error.
 *
 * \sa MGPlusSetPathGradientBrushCenterColor 
 */
MGPLUS_EXPORT MPStatus MGPlusSetPathGradientBrushSurroundRect (HBRUSH brush, 
        RECT* rect);

/**
 * \fn MPStatus MGPlusSetLinearGradientBrushMode (HBRUSH brush, 
 * MPLinearGradientMode mode)
 *
 * \brief Set the linear gradient brush mode.
 *
 * This function sets the linear gradient brush mode. 
 * 
 * \param brush           The bursh pointer.
 * \param mode            The linear gradient mode.
 *
 * \return Get Status, MP_GENERIC_ERROR indicates an error.
 *
 * \sa MGPlusGetLinearGradientBrushMode 
 */
MGPLUS_EXPORT MPStatus MGPlusSetLinearGradientBrushMode (HBRUSH brush, 
        MPLinearGradientMode mode);

/**
 * \fn MPStatus MGPlusGetLinearGradientBrushMode (HBRUSH brush, MPLinearGradientMode* mode)
 * \brief Get the linear gradient brush mode.
 *
 * This function gets the linear gradient brush mode. 
 * 
 * \param brush           The bursh pointer.
 * \param mode            The linear gradient mode pointer.
 *
 * \return Get Status, MP_GENERIC_ERROR indicates an error.
 *
 * \sa MGPlusSetLinearGradientBrushMode 
 */
MGPLUS_EXPORT MPStatus MGPlusGetLinearGradientBrushMode (HBRUSH brush, 
        MPLinearGradientMode* mode);

/**
 * \fn MPStatus MGPlusSetLinearGradientBrushRect (HBRUSH brush, 
 * RECT* rect)
 *
 * \brief Set the rect of the linear gradient brush.
 *
 * This function set the rect of the linear gradient brush. 
 * 
 * \param brush           The bursh pointer.
 * \param rect            The linear gradient brush valid rect pointer.
 *
 * \return Get Status, MP_GENERIC_ERROR indicates an error.
 */
MGPLUS_EXPORT MPStatus MGPlusSetLinearGradientBrushRect (HBRUSH brush, RECT* rect);

/**
 * \fn MGPlusSetLinearGradientBrushColors (HBRUSH brush, 
 * ARGB* colors, int count)
 *
 * \brief Set the start color and end color with the linear gradient brush.
 *
 * This function set the start color and end color with the linear gradient brush. 
 * 
 * \param brush           The bursh pointer.
 * \param colors          The linear graidnet brush's color pointer.
 * \param count           The number of linear gradient brush's colors.
 *
 * \return Get Status, MP_GENERIC_ERROR indicates an error.
 */
MGPLUS_EXPORT MPStatus MGPlusSetLinearGradientBrushColors (HBRUSH brush, 
        ARGB* colors, int count);

/**
 * \fn MGPlusSetLinearGradientBrushColorsEx (HBRUSH brush, 
 * ARGB* colors, int count, float* position)
 *
 * \brief Set the start color and end color with the linear gradient brush, and 
 * every color with position info.
 *
 * This function set the start color and end color with the linear gradient brush. 
 * 
 * \param brush           The bursh pointer.
 * \param colors          The linear graidnet brush's color pointer.
 * \param count           The number of linear gradient brush's colors.
 * \param position        The position info of linear gradient brush's colors.
 *
 * \return Get Status, MP_GENERIC_ERROR indicates an error.
 */
MGPLUS_EXPORT MPStatus MGPlusSetLinearGradientBrushColorsEx (HBRUSH brush, 
        ARGB* colors, int count, float* position);


/**
 * \fn MPStatus MGPlusLinearGradientBrushGetColors (HBRUSH brush, ARGB* color, float* position)
 *
 * \brief get the linear gradient brush color.
 *
 * This function get the linear gradient brush color,
 * supported by mGPlus V1.2.1 or upper. 
 * 
 * \param brush           The bursh pointer.
 * \param color           The input linear graidnet brush's color array pointer.
 * \param position           The input linear graidnet brush's position array pointer.
 *
 * \return Get Status, MP_GENERIC_ERROR indicates an error.
 */
MGPLUS_EXPORT MPStatus MGPlusLinearGradientBrushGetColors (HBRUSH brush, ARGB* color, 
        float* position);

/**
 * \fn MGPlusLinearGradientBrushGetColorNumber (HBRUSH brush)
 *
 * \brief get the linear gradient brush color number.
 *
 * This function get the linear gradient brush color number,
 * supported by mGPlus V1.2.1 or upper. 
 * 
 * \param brush           The bursh pointer.
 *
 * \return Get Gradient color number. 
 */
MGPLUS_EXPORT int MGPlusLinearGradientBrushGetColorNumber (HBRUSH brush);

/**
 * \fn MGPlusLinearGradientBrushAddColor (HBRUSH brush, ARGB color, float position);
 *
 * \brief Add the linear gradient brush color.
 *
 * This function adds the linear gradient brush color,
 * supported by mGPlus V1.2.1 or upper. 
 * 
 * \param brush           The bursh pointer.
 * \param color           The linear graidnet brush's color.
 * \param position        The linear gradient brush's color position info.
 *
 * \return Get Status, MP_GENERIC_ERROR indicates an error.
 */
MGPLUS_EXPORT MPStatus MGPlusLinearGradientBrushAddColor (HBRUSH brush, 
        ARGB color, float position);

    /** @} end of brush */

    /**
    * \defgroup Glyph Glyph
    *
    * MGPlus maintains some glyph defines and operations
    *
    * @{
    */

/**
 * \fn HFONT MGPlusCreateFont (const char* font_name, unsigned face_index, 
        MPGlyphRendering ren_type, unsigned int width, unsigned int height, BOOL flip_y);
 * \brief Create ttf font.
 *
 * This function get the glyph outline path from outline's data buffer. 
 *
 * \param font_name          The create font name. 
 * \param face_index         The font's face index. 
 * \param ren_type           The font render type.
 * \param width              The font width.
 * \param height             The font height.
 * \param flip_y             The glyph's y-coor flip, if true, flip 
 *                           the y coordinate.
 *
 * \return If success, it returns the font handle.
 */
MGPLUS_EXPORT HFONT MGPlusCreateFont (const char* font_name, 
        unsigned face_index, MPGlyphRendering ren_type,
        unsigned int width, unsigned int height, BOOL flip_y);

/**
 * \fn HPATH MGPlusDeleteFont (HFONT hfont)
 * \brief Delete font.
 *
 * This function destroy the special font. 
 * 
 * \param hfont              The handle pointer of font. 
 *
 * \return If success, it returns MP_OK.
 */
MGPLUS_EXPORT MPStatus MGPlusDeleteFont (HFONT hfont);

/**
 * \fn HPATH MGPlusGetGlyphPath (int x, int y, LPGLYPHDATA lpdata)
 * \brief Get the Glyph outline path.
 *
 * This function get the glyph outline path from outline's data buffer. 
 * 
 * \param x                  The x coordinate of glyph. 
 * \param y                  The y coordinate of glyph. 
 * \param lpdata             The pointer of glyph outline data. 
 *
 * \return If success, it returns the path.
 */
MGPLUS_EXPORT HPATH MGPlusGetGlyphPath (int x, int y, LPGLYPHDATA lpdata);

/**
 * \fn MPStatus MGPlusGetGlyphOutline (HFONT hfont, unsigned uchar, 
 *       LPGLYPHMETRICS lpgm, LPGLYPHDATA lpdata) 
 * \brief Get the font path or LPGLYPHMETERICS info through uFormat.
 *
 * This function get the font path from the glyph data. 
 * 
 * \param hfont              The pointer of font. 
 * \param uchar              The query character. 
 * \param lpgm               The LPGLYPHMETRICS pointer.
 * \param lpdata             The glyph data pointer. 
 *
 * \return Get Status, MP_GENERIC_ERROR indicates an error.
 */
MGPLUS_EXPORT MPStatus MGPlusGetGlyphOutline (HFONT hfont, unsigned uchar, 
        LPGLYPHMETRICS lpgm, LPGLYPHDATA lpdata);

/**
 * \fn MGPLUS_EXPORT MPStatus MGPlusDrawGlyph (HGRAPHICS graphics, HFONT hfont, 
 *            int x, int y, LPGLYPHDATA lpdata, ARGB color);
 *
 * \brief Draw glyph path.
 *
 * This function draw glyph at x/y position with glyph data, user should 
 * be special render glyph color.
 * 
 * \param graphics           The graphic handler. 
 * \param hfont              The pointer of font. 
 * \param x                  The x coordinate of glyph. 
 * \param y                  The y coordinate of glyph. 
 * \param lpdata             The glyph data pointer. 
 * \param color              The render glyph color. 
 *
 * \return Get Status, MP_GENERIC_ERROR indicates an error.
 */
MGPLUS_EXPORT MPStatus MGPlusDrawGlyph (HGRAPHICS graphics, HFONT hfont, 
        int x, int y, LPGLYPHDATA lpdata, ARGB color);

    /** @} end of glyph */

    /**
    * \defgroup Fashion Fashion LFRDR
    *
    * MGPlus maintains some Fashion LFRDR defines and operations
    *
    * @{
    */

/**
 * \fn MGPLUS_EXPORT BOOL MGPlusRegisterFashionLFRDR (void);
 *
 * \brief  Registers the Fashion LFRDR.
 *
 * \return if success return TRUE, else return FALSE.
 */ 
MGPLUS_EXPORT BOOL MGPlusRegisterFashionLFRDR (void);

/**
 * \fn MGPLUS_EXPORT BOOL MGPlusUnregisterFashionLFRDR (void);
 *
 * \brief  Unregisters the Fashion LFRDR.
 *
 * \return if success return TRUE, else return FALSE.
 */ 
MGPLUS_EXPORT BOOL MGPlusUnregisterFashionLFRDR (void);


    /** @} end of fashionLF */


#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif
