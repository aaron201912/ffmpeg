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

#ifndef _MFILLBOXEX_H
#define _MFILLBOXEX_H

#ifdef __cplusplus
extern "C" {
#endif

#include <minigui/common.h>
#include <minigui/gdi.h>

typedef void (*cb_fill_span_t)(HDC hdc, void *context, const RECT *rc);
void ncsFillRegion(HDC hdc, PCLIPRGN prgn, cb_fill_span_t cb, void *context);

void ncs_cb_fillspan_simple(HDC hdc, void *ctx_rgba, const RECT *rc);

struct gradient_context{
    int top;
    int bottom;
    unsigned int rgba[2];
};

void ncs_cb_fillspan_gradient(HDC hdc, void *ctx_gradient, const RECT *rc);

typedef unsigned int gradient_t[2];
struct multigradient_context{
    int top;
    int bottom;
    int n;
    gradient_t *colortable;
    int _private_current_index;
};

void ncs_cb_fillspan_multigradient(HDC hdc, void *ctx_multigradient, const RECT *rc);

#ifdef __cplusplus
}
#endif

#endif /* _MFILLBOXEX_H */
