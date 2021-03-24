/**
 * File:   awtk_schedule.c
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

#include "awtk_schedule.h"

typedef void (*timer_callback_t)(void* p);

static ret_t on_misc_timer(const timer_info_t* info) {
  timer_callback_t on_timer = (timer_callback_t)(info->extra_ctx);

  if (on_timer != NULL) {
    on_timer(info->ctx);
  }

  return RET_REMOVE;
}

static nserror awtk_misc_schedule(int t, void (*callback)(void* p), void* p) {
  if (t < 0) {
    timer_remove_all_by_ctx(p);
    log_debug("remove timer %p %p\n", callback, p);
  } else {
    int32_t id = timer_add(on_misc_timer, p, t);
    timer_info_t* info = (timer_info_t*)timer_find(id);
    if (info != NULL) {
      info->extra_ctx = callback;
    }
  }

  return NSERROR_OK;
}

static void awtk_misc_quit(void) {
  tk_quit();
}

static struct gui_misc_table awtk_misc_table = {
    .schedule = awtk_misc_schedule,
    .quit = awtk_misc_quit,
};

struct gui_misc_table* awtk_get_misc_table(void) {
  return &awtk_misc_table;
}
