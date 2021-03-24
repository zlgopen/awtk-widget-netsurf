
#include <stdint.h>
#include <limits.h>
#include <getopt.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <nsutils/time.h>

#include <awtk.h>

#include "utils/utils.h"
#include "utils/nsoption.h"
#include "utils/filepath.h"
#include "utils/log.h"
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
#include "adapter/awtk_window.h"

static struct gui_window* gui_window_create(struct browser_window* browser_win,
                                            struct gui_window* existing,
                                            gui_window_create_flags flags) {
  gui_window_t* gw = TKMEM_ZALLOC(gui_window_t);
  return_value_if_fail(gw != NULL, NULL);

  gw->browser_win = browser_win;

  return gw;
}

static void gui_window_destroy(struct gui_window* gw) {
  TKMEM_FREE(gw);
}

static nserror awtk_window_invalidate_area(struct gui_window* gw, const struct rect* rect) {
  widget_invalidate_force(gw->web_view, NULL);
  return NSERROR_OK;
}

static bool gui_window_get_scroll(struct gui_window* g, int* sx, int* sy) {
  return true;
}

static nserror gui_window_set_scroll(struct gui_window* gw, const struct rect* rect) {
  return NSERROR_OK;
}

static nserror gui_window_get_dimensions(struct gui_window* gw, int* width, int* height) {
  *width = gw->web_view->w;
  *height = gw->web_view->h;

  return NSERROR_OK;
}

static void gui_window_set_status(struct gui_window* g, const char* text) {
  widget_t* win = widget_get_window(g->web_view);
  widget_t* status = widget_lookup(win, "status", TRUE);

  if(status != NULL) {
    widget_set_tr_text(status, text);
  }

  log_debug("status:%s\n", text);
}

static void gui_window_set_pointer(struct gui_window* g, gui_pointer_shape shape) {
  switch (shape) {
    case GUI_POINTER_POINT:
      break;

    case GUI_POINTER_CARET:
      break;

    case GUI_POINTER_MENU:
      break;

    case GUI_POINTER_PROGRESS:
      break;

    case GUI_POINTER_MOVE:
      break;

    default:
      break;
  }
}

static nserror gui_window_set_url(struct gui_window* g, nsurl* url) {
  return NSERROR_OK;
}

static void gui_window_start_throbber(struct gui_window* g) {
  log_debug("gui_window_start_throbber\n");
}

static void gui_window_stop_throbber(struct gui_window* gw) {
  log_debug("gui_window_stop_throbber\n");
}

static void gui_window_place_caret(struct gui_window* g, int x, int y, int height,
                                   const struct rect* clip) {
  log_debug("gui_window_place_caret\n");
}

static nserror gui_window_event(struct gui_window* gw, enum gui_window_event event) {
  switch (event) {
    case GW_EVENT_UPDATE_EXTENT:
      break;

    case GW_EVENT_REMOVE_CARET:
      break;

    case GW_EVENT_START_THROBBER:
      gui_window_start_throbber(gw);
      break;

    case GW_EVENT_STOP_THROBBER:
      gui_window_stop_throbber(gw);
      break;

    default:
      break;
  }
  return NSERROR_OK;
}

static void gui_awtk_console_log(struct gui_window* gw, browser_window_console_source src,
                                 const char* msg, size_t msglen,
                                 browser_window_console_flags flags) {
  log_debug("%s\n", msg);
}

static struct gui_window_table awtk_window_table = {.create = gui_window_create,
                                                    .destroy = gui_window_destroy,
                                                    .invalidate = awtk_window_invalidate_area,
                                                    .get_scroll = gui_window_get_scroll,
                                                    .set_scroll = gui_window_set_scroll,
                                                    .get_dimensions = gui_window_get_dimensions,
                                                    .event = gui_window_event,
                                                    .set_url = gui_window_set_url,
                                                    .set_status = gui_window_set_status,
                                                    .set_pointer = gui_window_set_pointer,
                                                    .place_caret = gui_window_place_caret,
                                                    .console_log = gui_awtk_console_log};

struct gui_window_table* awtk_get_window_table(void) {
  return &awtk_window_table;
}
