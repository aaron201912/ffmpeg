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

#ifndef _MGUI_IME_H
#define _MGUI_IME_H

#ifdef _MGNCSENGINE_IME

#ifdef __cplusplus
extern "C" {
#endif

#define IME_STOP_MSG      1
#define IME_CONTINUE_MSG  0

typedef int (*NCSCB_LEN_FIRST_CHAR)(const  unsigned char*, int len);

typedef int (*NCSCB_RETRIEVE_CHAR)(const char*);
typedef struct _CHARSET_CONVERTER
{
	//get a char who has a special code
	NCSCB_RETRIEVE_CHAR retrieve_char;
	void * conv_info;
    void * reserved;
}CHARSET_CONVERTER;

MGNCS_EXPORT BOOL ncsIMInitCharsetConv(CHARSET_CONVERTER* conv, const char* from_charset, const char* to_charset, NCSCB_RETRIEVE_CHAR retrieve_char);
MGNCS_EXPORT void ncsIMFreeCharsetConv(CHARSET_CONVERTER* conv);
MGNCS_EXPORT unsigned int ncsIMCharsetConv(CHARSET_CONVERTER* conv, const char ** pwords);
MGNCS_EXPORT char*  ncsIMConvertWord(CHARSET_CONVERTER* conv, const char* word, char* out, int len);

MGNCS_EXPORT NCSCB_LEN_FIRST_CHAR ncsIMGetFirstCharCallback(const char* encoding);

typedef struct _mIMEngineClass mIMEngineClass;
typedef struct _mIMEngine      mIMEngine;
typedef struct _mIMIteratorClass mIMIteratorClass;
typedef struct _mIMIterator      mIMIterator;

#define NCS_IM_SCANCODE_ISDIGIT(s)   ((s) >= SCANCODE_1 && (s) <= SCANCODE_0)
#define NCS_IM_SCANCODE_ISALPHA(s)  \
	(   ((s) >= SCANCODE_Q && (s) <= SCANCODE_P)   \
	 || ((s) >= SCANCODE_A && (s) <= SCANCODE_L)   \
	 || ((s) >= SCANCODE_Z && (s) <= SCANCODE_M)   \
	)

enum mIMIteratorProp{
    NCSP_IMEIT_CHARTYPE
};

#define mIMIteratorClassHeader(clss, super)              \
	mObjectClassHeader(clss, super)                  \
	PUBLIC void  (*empty)(clss*);                    \
	PUBLIC int   (*count)(clss*);                    \
	PUBLIC int   (*next)(clss*);                     \
	PUBLIC int   (*prev)(clss*);                     \
	PUBLIC int   (*locate)(clss*,int off, int type); \
	PUBLIC int   (*curIndex)(clss*);                 \
    PUBLIC DWORD (*setProperty)(clss*, int id, DWORD valude);   \
    PUBLIC DWORD (*getProperty)(clss*, int id);   \
	PUBLIC const char*(*word)(clss*);

#define mIMIteratorHeader(clss)                          \
	mObjectHeader(clss)                              \
	CHARSET_CONVERTER *charset_converter;


struct _mIMIteratorClass {
	mIMIteratorClassHeader(mIMIterator, mObject)
};

struct _mIMIterator {
	mIMIteratorHeader(mIMIterator)
};

MGNCS_EXPORT extern mIMIteratorClass g_stmIMIteratorCls;

#define MAX_IME_NAME_LEN  16
#define mIMEngineClassHeader(clss, super)                      \
	mObjectClassHeader(clss, super)                        \
    const char*  (*imeName)(clss*);                      \
	mIMEngine *  (*create)(const char* pdict, DWORD add_data);             \
	const char*  (*getEncoding)(clss*);                    \
	PROTECTED NCSCB_RETRIEVE_CHAR (*getRetrieveChar)(clss*);\
	mIMIterator* (*newIterator)(clss*);                    \
	mIMIterator* (*newAssocIterator)(clss*);               \
	/* find words */                                       \
	BOOL         (*findWords)(clss*, mIMIterator* it, const char* input, int start);     \
	BOOL         (*findAssociate)(clss*, mIMIterator* it, const char* input);

#define mIMEngineHeader(clss)                                  \
	mObjectHeader(clss)

struct _mIMEngineClass{
	mIMEngineClassHeader(mIMEngine, mObject)
};

struct _mIMEngine {
	mIMEngineHeader(mIMEngine)
};

MGNCS_EXPORT extern mIMEngineClass g_stmIMEngineCls;

typedef struct _mIMEClass mIMEClass;
typedef struct _mIME      mIME;

typedef struct _mIMManagerClass mIMManagerClass;
typedef struct _mIMManager      mIMManager;


#define NCS_MAX_INPUT            128
#define NCS_IME_UPDATE_KEY       0x01
#define NCS_IME_UPDATE_WORDS     0x02
#define NCS_IME_UPDATE_FEEDBACK  0x04
#define NCS_IME_UPDATE_CURSOR    0x08
#define NCS_IME_UPDATE_ALL       0xFFFF

#define NCS_IMM_BEGIN            (MSG_USER + 100)
#define NCS_IMM_UPDATE           (NCS_IMM_BEGIN + 1)
// int update_code = wParam
#define NCS_IMM_GETSELWORD       (NCS_IMM_BEGIN + 2)
// BOOL braw = wParam
#define NCS_IMM_ISSELWORDCTRLACTIVE       (NCS_IMM_BEGIN + 3)
#define NCS_IMM_SETSELWORDACTIVE       (NCS_IMM_BEGIN + 4)

#define mIMEClassHeader(clss, super)                            \
	mObjectClassHeader(clss, super)                         \
	/** implement by drived class **/                       \
	PROTECTED  BOOL (*showIME)(clss*);                      \
	PROTECTED  void (*hideIME)(clss*);                      \
	PROTECTED  void (*update)(clss*, DWORD flfags);         \
	PROTECTED  const char* (*getSelWord)(clss*, BOOL braw); \
	PROTECTED  DWORD (*getIMStatus)(clss*, int);            \
	PROTECTED  BOOL  (*setIMStatus)(clss*,int, DWORD);      \
	PROTECTED  BOOL  (*onCaretPos)(clss*, int,int);         \
	PROTECTED  BOOL  (*findWords)(clss*, const char* input, int start, int len); \
	PROTECTED  BOOL  (*findAssociate)(clss*, const char* words);                 \
	PROTECTED  BOOL  (*emptyKey)(clss*);                    \
	PROTECTED  BOOL  (*setEncoding)(clss*, const char*);    \
	PROTECTED BOOL (*addChar)(clss*, int ch);               \
	PROTECTED BOOL (*delChar)(clss*,BOOL bback);            \
	PROTECTED BOOL (*setInsert)(clss*, int pos, int type);  \
	PROTECTED BOOL (*clearInput)(clss*);                    \
	PROTECTED int  (*onChar)(clss*, int ch,DWORD keyflags); \
	PROTECTED int  (*onKeyDown)(clss*,int scancode, DWORD keyflags);  \
	PUBLIC void (*setIMEngine)(clss*, mIMEngine*);          \
	PUBLIC void (*empty)(clss*, BOOL bUpdate);


#define mIMEHeader(clss)                                        \
	mObjectHeader(clss)                                     \
	mIME* next;                                             \
	char  input[NCS_MAX_INPUT];                             \
	int   len;                                              \
	int   start:16;                                         \
	int   insert:16;                                        \
	mIMManager  *ime_manager;                               \
	mIMEngine   *ime_engine;                                \
	mIMIterator *key_it;                                    \
	mIMIterator *assoc_it;                                  \
	CHARSET_CONVERTER charset_conv;                         \
	HWND         hwnd_view; /** the additonaldata of hwnd is saved the mIME pointer**/ \
	WNDPROC      old_view_proc;

struct _mIMEClass {
	mIMEClassHeader(mIME, mObject)
};

struct _mIME {
	mIMEHeader(mIME)
};

MGNCS_EXPORT extern mIMEClass g_stmIMECls;

#define mIME_sendWords(ime)  (_c(ime->ime_manager)->sendWords(ime->ime_manager))
#define mIME_sendKeys(ime)   (_c(ime->ime_manager)->sendKeys(ime->ime_manager))

#define mIME_FromView(hwnd)  ((mIME*)GetWindowAdditionalData(hwnd))
MGNCS_EXPORT void mIME_attach(mIME *ime, HWND hwnd);

MGNCS_EXPORT void mIME_passMessage(mIME* ime, UINT message, WPARAM wParam, LPARAM lParam);
#define mIMView_passMessage(hwnd, message, wParam, lParam) \
	mIME_passMessage(mIME_FromView(hwnd), (message), (wParam), (lParam))

#define mIMManagerClassHeader(clss, super)                     \
	mObjectClassHeader(clss, super)                        \
	PROTECTED int (*procIMMsg)(clss*,UINT,WPARAM,LPARAM,LRESULT*); \
	PROTECTED BOOL (*nextIME)(clss*);                      \
	PROTECTED BOOL (*setIME)(clss*, const char*);          \
	PUBLIC BOOL (*addIME)(clss*, mIME *ime);               \
	PROTECTED BOOL (*sendWords)(clss*);                    \
	PROTECTED BOOL (*sendKeys)(clss*);                     \
	PUBLIC void (*setEncoding)(clss*, const char*);

#define mIMManagerHeader(clss)                                 \
	mObjectHeader(clss)                                    \
	NCSCB_LEN_FIRST_CHAR len_first_char;                   \
	HWND   hwnd;                                           \
	HWND   hwnd_target;                                    \
	mIME * header;                                         \
	mIME * cur_ime;                                        \
	DWORD  flags;                                          \
	int    ime_x:16;                                       \
	int    ime_y:16;

#define NCSF_IMM_DISABLE  0x01
#define NCSF_IMM_ACTIVED  0x02
#define NCSF_IMM_NULLIME  0x04
#define NCSF_IMM_SETTING_IME 0x80000000

enum NCSIMMEvent {
    NCSN_IMM_IMEOPEN = 5678,
    NCSN_IMM_IMECHANGED,
    NCSN_IMM_IMECLOSE,
};

struct _mIMManagerClass {
	mIMManagerClassHeader(mIMManager, mObject)
};
struct _mIMManager {
	mIMManagerHeader(mIMManager)
};

MGNCS_EXPORT extern mIMManagerClass g_stmIMManagerCls;

#endif //_MGNCSENGINE_IME

/**
 * IME implement for mobile or phone, who only has a phone keybord
 */
#ifdef _MGNCSENGINE_DIGIT

#define NCS_DIGIT_MAX_FINDS   32

#define mDigitIteratorClassHeader(clss, super)                 \
	mIMIteratorClassHeader(clss, super)                    \
	PUBLIC mIMIterator* (*subIterator)(clss*); /** unneed  free the return value! **/

#define mDigitIteratorHeader(clss)                             \
	mIMIteratorHeader(clss)                                \
	int           count;                                   \
	int           cur;                                     \
	mIMIterator * sub_it;                                  \
	const char**  words;                                   \
	int           finds[NCS_DIGIT_MAX_FINDS][2]; /*begin and end */

DECLARE_CLASS(mDigitIterator, mIMIterator)

#define mDigitIMEClassHeader(clss, super)                      \
	mIMEClassHeader(clss, super)

#define mDigitIMEHeader(clss)                                  \
	mIMEHeader(clss)                                       \
	CHARSET_CONVERTER sub_conv;

DECLARE_CLASS(mDigitIME, mIME)

// digital engine
#define mDigitEngineClassHeader(clss, super)                 \
	mIMEngineClassHeader(clss, super)                    \
	PROTECTED BOOL (*findRange)(clss*,                   \
			const char* input,                   \
			int start, int len,                  \
			int *pbegin, int *pend);

#define mDigitEngineHeader(clss)                             \
	mIMEngineHeader(clss)                                \
	mIMEngine*     sub_engine;

DECLARE_CLASS(mDigitEngine, mIMEngine)

#endif //_MGNCSENGINE_DIGIT

#ifdef _MGNCSENGINE_DIGITPTI
////////////////////////////
//digitpti
#define mDigitPtiEngineClassHeader(clss, super)              \
	mDigitEngineClassHeader(clss, super)

#define mDigitPtiEngineHeader(clss)                          \
	mDigitEngineHeader(clss)    \
    int case_type;

DECLARE_CLASS(mDigitPtiEngine, mDigitEngine)

#define mDigitPtiIteratorClassHeader(clss, super)           \
	mDigitIteratorClassHeader(clss, super)

#define mDigitPtiIteratorHeader(clss)                       \
	mDigitIteratorHeader(clss)                          \
	char* letters; //only for signle letters, for example "a\0b\0\c\0"

DECLARE_CLASS(mDigitPtiIterator, mDigitIterator)

#endif //_MGNCSENGINE_DIGITPTI

//////////////////////////////
//digit pinyin

#ifdef _MGNCSENGINE_DIGITPY

#define mDigitPyEngineClassHeader(clss, super)             \
	mDigitEngineClassHeader(clss, super)

#define mDigitPyEngineHeader(clss)                          \
	mDigitEngineHeader(clss)                            \
	int               py_count;                         \
	unsigned int *    keys;                             \
	const char**      pinyins;

DECLARE_CLASS(mDigitPyEngine, mDigitEngine)

#define mDigitPyIteratorClassHeader(clss, super)           \
	mDigitIteratorClassHeader(clss, super)


#define mDigitPyIteratorHeader(clss)                       \
	mDigitIteratorHeader(clss)                       \
	mIMEngine*     py_engine;

DECLARE_CLASS(mDigitPyIterator, mDigitIterator)

#endif //_MGNCSENGINE_DIGITPY

////////////////////////////////////////
// pti engine
#ifdef _MGNCSENGINE_PTI

#define mPtiIteratorClassHeader(clss, super)                  \
	mIMIteratorClassHeader(clss, super)

#define mPtiIteratorHeader(clss)                              \
	mIMIteratorHeader(clss)                               \
	int          cur;                                     \
	const char** words;                                   \
	int          count;                                   \
	int          begin;                                   \
    char upper_buffer[128];                             \
    int char_type;                                      \
	const char*  lead_letter;

DECLARE_CLASS(mPtiIterator, mIMIterator)

#define mPtiEngineClassHeader(clss, super)                    \
	mIMEngineClassHeader(clss, super)

#define MGPTI_2ND_HASH_TABLE_SIZE   100

enum {
    MGPTI_LOWER = 100,
    MGPTI_UPPER
};

typedef struct _MGPTI_2ND_HASH_BUCKET {
	int nr_words;
	int* word_indices;
} MGPTI_2ND_HASH_BUCKET;

typedef struct _MGPTI_1ST_HASH_BUCKET {
	int indices [26];
	MGPTI_2ND_HASH_BUCKET* buckets;
} MGPTI_1ST_HASH_BUCKET;


#define mPtiEngineHeader(clss)                                \
	mIMEngineHeader(clss)                                 \
	char  **sorted_words;                                 \
	int     max_index;                                    \
	MGPTI_1ST_HASH_BUCKET *buckets[26];


DECLARE_CLASS(mPtiEngine, mIMEngine)

MGNCS_EXPORT BOOL mPtiEngine_findWordRange(mPtiEngine* self, int* pbegin, int* pend, const char* input, int start, int len);

#endif //_MGNCSENGINE_PTI

///////////////////////////////////////
// pinyin engine
#ifdef _MGNCSENGINE_PINYIN

#define PINYIN_MAGIC_NUMBER          "CCEGB"
#define PINYIN_ENAME_LENGTH          24
#define PINYIN_CNAME_LENGTH          16
#define PINYIN_WORD_LENGTH           16
#define PINYIN_SELECT_KEY_LENGTH     16
#define PINYIN_KEY_LENGTH            16
#define PINYIN_SEL_LENGTH            58


typedef struct {
	unsigned long key1;
	unsigned long key2;
	unsigned short word_offset_idx;    //pharse_offset table index
	unsigned short frequency;
}NCS_PINYIN_ITEM;

typedef struct {
	unsigned int off_begin; //begin offset
	unsigned int off_end;   //end offset
}NCS_PINYIN_PHOFFSET;

typedef struct {
	char magic_number[sizeof(PINYIN_MAGIC_NUMBER)];
	char ename[PINYIN_ENAME_LENGTH];                  //ascii name
	char cname[PINYIN_CNAME_LENGTH];                  //promt
	char selkey[PINYIN_SELECT_KEY_LENGTH];            //select keys

	char last_full;
	int  totalKey;
	int  maxPress;
	int  maxDupSel;
	int  totalChar;

	unsigned char  keyMap[128];
	unsigned char  keyName[64];
	unsigned short keyIndex[64];

	int    wordNum;
	char*  wordFile;
	char*  assocFile;
	NCS_PINYIN_ITEM * item;
}NCS_PINYIN_IME_TABLE;


#define mPinyinEngineClassHeader(clss, super)                 \
	mIMEngineClassHeader(clss, super)

#define mPinyinEngineHeader(clss)                             \
	mIMEngineHeader(clss)                                 \
	NCS_PINYIN_IME_TABLE * ime_table;

DECLARE_CLASS(mPinyinEngine, mIMEngine)

#define PINYIN_MAX_WORD    64
#define PINYIN_MAX_WORDLEN 64
#define PINYIN_INPUT_MAX     32
typedef struct {
	union {
		char chars[4];
		const char* word;
	}word;
	int         len;
}NCS_PINYIN_WORD_INFO;


#define mPinyinIteratorClassHeader(clss, super)               \
	mIMIteratorClassHeader(clss, super)

#define mPinyinIteratorHeader(clss)                           \
	mIMIteratorHeader(clss)                               \
	int           count;                                  \
	int           cur;                                    \
	unsigned int charIndex[PINYIN_INPUT_MAX];             \
	unsigned int  startKey;                               \
	unsigned int  endKey;                                 \
	unsigned long matched_key1;                           \
	unsigned long matched_key2;                           \
        NCS_PINYIN_WORD_INFO words[PINYIN_MAX_WORD];          \
	char          word[PINYIN_MAX_WORDLEN];               \
	BOOL          isAssociate;

DECLARE_CLASS(mPinyinIterator, mIMIterator)

#if MGUI_BYTEORDER == MGUI_BIG_ENDIAN
#define ARCH_SWAP16(v16)  ArchSwap16(v16)
#define ARCH_SWAP32(v32)  ArchSwap32(v32)
#else
#define ARCH_SWAP16(v16)  (v16)
#define ARCH_SWAP32(v32)  (v32)
#endif

#ifdef __cplusplus
}
#endif

#endif //_MGNCSENGINE_PINYIN
#endif


