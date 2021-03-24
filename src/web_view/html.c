/**
 * File:   html.c
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
#include "tkc/path.h"
#include "tkc/utils.h"
#include "tkc/data_reader.h"
#include "adapter/awtk_window.h"
#include "widget_animators/widget_animator_scroll.h"

#include "adapter/awtk_plot.h"
#include "adapter/awtk_window.h"

#include "desktop/browser_private.h"

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
#include "netsurf/netsurf.h"
#include "netsurf/cookie_db.h"
#include "content/fetch.h"
#include "content/hlcache.h"
#include "content/content.h"
#include "netsurf/content.h"

#include "html.h"

static bool_t html_is_scollable(widget_t* widget);
static int32_t html_get_virtual_height(widget_t* widget);

typedef struct _html_impl_t {
  struct browser_window* browser_win;
} html_impl_t;

static html_impl_t* html_impl_create(void) {
  html_impl_t* impl = TKMEM_ZALLOC(html_impl_t);

  return impl;
}

static ret_t html_impl_destroy(html_impl_t* impl) {
  TKMEM_FREE(impl);
  return RET_OK;
}

static uint32_t html_get_margin(widget_t* widget) {
  if (widget->astyle != NULL) {
    return style_get_int(widget->astyle, STYLE_ID_MARGIN, 0);
  } else {
    return 0;
  }
}

static uint32_t html_get_row_height(widget_t* widget) {
  return 40;
}

ret_t html_load(widget_t* widget) {
  html_impl_t* impl = NULL;
  html_t* html = HTML(widget);
  return_value_if_fail(html != NULL && html->impl != NULL, RET_FAIL);
  impl = html->impl;

  if (impl->browser_win != NULL && html->url != NULL && html->url[0]) {
    nsurl* url = NULL;
    nserror error = NSERROR_OK;
    struct browser_window* bw = impl->browser_win;

    error = nsurl_create(html->url, &url);
    if (error != NSERROR_OK) {
      log_warn("create url failed\n");
    } else {
      browser_window_navigate(bw, url, NULL, BW_NAVIGATE_HISTORY, NULL, NULL, NULL);
      nsurl_unref(url);
    }
  }

  return RET_OK;

  return RET_OK;
}

static ret_t html_on_size_changed(widget_t* widget) {
  html_impl_t* impl = NULL;
  html_t* html = HTML(widget);
  return_value_if_fail(html != NULL && html->impl != NULL, RET_FAIL);
  impl = html->impl;

  if (impl->browser_win != NULL && impl->browser_win->window != NULL) {
    //int32_t w = widget->w - 2 * html_get_margin(widget);
    //int32_t h = widget->h - 2 * html_get_margin(widget);
    //browser_window_set_dimensions(impl->browser_win, w, h);
  }

  return RET_OK;
}

ret_t html_set_url(widget_t* widget, const char* url) {
  html_t* html = HTML(widget);
  return_value_if_fail(html != NULL, RET_BAD_PARAMS);

  html->url = tk_str_copy(html->url, url);

  return RET_OK;
}

static ret_t html_get_prop(widget_t* widget, const char* name, value_t* v) {
  html_t* html = HTML(widget);
  html_impl_t* impl = html->impl;
  return_value_if_fail(html != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(HTML_PROP_URL, name)) {
    value_set_str(v, html->url);
    return RET_OK;
  } else if (tk_str_eq(WIDGET_PROP_YOFFSET, name)) {
    value_set_int32(v, html->yoffset);
    return RET_OK;
  } else if (tk_str_eq(WIDGET_PROP_XOFFSET, name)) {
    value_set_int32(v, 0);
    return RET_OK;
  } else if (tk_str_eq(WIDGET_PROP_VIRTUAL_W, name)) {
    value_set_int32(v, widget->w);
    return RET_OK;
  } else if (tk_str_eq(WIDGET_PROP_VIRTUAL_H, name)) {
    int32_t virtual_h = widget->h;
    if (impl->browser_win != NULL) {
      virtual_h = html_get_virtual_height(widget);
    }
    value_set_int32(v, virtual_h);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_YSLIDABLE)) {
    value_set_bool(v, html->yslidable);
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static ret_t idle_load_page(const idle_info_t* info) {
  widget_t* widget = WIDGET(info->ctx);

  html_load(widget);

  return RET_REMOVE;
}

static ret_t html_set_prop(widget_t* widget, const char* name, const value_t* v) {
  html_t* html = HTML(widget);
  return_value_if_fail(widget != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(HTML_PROP_URL, name)) {
    html_set_url(widget, value_str(v));
    if (widget_is_window_opened(widget)) {
      widget_add_idle(widget, idle_load_page);
    }
    return RET_OK;
  } else if (tk_str_eq(WIDGET_PROP_TEXT, name)) {
    wstr_from_value(&(widget->text), v);
    if (widget_is_window_opened(widget)) {
      widget_add_idle(widget, idle_load_page);
    }
  } else if (tk_str_eq(WIDGET_PROP_YOFFSET, name)) {
    html->yoffset = value_int32(v);
    return RET_OK;
  } else if (tk_str_eq(WIDGET_PROP_XOFFSET, name)) {
    return RET_OK;
  } else if (tk_str_eq(WIDGET_PROP_VIRTUAL_W, name)) {
    return RET_OK;
  } else if (tk_str_eq(WIDGET_PROP_VIRTUAL_H, name)) {
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_YSLIDABLE)) {
    html->yslidable = value_bool(v);
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static ret_t html_on_destroy(widget_t* widget) {
  html_t* html = HTML(widget);
  return_value_if_fail(widget != NULL && html != NULL, RET_BAD_PARAMS);

  html_impl_destroy(html->impl);
  TKMEM_FREE(html->url);

  return RET_OK;
}

static ret_t html_on_paint_self(widget_t* widget, canvas_t* c) {
  rect_t r;
  rect_t r_save;
  html_t* html = HTML(widget);
  html_impl_t* impl = html->impl;
  int32_t yoffset = html->yoffset;
  //uint64_t start = time_now_us();

  r = rect_init(c->ox, c->oy, widget->w, widget->h);
  if (widget->astyle != NULL) {
    int32_t margin = style_get_int(widget->astyle, STYLE_ID_MARGIN, 0);

    r.x += margin;
    r.y += margin;
    r.w -= margin << 1;
    r.h -= margin << 1;
  }

  canvas_get_clip_rect(c, &r_save);
  r = rect_intersect(&r, &r_save);
  canvas_set_clip_rect(c, &r);

  if (impl->browser_win != NULL) {
    plotter_info_t info = {c, widget};

    struct redraw_context ctx = {
        .interactive = true,
        .background_images = true,
    };

    struct rect clip;
    vgcanvas_t* vg = canvas_get_vgcanvas(c);

    clip.x0 = 0;
    clip.y0 = yoffset;
    clip.x1 = widget->w;
    clip.y1 = widget->h + yoffset;

    ctx.priv = &info;
    ctx.plot = awtk_get_plotters_table();
    vgcanvas_save(vg);
    vgcanvas_translate(vg, c->ox, c->oy - yoffset);

    browser_window_redraw(impl->browser_win, 0, 0, &clip, &ctx);

    vgcanvas_translate(vg, -c->ox, -c->oy);
    vgcanvas_restore(vg);
  }
  canvas_set_clip_rect(c, &r_save);
  //log_debug("html paint cost: %d us\n", (int)(time_now_us()-start));

  return RET_OK;
}

static ret_t html_on_scroll_done(void* ctx, event_t* e) {
  widget_t* widget = WIDGET(ctx);
  html_t* html = HTML(ctx);
  return_value_if_fail(widget != NULL && html != NULL, RET_BAD_PARAMS);

  html->wa = NULL;

  return RET_REMOVE;
}

ret_t html_scroll_to(widget_t* widget, int32_t yoffset_end) {
  int32_t yoffset = 0;
  int32_t virtual_h = 0;
  html_t* html = HTML(widget);
  return_value_if_fail(html != NULL, RET_FAIL);
  virtual_h = widget_get_prop_int(widget, WIDGET_PROP_VIRTUAL_H, 0);

  if (!html_is_scollable(widget)) {
    html->yoffset = 0;
    return RET_OK;
  }

  if (yoffset_end < 0) {
    yoffset_end = 0;
  }

  if ((yoffset_end + widget->h) > virtual_h) {
    yoffset_end = virtual_h - widget->h;
  }

  if (yoffset_end == html->yoffset) {
    return RET_OK;
  }

  yoffset = html->yoffset;
  if (html->wa != NULL) {
    widget_animator_scroll_t* wa = (widget_animator_scroll_t*)html->wa;
    if (yoffset_end != html->yoffset) {
      bool_t changed = wa->y_to != yoffset_end;
      bool_t in_range = wa->y_to >= 0 && wa->y_to < (virtual_h - widget->h);
      if (changed && in_range) {
        wa->y_to = yoffset_end;
        wa->y_from = html->yoffset;
        wa->base.now = 0;
        wa->base.start_time = 0;
      }
    }
  } else {
    html->wa = widget_animator_scroll_create(widget, TK_ANIMATING_TIME, 0, EASING_SIN_INOUT);
    return_value_if_fail(html->wa != NULL, RET_OOM);
    widget_animator_scroll_set_params(html->wa, 0, yoffset, 0, yoffset_end);
    widget_animator_on(html->wa, EVT_ANIM_END, html_on_scroll_done, html);
    widget_animator_start(html->wa);
  }

  return RET_OK;
}

ret_t html_scroll_delta_to(widget_t* widget, int32_t yoffset_delta) {
  html_t* html = HTML(widget);
  return_value_if_fail(html != NULL, RET_FAIL);

  html->yoffset_end = html->yoffset + yoffset_delta;

  return html_scroll_to(widget, html->yoffset_end);
}

static ret_t html_on_pointer_down(html_t* html, pointer_event_t* e) {
  velocity_t* v = &(html->velocity);
  return_value_if_fail(v != NULL, RET_BAD_PARAMS);

  velocity_reset(v);
  html->ydown = e->y;
  html->yoffset_save = html->yoffset;

  velocity_update(v, e->e.time, e->x, e->y);

  return RET_OK;
}

static ret_t html_on_pointer_move(html_t* html, pointer_event_t* e) {
  int32_t dy = 0;
  velocity_t* v = NULL;
  return_value_if_fail(html != NULL, RET_BAD_PARAMS);
  v = &(html->velocity);
  dy = e->y - html->ydown;

  velocity_update(v, e->e.time, e->x, e->y);

  if (html->wa == NULL && dy) {
    int32_t yoffset = html->yoffset_save - dy;
    widget_set_prop_int(WIDGET(html), WIDGET_PROP_YOFFSET, yoffset);
  }

  return RET_OK;
}

static ret_t html_on_pointer_up(html_t* html, pointer_event_t* e) {
  velocity_t* v = NULL;
  int32_t yoffset_end = 0;
  widget_t* widget = WIDGET(html);
  return_value_if_fail(html != NULL && widget != NULL, RET_BAD_PARAMS);
  v = &(html->velocity);

  velocity_update(v, e->e.time, e->x, e->y);
  yoffset_end = html->yoffset - v->yv;

  html_scroll_to(widget, yoffset_end);

  return RET_OK;
}

static ret_t html_down(widget_t* widget) {
  uint32_t row_height = html_get_row_height(widget);
  return html_scroll_delta_to(widget, row_height);
}

static ret_t html_up(widget_t* widget) {
  uint32_t row_height = html_get_row_height(widget);
  return html_scroll_delta_to(widget, -row_height);
}

static ret_t html_pagedown(widget_t* widget) {
  int32_t h = 0;
  int32_t pageh = 0;
  html_t* html = HTML(widget);
  int32_t row_height = html_get_row_height(widget);
  return_value_if_fail(html != NULL && widget != NULL, RET_BAD_PARAMS);

  h = widget->h - html_get_margin(widget) * 2 - 30;
  pageh = tk_max(h, row_height);

  return html_scroll_delta_to(widget, pageh);
}

static ret_t html_pageup(widget_t* widget) {
  int32_t h = 0;
  int32_t pageh = 0;
  html_t* html = HTML(widget);
  int32_t row_height = html_get_row_height(widget);
  return_value_if_fail(html != NULL && widget != NULL, RET_BAD_PARAMS);

  h = widget->h - html_get_margin(widget) * 2 - 30;
  pageh = tk_max(h, row_height);

  return html_scroll_delta_to(widget, -pageh);
}

static ret_t html_on_key_down(widget_t* widget, key_event_t* evt) {
  ret_t ret = RET_OK;
  return_value_if_fail(evt != NULL, RET_BAD_PARAMS);

  if (evt->key == TK_KEY_PAGEDOWN) {
    html_pagedown(widget);
    ret = RET_STOP;
  } else if (evt->key == TK_KEY_PAGEUP) {
    html_pageup(widget);
    ret = RET_STOP;
  } else if (evt->key == TK_KEY_UP) {
    html_up(widget);
    ret = RET_STOP;
  } else if (evt->key == TK_KEY_DOWN) {
    html_down(widget);
    ret = RET_STOP;
  }

  return ret;
}

static int32_t html_get_virtual_height(widget_t* widget) {
  html_t* html = HTML(widget);
  html_impl_t* impl = html->impl;
  if (impl->browser_win != NULL && impl->browser_win->current_content != NULL) {
    int32_t height = content_get_height(impl->browser_win->current_content);
    return tk_max(widget->h, height);
  } else {
    int32_t margin = html_get_margin(widget);

    return widget->h - 2 * margin;
  }
}

static bool_t html_is_scollable(widget_t* widget) {
  int32_t height = html_get_virtual_height(widget);
  int32_t margin = html_get_margin(widget);

  return (height + 2 * margin) > (int32_t)(widget->h);
}

static ret_t html_on_event(widget_t* widget, event_t* e) {
  ret_t ret = RET_OK;
  html_t* html = HTML(widget);
  html_impl_t* impl = html->impl;
  bool_t scrollable = html_is_scollable(widget);
  return_value_if_fail(widget != NULL && html != NULL, RET_BAD_PARAMS);

  switch (e->type) {
    case EVT_WINDOW_OPEN: {
      html_load(widget);
      break;
    }
    case EVT_RESIZE:
    case EVT_MOVE_RESIZE: {
      html_on_size_changed(widget);
      break;
    }
    case EVT_KEY_DOWN: {
      if (scrollable) {
        ret = html_on_key_down(widget, (key_event_t*)e);
      }
      break;
    }
    case EVT_POINTER_LEAVE: {
      if (impl->browser_win != NULL) {
      }
      break;
    }
    case EVT_POINTER_DOWN: {
      html->pressed = TRUE;
      if (scrollable && html->yslidable) {
        widget_grab(widget->parent, widget);
        html_on_pointer_down(html, (pointer_event_t*)e);
      }
      break;
    }
    case EVT_POINTER_MOVE: {
      if (scrollable && html->yslidable) {
        pointer_event_t* evt = (pointer_event_t*)e;
        if (evt->pressed && html->pressed) {
          html_on_pointer_move(html, evt);
          widget_invalidate(widget, NULL);
          ret = RET_STOP;
        }
      }
      break;
    }
    case EVT_POINTER_UP: {
      html->pressed = FALSE;
      if (scrollable && html->yslidable) {
        html_on_pointer_up(html, (pointer_event_t*)e);
        widget_ungrab(widget->parent, widget);
        ret = RET_STOP;
      }

      if (impl->browser_win != NULL && html->wa == NULL) {
        pointer_event_t* evt = (pointer_event_t*)e;
        point_t p = {evt->x, evt->y};
        widget_to_local(widget, &p);

        p.y += html->yoffset;
        browser_window_mouse_click(impl->browser_win, BROWSER_MOUSE_CLICK_1, p.x, p.y);
      }

      break;
    }
    case EVT_WHEEL: {
      if (scrollable) {
        wheel_event_t* evt = (wheel_event_t*)e;
        if (evt->dy > 0) {
          html_down(widget);
        } else if (evt->dy < 0) {
          html_up(widget);
        }

        ret = RET_STOP;
      }
      break;
    }
    default:
      break;
  }

  return ret;
}

const char* s_html_properties[] = {HTML_PROP_URL, NULL};

TK_DECL_VTABLE(html) = {.size = sizeof(html_t),
                        .type = WIDGET_TYPE_HTML,
                        .clone_properties = s_html_properties,
                        .persistent_properties = s_html_properties,
                        .parent = TK_PARENT_VTABLE(widget),
                        .create = html_create,
                        .get_prop = html_get_prop,
                        .set_prop = html_set_prop,
                        .on_paint_self = html_on_paint_self,
                        .on_event = html_on_event,
                        .on_destroy = html_on_destroy};

widget_t* html_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  nserror ret = NSERROR_OK;
  gui_window_t* gw = NULL;
  struct browser_window* browser_win = NULL;
  widget_t* widget = widget_create(parent, TK_REF_VTABLE(html), x, y, w, h);
  html_t* html = HTML(widget);

  return_value_if_fail(html != NULL, NULL);

  html->yslidable = TRUE;
  html->impl = html_impl_create();

  if (ret == NSERROR_OK) {
    ret = browser_window_create(BW_CREATE_HISTORY, NULL, NULL, NULL, &browser_win);
  }

  html->impl->browser_win = browser_win;
  gw = (gui_window_t*)(browser_win->window);
  gw->web_view = widget;

  return widget;
}

ret_t html_set_yslidable(widget_t* widget, bool_t yslidable) {
  html_t* html = HTML(widget);
  return_value_if_fail(html != NULL, RET_FAIL);

  html->yslidable = yslidable;

  return RET_OK;
}

widget_t* html_cast(widget_t* widget) {
  return_value_if_fail(WIDGET_IS_INSTANCE_OF(widget, html), NULL);

  return widget;
}

ret_t html_back(widget_t* widget) {
  html_impl_t* impl = NULL;
  html_t* html = HTML(widget);
  return_value_if_fail(html != NULL && html->impl != NULL, RET_FAIL);
  impl = html->impl;

  if (impl->browser_win != NULL) {
    if (browser_window_back_available(impl->browser_win)) {
      browser_window_history_back(impl->browser_win, false);
    }
  }

  return RET_OK;
}

ret_t html_refresh(widget_t* widget) {
  html_impl_t* impl = NULL;
  html_t* html = HTML(widget);
  return_value_if_fail(html != NULL && html->impl != NULL, RET_FAIL);
  impl = html->impl;

  if (impl->browser_win != NULL) {
    browser_window_refresh_url_bar(impl->browser_win);
  }

  return RET_OK;
}

ret_t html_forward(widget_t* widget) {
  html_impl_t* impl = NULL;
  html_t* html = HTML(widget);
  return_value_if_fail(html != NULL && html->impl != NULL, RET_FAIL);
  impl = html->impl;

  if (impl->browser_win != NULL) {
    if (browser_window_forward_available(impl->browser_win)) {
      browser_window_history_forward(impl->browser_win, false);
    }
  }

  return RET_OK;
}

ret_t html_stop(widget_t* widget) {
  html_impl_t* impl = NULL;
  html_t* html = HTML(widget);
  return_value_if_fail(html != NULL && html->impl != NULL, RET_FAIL);
  impl = html->impl;

  if (impl->browser_win != NULL) {
    if (browser_window_stop_available(impl->browser_win)) {
      browser_window_stop(impl->browser_win);
    }
  }

  return RET_OK;
}

bool_t html_back_available(widget_t* widget) {
  html_impl_t* impl = NULL;
  html_t* html = HTML(widget);
  return_value_if_fail(html != NULL && html->impl != NULL, FALSE);
  impl = html->impl;

  if (impl->browser_win != NULL) {
    return (browser_window_back_available(impl->browser_win));
  }

  return FALSE;
}

bool_t html_forward_available(widget_t* widget) {
  html_impl_t* impl = NULL;
  html_t* html = HTML(widget);
  return_value_if_fail(html != NULL && html->impl != NULL, FALSE);
  impl = html->impl;

  if (impl->browser_win != NULL) {
    return (browser_window_forward_available(impl->browser_win));
  }

  return FALSE;
}

bool_t html_stop_available(widget_t* widget) {
  html_impl_t* impl = NULL;
  html_t* html = HTML(widget);
  return_value_if_fail(html != NULL && html->impl != NULL, FALSE);
  impl = html->impl;

  if (impl->browser_win != NULL) {
    return (browser_window_stop_available(impl->browser_win));
  }

  return FALSE;
}
