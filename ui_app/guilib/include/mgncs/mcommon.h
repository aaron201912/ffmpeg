/**
 * \file 
 * \author 
 * \date 
 *
 * \brief 
 *
 \verbatim

    This file is part of mGNCS, a component for MiniGUI.

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

#ifndef MINICOMMON_H
#define MINICOMMON_H

#if defined (WIN32) 

#ifdef __MGNCS_LIB__
	#define MGNCS_EXPORT       __declspec(dllexport)
#else
	#define MGNCS_EXPORT       __declspec(dllimport)
#endif
#else
	#define MGNCS_EXPORT       
#endif

typedef struct Allocator{
	void *(*alloc)(void* pold, size_t new_size);
	size_t (*getsize)(void* p);
	void (*free)(void* p);
}Allocator;

Allocator * ncsGetDefaultAllocator(void);


/**
 * \enum enumNCSAlign
 */
enum enumNCSAlign{
	NCS_ALIGN_LEFT = 0,
	NCS_ALIGN_RIGHT,
	NCS_ALIGN_CENTER
};

/**
 * \enum enumNCSVAlign
 */
enum enumNCSVAlign{
	NCS_VALIGN_TOP = 0,
	NCS_VALIGN_BOTTOM,
	NCS_VALIGN_CENTER
};

/**
 * \enum enumNCSImageDrawMode
 */
enum enumNCSImageDrawMode{
	NCS_DM_NORMAL = 0,
	NCS_DM_SCALED,
	NCS_DM_TILED
};

//support alloca
#ifdef HAVE_ALLOCA
#define ALLOCA(size)  alloca(size)
#define FREEA(p)
#else
#define ALLOCA(size) malloc(size)
#define FREEA(p) free(p)
#endif

MGNCS_EXPORT void ncsDrawImage(HDC hdc, PBITMAP pbmp, const RECT * rc, int mode, int align, int valign);

#define IMG_TYPE_UNKNOWN  0
#define IMG_TYPE_BITMAP   1
#define IMG_TYPE_ICON     2
#define IMG_TYPE_MYBITMAP 4

#define IMG_FLAG_IGNORE   0
#define IMG_FLAG_UNLOAD   1
#define IMG_FLAG_RELEASE_RES  2
typedef struct _ImageDrawInfo{
	unsigned char img_type;
	unsigned char flag;
	unsigned char drawMode;
	unsigned char revert;
	union{
		PBITMAP pbmp;
		HICON hIcon;
		PMYBITMAP pmybmp;
		RES_KEY key;
	}img;
}ImageDrawInfo;

MGNCS_EXPORT void ncsInitDrawInfo(ImageDrawInfo *idi);

MGNCS_EXPORT void ncsCleanImageDrawInfo(ImageDrawInfo * idi);

MGNCS_EXPORT BOOL ncsSetImageDrawInfoByFile(ImageDrawInfo *idi, const char* image_file, int drawMode, BOOL bas_mybitmp);

MGNCS_EXPORT BOOL ncsSetImageDrawInfoByRes(ImageDrawInfo *idi, RES_KEY key, int drawMode, int res_type);

MGNCS_EXPORT BOOL ncsSetImageDrawInfo(ImageDrawInfo *idi, void* pimg, int drawMode, int img_type);


MGNCS_EXPORT void ncsImageDrawInfoDraw(ImageDrawInfo *idi, HDC hdc, const RECT* prc, int align, int valign);

MGNCS_EXPORT BOOL ncsImageDrawInfoGetImageSize(ImageDrawInfo *idi, int *px, int *py);

#endif

