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

#ifndef _MGNCS_ANIMATINEDITPIECE_H
#define _MGNCS_ANIMATINEDITPIECE_H

#ifdef __cplusplus
extern "C" {
#endif

typedef enum{
    TEXT_ALIGN_LEFT,
    TEXT_ALIGN_CENTER,
    TEXT_ALIGN_RIGHT,
}text_align;

typedef struct _mAnimationEditPieceClass mAnimationEditPieceClass;
typedef struct _mAnimationEditPiece mAnimationEditPiece;

#define mAnimationEditPieceClassHeader(clss, superCls) \
	mStaticPieceClassHeader(clss, superCls)\
    void (*setContent)(HWND hwnd,mAnimationEditPiece *self, const char* str,text_align align); \
    int (*getContent)(mAnimationEditPiece *self, char *strbuff, int buf_len); \
    void (*append)(HWND hwnd,mAnimationEditPiece *self, const char* str); \
    void (*tailDelete)(HWND hwnd,mAnimationEditPiece *self, int num); 

struct _mAnimationEditPieceClass
{
	mAnimationEditPieceClassHeader(mAnimationEditPiece, mStaticPiece)
};

MGNCS_EXPORT extern mAnimationEditPieceClass g_stmAnimationEditPieceCls;

#define mAnimationEditPieceHeader(clss) \
	mStaticPieceHeader(clss) \
    char *text; \
    int content_length; \
    int text_offset; \
    PLOGFONT *pFont; \
    ARGB text_color;\
    ARGB text_shadow_color;
    

struct _mAnimationEditPiece
{
	mAnimationEditPieceHeader(mAnimationEditPiece)
};

#define ANIMATINEDITPIECE_PROP_BEGIN    USER_PIECE_PROP_BEGIN + 450
enum mAnimationEditPieceProps {   
    NCSP_ANIMATIONEDITPIECE_FONT = ANIMATINEDITPIECE_PROP_BEGIN,
    NCSP_ANIMATIONEDITPIECE_TEXTCOLOR,
    NCSP_ANIMATIONEDITPIECE_TEXTSHADOWCOLOR,
};

#ifdef __cplusplus
}
#endif
#endif /*_MGNCS_ANIMATINEDITPIECE_H*/
