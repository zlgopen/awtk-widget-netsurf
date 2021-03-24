/**
 * File:   awtk_clipboard.h
 * Author: AWTK Develop Team
 * Brief:  clipboard 
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

#include "awtk_clipboard.h"

static void gui_get_clipboard(char** buffer, size_t* length) {
  clip_board_data_type_t type = CLIP_BOARD_DATA_TYPE_TEXT;
  clip_board_get_data(clip_board(), &type, (const void**)buffer, (uint32_t*)length);
}

static void gui_set_clipboard(const char* buffer, size_t length, nsclipboard_styles styles[],
                              int n_styles) {
  clip_board_set_data(clip_board(), CLIP_BOARD_DATA_TYPE_TEXT, buffer, length);
}

static struct gui_clipboard_table clipboard_table = {
    .get = gui_get_clipboard,
    .set = gui_set_clipboard,
};

struct gui_clipboard_table* awtk_get_clipboard_table(void) {
  return &clipboard_table;
}
