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

#ifndef MINICTRL_DATA_BINDING_H
#define MINICTRL_DATA_BINDING_H

#ifdef __cplusplus
extern "C"{
#endif

typedef struct _mBindPropClass mBindPropClass;
#ifndef BIND_PROP
#define BIND_PROP
typedef struct _mBindProp mBindProp;
#endif

#define mBindPropClassHeader(clss, superCls) \
	mObjectClassHeader(clss, superCls) \
	DWORD (* get)(clss *); \
	BOOL (* set)(clss *, DWORD newdata);

struct _mBindPropClass{
	mBindPropClassHeader(mBindProp, mObject)
};

MGNCS_EXPORT extern mBindPropClass g_stmBindPropCls;

typedef struct _mBindPropListNode{
	mBindProp * p;
	struct _mBindPropListNode * next;
}mBindPropListNode;

#define NCS_PROP_FLAG_READ  0x01
#define NCS_PROP_FLAG_WRITE 0x02
#define mBindPropHeader(clss) \
	mObjectHeader(clss) \
	int read:1; \
	int write:1; \
	int vtype:14; \
	int ref:16; \
	mBindPropListNode * listens; \
	mBindProp * next;

#define MAKE_BINDPROP_INFO(br, bw, type) \
	(((br)?NCS_PROP_FLAG_READ:0)|((bw)?NCS_PROP_FLAG_WRITE:0)|((type)<<16))

struct _mBindProp{
	mBindPropHeader(mBindProp)
};
///////////////////////////////////////////
MGNCS_EXPORT void ncsBindPropRasieChanged(mBindProp *bprop);

MGNCS_EXPORT int  ncsBindPropAddRef(mBindProp *bprop);
MGNCS_EXPORT int  ncsBindPropRelease(mBindProp *bprop);
MGNCS_EXPORT void ncsBindPropAutoRelfect(mBindProp *bprop);

MGNCS_EXPORT BOOL ncsConnectBindProps(mBindProp * psource, mBindProp* plistener, int type);

enum ncsBindPropType{
	NCS_BPT_SINGLE = 0,
	NCS_BPT_DBL
};

//////////////////////////////////////////////
MGNCS_EXPORT BOOL ncsAutoReflectObjectBindProps(mObject * obj);
MGNCS_EXPORT BOOL ncsRemoveObjectBindProps(mObject *obj);

MGNCS_EXPORT BOOL ncsAddObjectBindProps(mObject *obj, mBindProp *bprop);

typedef BOOL (*NCS_TEST_BINDPROP)(mBindProp*, void *param);
typedef mBindProp* (*NCS_NEW_BINDPROP)(void* param);

MGNCS_EXPORT mBindProp* ncsRegisterBindProp(mObject* obj, 
		NCS_TEST_BINDPROP test_exist,
		NCS_NEW_BINDPROP  new_prop,
		void *param);

typedef BOOL (*NCS_RAISE_TEST)(mBindProp* prop, DWORD param);
MGNCS_EXPORT void ncsRaiseObjectBindProps(mObject *obj, NCS_RAISE_TEST  raise_test, DWORD param);

////////////////////////////////////////
MGNCS_EXPORT void *ncsNewBindPropsObjGroup(void);
MGNCS_EXPORT void ncsDeleteBindPropsGroup(void*);
MGNCS_EXPORT void ncsAutoReflexBindPropsGroup(void* group);
MGNCS_EXPORT void ncsAddBindPropsObj(void *group, mObject *obj);

/////////////////////////////////////
// component

typedef struct _mCompBindPropClass mCompBindPropClass;
typedef struct _mCompBindProp mCompBindProp;

#define mCompBindPropClassHeader(clss, superCls) \
	mBindPropClassHeader(clss, superCls)

struct _mCompBindPropClass{
	mCompBindPropClassHeader(mCompBindProp, mBindProp)
};

MGNCS_EXPORT extern mCompBindPropClass g_stmCompBindPropCls;

#define mCompBindPropHeader(clss) \
	mBindPropHeader(clss) \
	mComponent * host; \
	int  prop_id; \
	int  event_id;

struct _mCompBindProp{
	mCompBindPropHeader(mCompBindProp)
};


MGNCS_EXPORT void ncsRaiseComponentBindProps(mComponent *comp, int event_id);

MGNCS_EXPORT mBindProp * ncsRegisterComponentBindProp(mComponent* comp, int event_id, int prop_id, int data_type, DWORD flags);
#define NCS_CMPT_PROP(comp, event_id, prop_id, data_type, flags) \
	ncsRegisterComponentBindProp((mComponent*)(comp), (event_id), (prop_id), (data_type), flags)


////////////////////////////////////////
// simple value data

typedef struct _mVariableBindPropClass mVariableBindPropClass;
typedef struct _mVariableBindProp mVariableBindProp;

#define mVariableBindPropClassHeader(clss, super) \
	mBindPropClassHeader(clss, super)

struct _mVariableBindPropClass{
	mVariableBindPropClassHeader(mVariableBindProp, mBindProp)
};

MGNCS_EXPORT extern mVariableBindPropClass g_stmVariableBindPropCls;

#define mVariableBindPropHeader(clss) \
	mBindPropHeader(clss) \
	void *p;

struct _mVariableBindProp{
	mVariableBindPropHeader(mVariableBindProp)
};

#define _SET_VAR(type, left, right) \
	*((type*)(left)) = ((type)(right))


MGNCS_EXPORT mBindProp * ncsRegisterVariableBindProp(void *pvalue, int data_type, DWORD flags);
#define VAR_PROP(value, data_type, flags) \
	ncsRegisterVariableBindProp((void*)(&(value)), (data_type), (flags))

#define INT_PROP(value, flags)  VAR_PROP(value, NCS_BT_INT, flags)
#define STRPTR_PROP(value, flags) VAR_PROP(value, NCS_BT_STR_PTR, flags)

#define ncsRaiseVariableBindProp(pvalue) \
	ncsRaiseObjectBindProps((mObject*)(pvalue),NULL, 0)


MGNCS_EXPORT BOOL ncsInitDataBinding(void);

#ifdef __cplusplus
}
#endif

#endif

