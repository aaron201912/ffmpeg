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

#ifndef _MGUI_NCSCTRL_ANIMATEFRAMES_H
#define _MGUI_NCSCTRL_ANIMATEFRAMES_H

#ifdef _cplusplus
extern "C"{
#endif

#define NCSCTRL_ANIMATEFRAMES NCSCLASSNAME("animateframes")

typedef struct _mAnimateFrames mAnimateFrames;
typedef struct _mAnimateFramesClass mAnimateFramesClass;

#define mAnimateFramesHeader(clss) \
	mObjectHeader(clss) \
	int max_width; \
	int max_height; \
	int nr_frames; \
	void * cur_frame;

struct _mAnimateFrames{
	mAnimateFramesHeader(mAnimateFrames)
};

#define NCSR_ANIMATEFRAME_OK 0
#define NCSR_ANIMATEFRAME_LASTFRAME 1
#define NCSR_ANIMATEFRAME_FAILED -1

#define mAnimateFramesClassHeader(clss, superCls) \
	mObjectClassHeader(clss, superCls) \
	BOOL (*getMaxFrameSize)(mAnimateFrames* self, int *pwidth, int *pheight); \
	int (*drawFrame)(mAnimateFrames* self, HDC hdc, mObject *owner, RECT *pRect, int align, int valign, BOOL bScale); \
	int (*nextFrame)(mAnimateFrames* self);

struct _mAnimateFramesClass{
	mAnimateFramesClassHeader(mAnimateFrames, mObject)
};

MGNCS_EXPORT extern mAnimateFramesClass g_stmAnimateFramesCls;

#ifdef _cplusplus
}
#endif

#endif
