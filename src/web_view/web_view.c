/**
 * File:   web_view.h
 * Author: AWTK Develop Team
 * Brief:  web_view
 *
 * Copyright (c) 2018 - 2021  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2021-03-14 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/utils.h"
#include "scroll_view/scroll_bar.h"

#include "web_view/html.h"
#include "web_view/web_view.h"

static ret_t web_view_sync_scroll_bar_to_html(widget_t* widget) {
  int32_t yoffset = 0;
  html_t* html = NULL;
  web_view_t* web_view = WEB_VIEW(widget);
  return_value_if_fail(web_view != NULL, RET_BAD_PARAMS);
  html = HTML(web_view->html);
  yoffset = widget_get_value(web_view->scroll_bar);

  if (html != NULL) {
    emitter_disable(web_view->html->emitter);
    widget_set_prop_int(web_view->html, WIDGET_PROP_YOFFSET, yoffset);
    emitter_enable(web_view->html->emitter);
  }

  return RET_OK;
}

static ret_t web_view_on_scroll_bar_changed(void* ctx, event_t* e) {
  web_view_sync_scroll_bar_to_html(WIDGET(ctx));
  return RET_OK;
}

static ret_t web_view_sync_html_to_scroll_bar(widget_t* widget) {
  int32_t h = 0;
  int32_t max = 0;
  int32_t value = 0;
  int32_t yoffset = 0;
  int32_t virtual_h = 0;
  web_view_t* web_view = WEB_VIEW(widget);
  return_value_if_fail(web_view != NULL, RET_BAD_PARAMS);

  yoffset = widget_get_prop_int(web_view->html, WIDGET_PROP_YOFFSET, 0);
  virtual_h = widget_get_prop_int(web_view->html, WIDGET_PROP_VIRTUAL_H, 0);
  h = web_view->html->h;
  max = tk_max(virtual_h, h);

  if (max > h) {
    value = (yoffset * max) / (max - h);
  }

  if (web_view->scroll_bar != NULL) {
    emitter_disable(web_view->scroll_bar->emitter);
    widget_set_prop_int(web_view->scroll_bar, WIDGET_PROP_MAX, max);
    widget_set_prop_int(web_view->scroll_bar, WIDGET_PROP_VALUE, value);
    emitter_enable(web_view->scroll_bar->emitter);
  }

  return RET_OK;
}

static ret_t web_view_on_html_scrolled(void* ctx, event_t* e) {
  prop_change_event_t* evt = (prop_change_event_t*)e;
  return_value_if_fail(evt != NULL, RET_BAD_PARAMS);
  if (tk_str_eq(evt->name, WIDGET_PROP_YOFFSET)) {
    web_view_sync_html_to_scroll_bar(WIDGET(ctx));
  }

  return RET_OK;
}

#define STR_WIDGET_GO "go"
#define STR_WIDGET_WEB "web"
#define STR_WIDGET_URL "url"
#define STR_WIDGET_BACK "back"
#define STR_WIDGET_FORWARD "forward"
#define STR_WIDGET_REFRESH_STOP "refresh_stop"

static ret_t web_view_on_update_timer(const timer_info_t* info) {
  widget_t* view = WIDGET(info->ctx);
  widget_t* web = widget_lookup(view, STR_WIDGET_WEB, TRUE);
  widget_t* back = widget_lookup(view, STR_WIDGET_BACK, TRUE);
  widget_t* forward = widget_lookup(view, STR_WIDGET_FORWARD, TRUE);
  widget_t* refresh_stop = widget_lookup(view, STR_WIDGET_REFRESH_STOP, TRUE);

  if (back != NULL) {
    widget_set_enable(back, html_back_available(web));
  }

  if (forward != NULL) {
    widget_set_enable(forward, html_forward_available(web));
  }

  if (refresh_stop != NULL) {
    if (html_stop_available(web)) {
      widget_set_text(refresh_stop, L"X");
    } else {
      widget_set_text(refresh_stop, L"R");
    }
  }
  web_view_sync_html_to_scroll_bar(view);

  return RET_REPEAT;
}

static ret_t web_view_on_go(void* ctx, event_t* e) {
  char surl[MAX_PATH + 1];
  widget_t* view = WIDGET(ctx);
  widget_t* url = widget_lookup(view, STR_WIDGET_URL, TRUE);
  widget_t* web = widget_lookup(view, STR_WIDGET_WEB, TRUE);

  widget_get_text_utf8(url, surl, sizeof(surl));
  widget_set_prop_str(web, STR_WIDGET_URL, surl);

  return RET_OK;
}

static ret_t web_view_on_forward(void* ctx, event_t* e) {
  widget_t* view = WIDGET(ctx);
  widget_t* web = widget_lookup(view, STR_WIDGET_WEB, TRUE);
  html_forward(web);

  return RET_OK;
}

static ret_t web_view_on_back(void* ctx, event_t* e) {
  widget_t* view = WIDGET(ctx);
  widget_t* web = widget_lookup(view, STR_WIDGET_WEB, TRUE);

  html_back(web);
  return RET_OK;
}

static ret_t web_view_on_refresh_stop(void* ctx, event_t* e) {
  widget_t* view = WIDGET(ctx);
  widget_t* web = widget_lookup(view, STR_WIDGET_WEB, TRUE);

  if (html_stop_available(web)) {
    html_stop(web);
  } else {
    html_refresh(web);
  }
  return RET_OK;
}

static ret_t web_view_on_add_child(widget_t* widget, widget_t* child) {
  web_view_t* web_view = WEB_VIEW(widget);
  return_value_if_fail(web_view != NULL, RET_BAD_PARAMS);
  if (tk_str_eq(widget_get_type(child), WIDGET_TYPE_HTML)) {
    web_view->html = child;
    widget_on(child, EVT_PROP_CHANGED, web_view_on_html_scrolled, widget);
  } else if (tk_str_eq(widget_get_type(child), WIDGET_TYPE_SCROLL_BAR) ||
             tk_str_eq(widget_get_type(child), WIDGET_TYPE_SCROLL_BAR_DESKTOP) ||
             tk_str_eq(widget_get_type(child), WIDGET_TYPE_SCROLL_BAR_MOBILE)) {
    web_view->scroll_bar = child;
    widget_on(child, EVT_VALUE_CHANGED, web_view_on_scroll_bar_changed, widget);
  }

  return RET_FAIL;
}

static ret_t web_view_on_remove_child(widget_t* widget, widget_t* child) {
  web_view_t* web_view = WEB_VIEW(widget);
  return_value_if_fail(web_view != NULL, RET_BAD_PARAMS);

  widget_off_by_ctx(child, widget);
  if (web_view->html == child) {
    web_view->html = NULL;
  }

  if (web_view->scroll_bar == child) {
    web_view->scroll_bar = NULL;
  }

  return RET_FAIL;
}

static ret_t web_view_on_event(widget_t* widget, event_t* e) {
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  if (e->type == EVT_WINDOW_OPEN) {
    widget_child_on(widget, STR_WIDGET_GO, EVT_CLICK, web_view_on_go, widget);
    widget_child_on(widget, STR_WIDGET_FORWARD, EVT_CLICK, web_view_on_forward, widget);
    widget_child_on(widget, STR_WIDGET_BACK, EVT_CLICK, web_view_on_back, widget);
    widget_child_on(widget, STR_WIDGET_REFRESH_STOP, EVT_CLICK, web_view_on_refresh_stop, widget);

    widget_add_timer(widget, web_view_on_update_timer, 1000);
  }

  return RET_OK;
}

TK_DECL_VTABLE(web_view) = {
    .size = sizeof(web_view_t),
    .type = WIDGET_TYPE_WEB_VIEW,
    .parent = TK_PARENT_VTABLE(widget),
    .on_add_child = web_view_on_add_child,
    .on_remove_child = web_view_on_remove_child,
    .create = web_view_create,
    .on_event = web_view_on_event,
};

widget_t* web_view_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  return widget_create(parent, TK_REF_VTABLE(web_view), x, y, w, h);
}

widget_t* web_view_cast(widget_t* widget) {
  return_value_if_fail(WIDGET_IS_INSTANCE_OF(widget, web_view), NULL);

  return widget;
}
