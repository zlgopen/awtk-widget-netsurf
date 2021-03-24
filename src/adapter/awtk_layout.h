/**
 * File:   schedule.h
 * Author: AWTK Develop Team
 * Brief:  schedule 
 *
 * Copyright (c) 2021 - 2021  Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * License file for more details.
 *
 */

/**
 * History:
 * ================================================================
 * 2021-03-18 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_FONT_LAYOUT_H
#define TK_FONT_LAYOUT_H

#include "tkc/types_def.h"

#include "utils/utils.h"
#include "utils/nsoption.h"
#include "utils/filepath.h"
#include "utils/messages.h"
#include "netsurf/browser_window.h"
#include "netsurf/keypress.h"
#include "desktop/browser_history.h"
#include "netsurf/plotters.h"
#include "netsurf/window.h"
#include "netsurf/misc.h"
#include "netsurf/utf8.h"
#include "netsurf/layout.h"

BEGIN_C_DECLS

struct gui_layout_table* awtk_get_font_layout_table(void);

END_C_DECLS

#endif /*TK_FONT_LAYOUT_H*/
