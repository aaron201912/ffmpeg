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

#ifndef _MGNCS_PANELPIECE_H
#define _MGNCS_PANELPIECE_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include <mgplus/mgplus.h>
#include <mgeff/mgeff.h>

//delete object
#define HOTPIECE_RELEASE(self) \
    if (self) { \
        --(self->objRefCount); \
        if (self->objRefCount <= 0) { \
            DELPIECE((mHotPiece*)self); \
        } \
    } 

extern int s_totalObjRef;
extern int s_totalPieceRef;

//#define DBG_REF
#ifdef DBG_REF
#define DBG_REFLOG(s, s1, s2, s3) fprintf(stderr, s, s1, __FUNCTION__, __LINE__, s2, s3);
#else
#define DBG_REFLOG(s, s1, s2, s3)
#endif
#define ADDREF(self) do { DBG_REFLOG("ADDREF %p, %s:%d objRefCount = %d, s_totalObjRef = %d.\n", self, self->objRefCount, ++s_totalObjRef); \
    _c(self)->addRef(self);\
    }while(0)
#define UNREF(self) do { if (NULL != self) {\
            DBG_REFLOG("UNREF %p, %s:%d objRefCount = %d, s_totalObjRef = %d\n", self, self->objRefCount, --s_totalObjRef); \
            self->objStatus |= NCSF_OBJECT_TODEL; \
            _c(self)->release(self);\
        }\
    }while(0)

#define ADDREFPIECE(self) do { DBG_REFLOG("ADDREFPIECE %p, %s:%d objRefCount = %d, s_totalPieceRef = %d.\n", self, self->objRefCount, ++s_totalPieceRef); \
    _c(self)->addRef(self);\
    }while(0)
#define UNREFPIECE(self) do { if (NULL != self) { \
        DBG_REFLOG("UNREFPIECE %p, %s:%d objRefCount = %d, s_totalPieceRef = %d\n", self, self->objRefCount, --s_totalPieceRef); \
        self->objStatus |= NCSF_OBJECT_TODEL; \
        HOTPIECE_RELEASE(self);\
    } \
}while(0);

#define MSG_PANELPIECE_LOCKED (MSG_USER + 0x3042)

typedef struct _POINTF
{
    float xf;
    float yf;
} POINTF;

enum mPanelPieceProp
{
    NCSP_PANEL_CLIPRECT = USER_PIECE_PROP_BEGIN + 200,
};


typedef struct _mPanelPiece mPanelPiece;
typedef struct _mPanelPieceClass mPanelPieceClass;

#define mPanelPieceHeader(clss) \
	mStaticPieceHeader(clss)    \
    BOOL isTopPanel;            \
    BOOL shouldResetPaintMode;   \
    mAbstractItemManager *itemManager; \
    list_t eventHandlers;      \
    mShapeTransRoundPiece* bkgndPiece;     \
    mHotPiece* hovering_piece; \
    mLayoutManager *layout;    \
    mWidget* owner;            \
    BOOL update_flag;          \
    MGEFF_ANIMATION update_anim;    \
    MGEFF_ANIMATION animgrp;        \
    DWORD addData; \
    RECT clipRect; \
    RECT invalidRect; 

struct _mPanelPiece
{
	mPanelPieceHeader(mPanelPiece)
};

typedef int (*NCS_PIECE_EVENT_HANDLER)(mHotPiece *, int message, WPARAM, LPARAM, mObject *owner);

#define mPanelPieceClassHeader(clss, superCls) \
	mStaticPieceClassHeader(clss, superCls)              \
    mPieceItem* (*addContent)(clss*, mHotPiece*, int, int);        \
    mPieceItem* (*addContentToLayout)(clss*, mHotPiece*);        \
	void (*enableChildCache)(clss*, mHotPiece*, BOOL);	    \
    BOOL (*updateChildCache)(clss*, mHotPiece*);            \
	void (*addContentFinished)(clss*);	\
    BOOL (*delContent)(clss*, mHotPiece*);                  \
    void (*clearContents)(clss*);                           \
    void (*reLayout)(clss*);                                \
    void (*setLayoutManager)(clss*, mLayoutManager *layout);\
    void (*appendEventHandler)(clss *, int message, NCS_PIECE_EVENT_HANDLER); \
    void (*delEventHandler)(clss *, int); \
    void (*clearEventHandler)(clss *); \
    mPieceItem* (*searchItem)(clss*, mHotPiece*);          \
    void (*setClipRect)(clss *, mPieceItem*);                \
    mHotPiece *(*childAt)(clss *, int, int);                \
    mAbstractItemManager* (*createItemManager)(clss*); \
    mPieceItem* (*createItemNode)(clss*, mHotPiece*, int, int); \
    void (*initItemNode)(clss*, mPieceItem*, mHotPiece*); \
    void (*movePiece)(clss*, mHotPiece*, int, int);                 \
    void (*scalePiece)(clss*, mHotPiece*, float, float);                \
    void (*rotatePiece)(clss*, mHotPiece*, float, int, int, int);   \
    void (*setPieceAlpha)(clss*, mHotPiece*, int);                  \
    MGEFF_ANIMATION (*movePieceWithAnimation)(clss*, mHotPiece*, int, int, int, enum EffMotionType);                \
    MGEFF_ANIMATION (*scalePieceWithAnimation)(clss*, mHotPiece*, float, float, int, enum EffMotionType);               \
    MGEFF_ANIMATION (*rotatePieceWithAnimation)(clss*, mHotPiece*, float, int, int, int, int, enum EffMotionType);  \
    MGEFF_ANIMATION (*setPieceAlphaWithAnimation)(clss*, mHotPiece*, int, int, enum EffMotionType); \
    void (*setOwner)(clss*, mWidget*);    \
    mWidget *(*getOwner)(clss*); \
    void (*animationSyncRunAndDelete)(clss *self, MGEFF_ANIMATION); \
    void (*animationAsyncRun)(clss *self, MGEFF_ANIMATION, int keepalive); \
    mShapeTransRoundPiece* (*getBkgndPiece)(clss *); \
    void (*invalidatePiece)(clss *, mHotPiece *piece, const RECT *rc, BOOL reserveCache); \
    void (*setBkgndPiece)(clss *, mShapeTransRoundPiece*);

struct _mPanelPieceClass
{
	mPanelPieceClassHeader(mPanelPiece, mStaticPiece)
};

MGNCS_EXPORT extern mPanelPieceClass g_stmPanelPieceCls;


// global functions
extern void PanelPiece_invalidatePiece(mHotPiece *piece, const RECT *rc);
extern void PanelPiece_update(mHotPiece *piece, BOOL fErase);
extern mPanelPiece *PanelPiece_getTopPanel(mHotPiece *self);

#define PanelPiece_isTopPanel(self) \
    ( ( (self)->parent == NULL ) && ( (self)->owner != NULL ) )

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _MGNCS_PANELPIECE_H */
