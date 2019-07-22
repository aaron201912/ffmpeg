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

#ifndef _MGNCS_PIECE_H
#define _MGNCS_PIECE_H

#include "manimateframes.h"
#include "mbmparrayanimateframes.h"
#include "mgifanimateframes.h"

#include "pieces/piece-id.h"
#include "pieces/mhotpiece.h"
#include "pieces/mstaticpiece.h"
#include "pieces/mlayoutpiece.h"
#include "pieces/mcontainerpiece.h"
#include "pieces/mrenderablepiece.h"
#include "pieces/mlabelpiece.h"
#include "pieces/mimagepiece.h"
#include "pieces/mabstractbuttonpiece.h"
#include "pieces/mbuttonboxpiece.h"
#include "pieces/mpairpiece.h"
#include "pieces/mpushbuttonpiece.h"
#include "pieces/mimagelabelpiece.h"

#include "pieces/msliderpiece.h"

#include "pieces/mthumbboxpiece.h"

#include "pieces/mscrollthumbboxpiece.h"

#include "pieces/mtrackbarpiece.h"
#include "pieces/mboxlayoutpiece.h"

#include "pieces/mhboxlayoutpiece.h"

#include "pieces/mvboxlayoutpiece.h"
#include "pieces/mcheckboxpiece.h"
#include "pieces/mradioboxpiece.h"
#include "pieces/mcheckbuttonpiece.h"
#include "pieces/marrowpiece.h"
#include "pieces/mleftarrowpiece.h"
#include "pieces/mrightarrowpiece.h"
#include "pieces/muparrowpiece.h"
#include "pieces/mdownarrowpiece.h"

#include "pieces/mscrollbarpiece.h"

#include "pieces/marrowbuttonpiece.h"

#include "pieces/mspinnerpiece.h"

#include "pieces/mspinboxpiece.h"
#include "pieces/mhspinboxpiece.h"
#include "pieces/mvspinboxpiece.h"

#include "pieces/mgridboxpiece.h"
#include "pieces/mdaygridpiece.h"

#include "pieces/mmonthpiece.h"

#include "pieces/mnumspinnedpiece.h"
#include "pieces/mlistspinnedpiece.h"
#include "pieces/mweekheadpiece.h"
#include "pieces/mtoolimgpiece.h"
#include "pieces/mtoolimgitempiece.h"
#include "pieces/mseparatorpiece.h"

#include "pieces/mlinelayoutpiece.h"

#include "pieces/mwidgetwrapperpiece.h"
#include "pieces/mrectpiece.h"

#include "pieces/manimatepiece.h"

#include "pieces/mprogresspiece.h"

#include "pieces/mledstaticpiece.h"

#include "pieces/msimplelistpiece.h"

BOOL init_pieces_classes(void);

#define HP(ths)   ((mHotPiece*)(ths))

//for widget
#define Body      HP(self->body)
#define SetHotPieceProp(piece, id, value)  _c(HP(piece))->setProperty(HP(piece),id, value)
#define GetHotPieceProp(piece, id)         _c(HP(piece))->getProperty(HP(piece),id)
#define SetBodyProp(id, value)  _c(Body)->setProperty(Body, id, value)
#define GetBodyProp(id)  _c(Body)->getProperty(Body, id)


#endif

