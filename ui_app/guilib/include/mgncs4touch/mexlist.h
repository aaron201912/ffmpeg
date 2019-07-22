/*
 * \file mexlist.h
 * \author miniStudio team of FMSoft
 * \date 2010/10/09
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

#ifndef _MGUI_NCSCTRL_EXLIST_H
#define _MGUI_NCSCTRL_EXLIST_H
 

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#define NCSCTRL_EXLIST          NCSCLASSNAME("exlist")

typedef struct _mExList           mExList;
typedef struct _mExListClass      mExListClass;
typedef struct _mExListRenderer   mExListRenderer;

//#define NCSS_EXLST_LOOP              (1<<NCSS_LIST_SHIFT)
//#define NCSS_EXLST_MULTIPLE          (2<<NCSS_LIST_SHIFT)
#define NCSS_EXLST_SHIFT             (NCSS_ASTLST_SHIFT+0) 

#define mExListHeader(clsName)  \
	mListHeader(clsName)        \
    unsigned long effector;     \
    int duration;               \
    int key;                    \
    int mouseY; 

struct _mExList
{
	mExListHeader(mExList)
};

#define mExListClassHeader(clsName, superCls)               \
	mListClassHeader(clsName, superCls)                     \
    void (*runAnimation)(clsName*, int startV, int endV);
    
struct _mExListClass
{
	mExListClassHeader(mExList, mList)
};

#define mExListRendererHeader(clsName, parentClass) \
	mListRendererHeader(clsName, parentClass)

struct  _mExListRenderer {
	mExListRendererHeader(mExList, mList)
};

enum mExListProp
{
    NCSP_EXLIST_EFFECTOR = NCSP_LIST_MAX,
    NCSP_EXLIST_MAX,
};

enum mExListNotify
{
    NCSN_EXLIST_NODE_CHANGED = NCSN_LIST_MAX,
    NCSN_EXLIST_MAX,
};


MTOUCH_EXPORT extern mExListClass g_stmExListCls;

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif /* _MGUI_NCSCTRL_EXLIST_H */

