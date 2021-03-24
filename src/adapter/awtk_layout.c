/**
 * File:   awtk_layout.h
 * Author: AWTK Develop Team
 * Brief:  awtk_layout
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

#include "awtk.h"
#include "awtk_layout.h"

nserror awtk_font_width(const plot_font_style_t* fstyle, const char* string, size_t length,
                        int* width) {
  wstr_t str;
  widget_t* wm = window_manager();
  canvas_t* c = widget_get_canvas(wm);
  float_t ratio = system_info()->device_pixel_ratio;
  int32_t font_size = plot_style_fixed_to_int(fstyle->size);

  canvas_set_font(c, NULL, font_size);

  wstr_init(&str, length);
  wstr_set_utf8_with_len(&str, string, length);

  *width = (int)canvas_measure_text(c, str.str, str.size);

  wstr_reset(&str);

  return NSERROR_OK;
}

nserror awtk_font_position(const plot_font_style_t* fstyle, const char* string, size_t length,
                           int x, size_t* char_offset, int* actual_x) {
  wstr_t str;
  uint32_t i = 0;
  float_t w = 0;
  widget_t* wm = window_manager();
  canvas_t* c = widget_get_canvas(wm);
  float_t ratio = system_info()->device_pixel_ratio;
  int32_t font_size = plot_style_fixed_to_int(fstyle->size);

  canvas_set_font(c, NULL, font_size);
  wstr_init(&str, length);
  wstr_set_utf8_with_len(&str, string, length);

  for (i = 0; i < str.size; i++) {
    float cw = canvas_measure_text(c, str.str + i, 1);
    if (w <= x && (w + cw) >= x) {
      break;
    }
    w += cw;
  }

  wstr_reset(&str);

  *char_offset = i;
  *actual_x = w;

  return NSERROR_OK;
}

/*FIXME*/
static nserror awtk_font_split(const plot_font_style_t* fstyle, const char* string, size_t length,
                               int x, size_t* char_offset, int* actual_x) {
  wstr_t str;
  uint32_t i = 0;
  float_t w = 0;
  widget_t* wm = window_manager();
  canvas_t* c = widget_get_canvas(wm);
  float_t ratio = system_info()->device_pixel_ratio;
  int32_t font_size = plot_style_fixed_to_int(fstyle->size);

  canvas_set_font(c, NULL, font_size);
  wstr_init(&str, length);
  wstr_set_utf8_with_len(&str, string, length);

  for (i = 0; i < str.size; i++) {
    float cw = canvas_measure_text(c, str.str + i, 1);
    if (w <= x && (w + cw) >= x) {
      break;
    }
    w += cw;
  }

  wstr_reset(&str);

  *char_offset = i;
  *actual_x = w;
  return NSERROR_OK;
}

static struct gui_layout_table awtk_font_layout_table = {
    .width = awtk_font_width,
    .position = awtk_font_position,
    .split = awtk_font_split,
};

struct gui_layout_table* awtk_get_font_layout_table(void) {
  return &awtk_font_layout_table;
}
