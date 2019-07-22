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

#ifndef MEM_SLAB_H
#define MEM_SLAB_H

#ifdef __cplusplus
extern "C" {
#endif

MGNCS_EXPORT void* slab_new_heap(int min_size, int max_size);

MGNCS_EXPORT void slab_free_heap(void *heap);

MGNCS_EXPORT void* slab_new_mem(void *heap, int size, int padding);

MGNCS_EXPORT void slab_free_mem(void *heap, void *p);

MGNCS_EXPORT int slab_init_default_heap(void);

MGNCS_EXPORT void slab_unit_default_heap(void);

#define _SLAB_ALLOC(size)  slab_new_mem(NULL, (size), 0)
#define _SLAB_FREE(p)   slab_free_mem(NULL, p)

#ifdef __cplusplus
}
#endif

#endif


