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

#ifndef _MGNCS_DATA_SOURCE_H
#define _MGNCS_DATA_SOURCE_H

#ifdef __cplusplus
extern "C"{
#endif

typedef struct _mRecordSetClass mRecordSetClass;
typedef struct _mRecordSet mRecordSet;

typedef struct _mDataSourceClass mDataSourceClass;
typedef struct _mDataSource mDataSource;

#define NCS_DS_SELECT_READ  0x01
#define NCS_DS_SELECT_WRITE 0x02

#define MSQLITE_READONLY		0x00000001
#define MSQLITE_READWRITE		0x00000002
#define MSQLITE_CREATE			0x00000004

#define mDataSourceClassHeader(clss, superCls) \
	mObjectClassHeader(clss, superCls) \
	mRecordSet * (*selectRecordSet)(clss *, const char* select, int select_type);  \
	DWORD (*getValue)(clss*, const char* mql,BOOL *bok); \
	BOOL  (*setValue)(clss*, const char* mql, DWORD value); \
	mBindProp * (*getBindProp)(clss*, const char* mql, int bp_type); \
	BOOL  (*createValue)(clss*, const char* ql, DWORD init_value); \
	BOOL  (*removeValue)(clss*, const char* ql);                   \
	BOOL  (*execute)(clss*, const char* mql); /*execute the query language*/

struct _mDataSourceClass {
	mDataSourceClassHeader(mDataSource, mObject)
};

#define mDataSourceHeader(clss) \
	mObjectHeader(clss)

struct _mDataSource {
	mDataSourceHeader(mDataSource)
};

MGNCS_EXPORT extern mDataSourceClass g_stmDataSourceCls;

////////////////////////
#define NCS_RS_CURSOR_BEGIN 0
#define NCS_RS_CURSOR_CUR   1
#define NCS_RS_CURSOR_END   2

#define NCS_RS_INSERT_AFTER  0
#define NCS_RS_INSERT_APPEND 1
#define NCS_RS_INSERT_BEFORE 2

#define NCS_RSE_CURSOR_UPDATE 0x01
#define NCS_RSE_VALUE_UPATED  0x02
#define NCS_RSE_VALUE_CREATE  0x04
#define NCS_RSE_VALUE_REMOVED 0x08

#define mRecordSetClassHeader(clss, superCls) \
	mObjectClassHeader(clss, superCls) \
	int (*getFieldCount)(clss*); \
	int (*getRecordCount)(clss*); \
	int (*getFieldType)(clss*, int filed_idx); \
	DWORD (*getCursor)(clss *); \
	BOOL  (*setCursor)(clss *, DWORD cursor); \
	DWORD (*seekCursor)(clss *, int seek, int offset); \
	DWORD (*getField)(clss *, int idx); \
	BOOL  (*setField)(clss *, int idx, DWORD value); \
	BOOL  (*newRecord)(clss *, int insert_type);  \
	BOOL  (*deleteRecord)(clss *); \
	BOOL  (*update)(clss *); \
	BOOL  (*cancel)(clss *); \
	BOOL  (*isEnd)(clss *); \
	mBindProp * (*getFieldBind)(clss *, int field_idx, int event_filter, int bp_type); \
	mBindProp * (*getCursorBind)(clss *, int bp_type);

struct _mRecordSetClass{
	mRecordSetClassHeader(mRecordSet, mObject)
};

#define mRecordSetHeader(clss) \
	mObjectHeader(clss)

struct _mRecordSet{
	mRecordSetHeader(mRecordSet)
};

MGNCS_EXPORT extern mRecordSetClass g_stmRecordSetCls;

//////////////////////////////////////////////////////
// Bind Prop

typedef struct _mDataBindPropClass mDataBindPropClass;
typedef struct _mDataBindProp mDataBindProp;

#define mDataBindPropClassHeader(clss, supper) \
	mBindPropClassHeader(clss,  supper) \

struct _mDataBindPropClass{
	mDataBindPropClassHeader(mDataBindProp, mBindProp)
};

MGNCS_EXPORT extern mDataBindPropClass g_stmDataBindPropCls;

#define mDataBindPropHeader(clss) \
	mBindPropHeader(clss) \
	unsigned int event_filter:16; \
	unsigned int revert:16;

struct _mDataBindProp {
	mDataBindPropHeader(mDataBindProp)
};

typedef struct _mDataValueBindPropClass mDataValueBindPropClass;
typedef struct _mDataValueBindProp mDataValueBindProp;

#define mDataValueBindPropClassHeader(clss, super) \
	mDataBindPropClassHeader(clss, super)

struct _mDataValueBindPropClass{
	mDataValueBindPropClassHeader(mDataValueBindProp, mDataBindProp)
};

#define mDataValueBindPropHeader(clss) \
	mDataBindPropHeader(clss) \
	DWORD *pvalue;

struct _mDataValueBindProp{
	mDataValueBindPropHeader(mDataValueBindProp)
};


typedef struct _mDataFieldBindPropClass mDataFieldBindPropClass;
typedef struct _mDataFieldBindProp mDataFieldBindProp;

#define mDataFieldBindPropClassHeader(clss, super) \
	mDataBindPropClassHeader(clss, super)

struct _mDataFieldBindPropClass {
	mDataFieldBindPropClassHeader(mDataFieldBindProp, mDataBindProp)
};

#define mDataFieldBindPropHeader(clss) \
	mDataBindPropHeader(clss) \
	mRecordSet * recordSet;

//user mDataBindProp::revert as field_idx

struct _mDataFieldBindProp {
	mDataFieldBindPropHeader(mDataFieldBindProp)
};


MGNCS_EXPORT void ncsRaiseDataSourceBindProps(mObject *obj, int event_filter);

MGNCS_EXPORT mBindProp * ncsRegisterDataValueBindProp(mDataSource *obj, DWORD *pvalue, int type, int flags, int event_filter);

MGNCS_EXPORT mBindProp * ncsRegisterDataFieldBindProp(mRecordSet *rs, int field_idx, int flags, int event_filter);

#define DATA_VAR_PROP(owner, pvalue ,data_type, flags, event_filter) \
	ncsRegisterDataValueBindProp((mDataSource*)(owner), (DWORD*)(pvalue), (int)(data_type), (int)(flags), (int)(event_filter));

#define DATA_FIELD_PROP(rs, field, flags, event_filer) \
	ncsRegisterDataFieldBindProp((mRecordSet*)(rs), (int)(field), (int)(flags), (int)(event_filter))


////////////////////////////////////////////////////////
// Default DataSource

BOOL ncsInitDefaultDataSource(void);
//mStaticDataSource
MGNCS_EXPORT extern mDataSource* g_pStaticDS;

MGNCS_EXPORT BOOL ncsRegisterStaticData(const char* pvalue_path, void ** array, int array_count, int field_count, int filed_width, const unsigned char* field_types);

MGNCS_EXPORT BOOL ncsRegisterStaticValue(const char* pvalue_path, DWORD value, int type);
#define ncsRegisterStaticString(pvalue_path, str) do{ \
	static char _str[] = str; \
	ncsRegisterStaticValue(pvalue_path, _str, NCS_BT_STR; \
}while(0)


//mTextDataSource
MGNCS_EXPORT mDataSource* ncsCreateTextDataSource(const char* source_file, int spliter, int mode);

//mEtcDataSource
MGNCS_EXPORT mDataSource* ncsCreateEtcDataSource(const char* etc_file, int mode);

MGNCS_EXPORT mDataSource* ncsCreateEtcDataSourceByHandle(GHANDLE hEtc, int mode);

//mSQLiteDataSource
#ifdef _MGNCSDB_SQLITE
MGNCS_EXPORT mDataSource* ncsCreateSQLiteDataSource(const char* sqlite_file, int mode);
#endif

////////////////////////////////////////////////////////


#ifdef __cplusplus
}
#endif

#endif

