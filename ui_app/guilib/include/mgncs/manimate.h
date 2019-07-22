/**
 * \file manimate.h
 * \brief This file includes declaration of mAnimate.
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
#ifdef _MGNCSCTRL_ANIMATE

#ifndef _MGUI_NCSCTRL_ANIMATESTATIC_H
#define _MGUI_NCSCTRL_ANIMATESTATIC_H


#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#define NCSCTRL_ANIMATE        NCSCLASSNAME("animate")

typedef struct _mAnimate mAnimate;
typedef struct _mAnimateClass mAnimateClass;
typedef struct _mAnimateRenderer mAnimateRenderer;


#define mAnimateHeader(clsName) \
	mStaticHeader(clsName)	\
	unsigned int state:4;  \
	unsigned int paint_flag; \
	unsigned int interval:24; \
	char *path;	\
	HDC  mem_dc;

struct _mAnimate
{
	mAnimateHeader(mAnimate)
};

#define mAnimateClassHeader(clsName, parentClass) \
	mStaticClassHeader(clsName, parentClass) \
	BOOL (*play)(clsName*); \
	BOOL (*pauseResume)(clsName*); \
	BOOL (*stop)(clsName*);

struct _mAnimateClass
{
	mAnimateClassHeader(mAnimate, mStatic)
};

#define mAnimateRendererHeader(clsName, parentClass) \
	mStaticRendererHeader(clsName, parentClass)

struct _mAnimateRenderer {
	mAnimateRendererHeader(mAnimate, mStatic)
};

MGNCS_EXPORT extern mAnimateClass g_stmAnimateCls;

enum mAnimateProp {
	NCSP_ANMT_GIFFILE = NCSP_STATIC_MAX + 1,
	NCSP_ANMT_DIR,
	NCSP_ANMT_INTERVAL,
	NCSP_ANMT_MEM, //gif memory
	NCSP_ANMT_BMPARRAY, //bmp array end by NULL, e.g PBITMAP bmps={&bmp1, &bmp2, ..., NULL}
	NCSP_ANMT_AFRAME, //set the animate frame object
	NCSP_ANMT_MAX,
};

#define NCSS_ANMT_SHIFT  (NCSS_STATIC_SHIFT + 1)

#define NCSS_ANMT_AUTOLOOP (0x1<<NCSS_ANMT_SHIFT)

#define NCSS_ANMT_SCALE	(0x2<<NCSS_ANMT_SHIFT)

#define NCSS_ANMT_AUTOFIT	(0x4<<NCSS_ANMT_SHIFT)

#define NCSS_ANMT_AUTOPLAY   (0x8<<NCSS_ANMT_SHIFT)

enum enumAnimateNotify{
	NCSN_ANMT_MAX = NCSN_STATIC_MAX + 1
};

#define ANIM_PLAY 0x01
#define ANIM_PAUSE 0x02
#define ANIM_STOP 0x04
#define ANIM_STARTPLAY 0xF112
#define ANIM_PAUSE_RESUME 0xF113
#define ANIM_STOPPLAY 0xF114

#define ncsAnimateStart(self)  ((self)?_c(self)->play(self):FALSE)

#define ncsAnimatePauseResume(self)  ((self)?_c(self)->pauseResume(self):FALSE)

#define ncsAnimateStop(self) ((self)?_c(self)->stop(self):FALSE)

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif
#endif		//_MGNCSCTRL_ANIMATE
