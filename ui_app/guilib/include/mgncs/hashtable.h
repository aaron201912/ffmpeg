/**
 * \file hashtable.h
 * \author XiaoweiYan
 * \date 2009/11/10
 *
 * \brief This file includes some functions for hashtable.
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
#ifndef HASHTABLE_H
#define HASHTABLE_H

//typedef unsigned int ht_key_t;

#define INVALID_KEY 0u

#define HTE_OK 0
#define HTE_EXIST 1
#define HTE_NOTEXIST 2
#define HTE_INVALID_KEY 3

typedef unsigned long ht_key_t;

typedef struct _HashEntry{
	ht_key_t key;
	void *data;
	struct _HashEntry *next;
}HashEntry;

typedef void (*freeObj)(void* obj);
typedef struct _HashTable{
	int count;
	HashEntry ** entries;
    freeObj      free_obj;
}HashTable;

MGNCS_EXPORT int hash_insert(HashTable* ht,ht_key_t key, void *data);

MGNCS_EXPORT int hash_delete(HashTable* ht,ht_key_t key);

MGNCS_EXPORT void * hash_get(HashTable* ht,ht_key_t key);

MGNCS_EXPORT HashTable * hash_new(int count, freeObj func);

MGNCS_EXPORT void hash_free(HashTable *ht);

#define HT_FOR_EACH_CONTINUE 0 
#define HT_FOR_EACH_DELETE   0x1
#define HT_FOR_EACH_BREAK    0x02
typedef int(*eachObj)(void*, void*);
MGNCS_EXPORT void hash_for_each(HashTable *ht, eachObj each, void* user);

#endif

