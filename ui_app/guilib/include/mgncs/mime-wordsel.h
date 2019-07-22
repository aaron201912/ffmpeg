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

#ifdef _MGNCSCTRL_IMWORDSEL

#ifndef _MGUI_IM_WORDSEL_H
#define _MGUI_IM_WORDSEL_H

#ifdef __cplusplus
extern "C" {
#endif

#define NCSCTRL_IMWORDSEL  NCSCLASSNAME("imwordsel")

typedef struct _mIMWordSelClass mIMWordSelClass;
typedef struct _mIMWordSel      mIMWordSel;
typedef struct _mIMWordSelRenderer mIMWordSelRenderer;

#define mIMWordSelClassHeader(clss, super)                  \
	mWidgetClassHeader(clss, super)                     \
	BOOL  (*nextPage)(clss*);                           \
	BOOL  (*prevPage)(clss*);                           \
	BOOL  (*selNext)(clss*);                            \
	BOOL  (*selPrev)(clss*);

#define mIMWordSelHeader(clss)                              \
	mWidgetHeader(clss)                                 \
	mIMIterator*  ime_it;                               \
    BOOL          isactive;                             \
	int           cur_sel;                              \
	int           page_start:16;                        \
	int           page_end:16;                          \
//	CHARSET_CONVERTER * charset_converter;

struct _mIMWordSelClass {
	mIMWordSelClassHeader(mIMWordSel, mWidget)
};

struct _mIMWordSel {
	mIMWordSelHeader(mIMWordSel)
};

MGNCS_EXPORT extern mIMWordSelClass g_stmIMWordSelCls;

#define mIMWordSelRendererHeader(clss, super)         \
	mWidgetRendererHeader(clss, super)

struct _mIMWordSelRenderer {
	mIMWordSelRendererHeader(mIMWordSel, mWidget)
};

enum mIMWordSelProp {
	NCSP_IMWS_ITERATOR  = NCSP_WIDGET_MAX + 1,
	NCSP_IMWS_CURSEL,
	NCSP_IMWS_CURWORD, //return mIMIterator*, whose current value is the word selected
	NCSP_IMWS_CURRAWWORD, //raw word, not convert it to the encoding need by application
//	NCSP_IMWS_CHARSETCONVERTER,
	NCSP_IMWS_ISEMPTY,
    NCSP_IMWS_ISACTIVE,
	NCSP_IMWS_MAX
};

#define mIMWordSel_isEmpty(self)  \
	(BOOL)(_c((mIMWordSel*)(self))->getProperty((mIMWordSel*)(self), NCSP_IMWS_ISEMPTY))

#define mIMWordSel_getCurSel(self)  \
	((int)(NCS_GET_PROP(self, NCSP_IMWS_CURSEL)))
#define mIMWordSel_setCurSel(self, int_v)  \
	 NCS_SET_PROP(self, NCSP_IMWS_CURSEL,(DWORD)(int_v))

#define mIMWordSel_getCurWord(self)  \
	((const char*)(NCS_GET_PROP(self, NCSP_IMWS_CURWORD)))
#define mIMWordSel_setCurWord(self, const_char_v) \
	NCS_SET_PROP(self, NCSP_IMWS_CURWORD, (DWORD)(const_char_v))

enum mIMWordSelEvent {
	NCSN_IMWS_SELCHANGED = NCSN_WIDGET_MAX + 1,
	NCSN_IMWS_MAX
};

#ifdef __cplusplus
}
#endif

#endif
#endif		//_MGNCSCTRL_IMWORDSEL

