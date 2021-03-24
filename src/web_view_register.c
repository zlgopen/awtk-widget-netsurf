/**
 * File:   web_view.c
 * Author: AWTK Develop Team
 * Brief:  Simple HTML View
 *
 * Copyright (c) 2021 - 2021 Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2021-03-09 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/utils.h"
#include "web_view_register.h"
#include "base/widget_factory.h"
#include "netsurf/netsurf.h"

#include "adapter/awtk_bitmap.h"
#include "adapter/awtk_fetch.h"
#include "adapter/awtk_layout.h"
#include "adapter/awtk_schedule.h"
#include "adapter/awtk_clipboard.h"
#include "adapter/awtk_plot.h"
#include "adapter/awtk_window.h"
#include "adapter/awtk_utf8.h"

#include "desktop/gui_table.h"

#include "web_view/html.h"
#include "web_view/web_view.h"

static struct netsurf_table awtk_table;

ret_t web_view_register(void) {
  awtk_table.misc = awtk_get_misc_table();
  awtk_table.window = awtk_get_window_table();
  awtk_table.clipboard = awtk_get_clipboard_table();
  awtk_table.fetch = awtk_get_fetch_table();
  awtk_table.utf8 = awtk_get_utf8_table();
  awtk_table.bitmap = awtk_get_bitmap_table();
  awtk_table.layout = awtk_get_font_layout_table();

  if (netsurf_register(&awtk_table) != NSERROR_OK) {
    log_debug("netsurf_register failed\n");
  }

  widget_factory_register(widget_factory(), WIDGET_TYPE_HTML, html_create);
  return widget_factory_register(widget_factory(), WIDGET_TYPE_WEB_VIEW, web_view_create);
}

const char* web_view_supported_render_mode(void) {
  return "OpenGL|AGGE-BGR565|AGGE-BGRA8888|AGGE-MONO";
}
