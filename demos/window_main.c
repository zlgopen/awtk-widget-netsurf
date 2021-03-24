#include "awtk.h"
#include "web_view_register.h"

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

static nserror set_defaults(struct nsoption_s* defaults) {
  /* Set defaults for absent option strings */
  nsoption_setnull_charp(cookie_file, strdup("~/.netsurf/Cookies"));
  nsoption_setnull_charp(cookie_jar, strdup("~/.netsurf/Cookies"));

  if (nsoption_charp(cookie_file) == NULL || nsoption_charp(cookie_jar) == NULL) {
    return NSERROR_BAD_PARAMETER;
  }
  nsoption_set_bool(enable_javascript, true);

  return NSERROR_OK;
}

static ret_t on_go(void* ctx, event_t* e) {
  char surl[MAX_PATH + 1];
  widget_t* win = WIDGET(ctx);
  widget_t* url = widget_lookup(win, "url", TRUE);
  widget_t* web = widget_lookup(win, "web", TRUE);

  widget_get_text_utf8(url, surl, sizeof(surl));
  widget_set_prop_str(web, "url", surl);

  return RET_OK;
}

ret_t on_open(void* ctx, event_t* evt) {
  widget_t* win = NULL;
  widget_t* go = NULL;
  const char* name = (const char*)ctx;

  win = window_open(name);
  go = widget_lookup(win, "go", TRUE);
//  widget_on(go, EVT_CLICK, on_go, win);

  return RET_OK;
}

ret_t application_init(void) {
  widget_t* win = NULL;

  web_view_register();
  nsoption_init(set_defaults, &nsoptions, &nsoptions_default);
  netsurf_init(NULL);

  win = window_open("main");
  widget_child_on(win, "open:html", EVT_CLICK, on_open, (void*)("html"));
  widget_child_on(win, "open:html_view1", EVT_CLICK, on_open, (void*)("html_view1"));
  widget_child_on(win, "open:html_view2", EVT_CLICK, on_open, (void*)("html_view2"));

  return RET_OK;
}

ret_t application_exit(void) {
  nsoption_finalise(nsoptions, nsoptions_default);
  netsurf_exit();
  log_debug("application_exit\n");
  return RET_OK;
}
