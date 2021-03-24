/**
 * File:   awtk_plot.h
 * Author: AWTK Develop Team
 * Brief:  plot 
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

#ifndef TK_AWTK_PLOT_H
#define TK_AWTK_PLOT_H

#include "awtk.h"

#include "utils/utils.h"
#include "utils/nsoption.h"
#include "utils/filepath.h"
#include "utils/messages.h"
#include "netsurf/browser_window.h"
#include "netsurf/keypress.h"
#include "desktop/browser_history.h"
#include "netsurf/plotters.h"
#include "netsurf/window.h"
#include "netsurf/plotters.h"

BEGIN_C_DECLS

typedef struct _plotter_info_t {
  canvas_t* c;
  widget_t* widget;
} plotter_info_t;

const struct plotter_table* awtk_get_plotters_table(void);

END_C_DECLS

#endif /*TK_AWTK_PLOT_H*/
