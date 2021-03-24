/**
 * File:   awtk_plot.h
 * Author: AWTK Develop Team
 * Brief:  awtk_plot
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
#include "awtk_plot.h"

static nserror awtk_plot_clip(const struct redraw_context* ctx, const struct rect* clip) {
  canvas_t* c = ((plotter_info_t*)(ctx->priv))->c;

  rect_t r = rect_init(clip->x0, clip->y0, clip->x1 - clip->x0, clip->y1 - clip->y0);
  return_value_if_fail(c != NULL, NSERROR_BAD_PARAMETER);

  //canvas_set_clip_rect(c, &r);

  return NSERROR_OK;
}

static color_t color_from_value(colour value) {
  color_t c;
  c.rgba.r = value & 0xff;
  c.rgba.g = (value >> 8) & 0xff;
  c.rgba.b = (value >> 16) & 0xff;
  c.rgba.a = (value >> 24) & 0xff;
  c.rgba.a = 0xff;

  return c;
}

static void vgcanvas_fill_and_stroke(vgcanvas_t* vg, const plot_style_t* style) {
  if (style->fill_type != PLOT_OP_TYPE_NONE) {
    vgcanvas_set_fill_color(vg, color_from_value(style->fill_colour));
    vgcanvas_fill(vg);
  }

  if (style->stroke_type != PLOT_OP_TYPE_NONE) {
    vgcanvas_set_stroke_color(vg, color_from_value(style->stroke_colour));
    vgcanvas_set_line_width(vg, style->stroke_width);
    vgcanvas_stroke(vg);
  }
}

static nserror awtk_plot_arc(const struct redraw_context* ctx, const plot_style_t* style, int x,
                             int y, int radius, int angle1, int angle2) {
  vgcanvas_t* vg = NULL;
  canvas_t* c = ((plotter_info_t*)(ctx->priv))->c;
  float_t start = (angle1 + 90) * (M_PI / 180);
  float_t end = (angle2 + 90) * (M_PI / 180);

  return_value_if_fail(c != NULL, NSERROR_BAD_PARAMETER);
  vg = lcd_get_vgcanvas(c->lcd);
  return_value_if_fail(vg != NULL, NSERROR_BAD_PARAMETER);

  vgcanvas_set_stroke_color(vg, color_from_value(style->fill_colour));
  vgcanvas_set_line_width(vg, 1);

  vgcanvas_begin_path(vg);
  vgcanvas_arc(vg, x, y, radius, start, end, FALSE);
  vgcanvas_stroke(vg);

  return NSERROR_OK;
}

static nserror awtk_plot_disc(const struct redraw_context* ctx, const plot_style_t* style, int x,
                              int y, int radius) {
  vgcanvas_t* vg = NULL;
  canvas_t* c = ((plotter_info_t*)(ctx->priv))->c;
  return_value_if_fail(c != NULL, NSERROR_BAD_PARAMETER);
  vg = lcd_get_vgcanvas(c->lcd);
  return_value_if_fail(vg != NULL, NSERROR_BAD_PARAMETER);

  vgcanvas_begin_path(vg);
  vgcanvas_arc(vg, x, y, radius, 0, 2 * M_PI, FALSE);
  vgcanvas_close_path(vg);
  vgcanvas_fill_and_stroke(vg, style);

  return NSERROR_OK;
}

static nserror awtk_plot_line(const struct redraw_context* ctx, const plot_style_t* style,
                              const struct rect* line) {
  vgcanvas_t* vg = NULL;
  canvas_t* c = ((plotter_info_t*)(ctx->priv))->c;
  return_value_if_fail(c != NULL, NSERROR_BAD_PARAMETER);
  vg = lcd_get_vgcanvas(c->lcd);
  return_value_if_fail(vg != NULL, NSERROR_BAD_PARAMETER);

  vgcanvas_begin_path(vg);
  vgcanvas_move_to(vg, line->x0, line->y0);
  vgcanvas_line_to(vg, line->x1, line->y1);

  if (style->stroke_type != PLOT_OP_TYPE_NONE) {
    vgcanvas_set_stroke_color(vg, color_from_value(style->stroke_colour));
    vgcanvas_set_line_width(vg, style->stroke_width);
    vgcanvas_stroke(vg);
  }

  return NSERROR_OK;
}

static nserror awtk_plot_rectangle(const struct redraw_context* ctx, const plot_style_t* style,
                                   const struct rect* nsrect) {
  vgcanvas_t* vg = NULL;
  canvas_t* c = ((plotter_info_t*)(ctx->priv))->c;
  return_value_if_fail(c != NULL, NSERROR_BAD_PARAMETER);
  vg = lcd_get_vgcanvas(c->lcd);
  return_value_if_fail(vg != NULL, NSERROR_BAD_PARAMETER);

  vgcanvas_rect(vg, nsrect->x0, nsrect->y0, nsrect->x1 - nsrect->x0, nsrect->y1 - nsrect->y0);
  vgcanvas_fill_and_stroke(vg, style);

  return NSERROR_OK;
}

static nserror awtk_plot_polygon(const struct redraw_context* ctx, const plot_style_t* style,
                                 const int* p, unsigned int n) {
  uint32_t i = 0;
  vgcanvas_t* vg = NULL;
  canvas_t* c = ((plotter_info_t*)(ctx->priv))->c;
  return_value_if_fail(c != NULL, NSERROR_BAD_PARAMETER);
  vg = lcd_get_vgcanvas(c->lcd);
  return_value_if_fail(vg != NULL, NSERROR_BAD_PARAMETER);

  vgcanvas_begin_path(vg);
  vgcanvas_move_to(vg, p[0], p[1]);
  for (i = 1; i != n; i++) {
    vgcanvas_line_to(vg, p[i * 2], p[i * 2 + 1]);
  }
  vgcanvas_fill_and_stroke(vg, style);

  return NSERROR_OK;
}

static nserror awtk_plot_path(const struct redraw_context* ctx, const plot_style_t* pstyle,
                              const float* p, unsigned int n, const float transform[6]) {
  return NSERROR_OK;
}

static nserror awtk_plot_bitmap(const struct redraw_context* ctx, struct bitmap* bitmap, int x,
                                int y, int width, int height, colour bg, bitmap_flags_t flags) {
  bitmap_t* img = (bitmap_t*)bitmap;
  bool repeat_x = (flags & BITMAPF_REPEAT_X);
  bool repeat_y = (flags & BITMAPF_REPEAT_Y);

  vgcanvas_t* vg = NULL;
  rect_t r = rect_init(x, y, width, height);
  canvas_t* c = ((plotter_info_t*)(ctx->priv))->c;
  int32_t ox = c->ox;
  int32_t oy = c->oy;
  return_value_if_fail(c != NULL, NSERROR_BAD_PARAMETER);
  vg = lcd_get_vgcanvas(c->lcd);
  return_value_if_fail(vg != NULL, NSERROR_BAD_PARAMETER);

  canvas_translate(c, -ox, -oy);
  if (!repeat_x && !repeat_y) {
    canvas_draw_image_ex(c, img, IMAGE_DRAW_DEFAULT, &r);
  } else if (repeat_x) {
    canvas_draw_image_ex(c, img, IMAGE_DRAW_REPEAT_X, &r);
  } else if (repeat_y) {
    canvas_draw_image_ex(c, img, IMAGE_DRAW_REPEAT_Y, &r);
  }
  canvas_translate(c, ox, oy);

  return NSERROR_OK;
}

static nserror awtk_plot_text(const struct redraw_context* ctx,
                              const struct plot_font_style* fstyle, int x, int y, const char* text,
                              size_t length) {
  wstr_t str;
  vgcanvas_t* vg = NULL;
  canvas_t* c = ((plotter_info_t*)(ctx->priv))->c;
  float_t ratio = system_info()->device_pixel_ratio;

  int32_t ox = c->ox;
  int32_t oy = c->oy;
  int32_t font_size = plot_style_fixed_to_int(fstyle->size);
  return_value_if_fail(c != NULL, NSERROR_BAD_PARAMETER);
  vg = lcd_get_vgcanvas(c->lcd);
  return_value_if_fail(vg != NULL, NSERROR_BAD_PARAMETER);

  wstr_init(&str, 0);
  wstr_set_utf8_with_len(&str, text, length);

  canvas_set_font(c, NULL, font_size);
  canvas_set_text_color(c, color_from_value(fstyle->foreground));

  canvas_translate(c, -ox, -oy);
  canvas_draw_text(c, str.str, str.size, x, y - font_size);
  canvas_translate(c, ox, oy);

  wstr_reset(&str);

  return NSERROR_OK;
}

/** framebuffer plot operation table */
const struct plotter_table fb_plotters = {
    .clip = awtk_plot_clip,
    .arc = awtk_plot_arc,
    .disc = awtk_plot_disc,
    .line = awtk_plot_line,
    .rectangle = awtk_plot_rectangle,
    .polygon = awtk_plot_polygon,
    .path = awtk_plot_path,
    .bitmap = awtk_plot_bitmap,
    .text = awtk_plot_text,
    .option_knockout = true,
};

const struct plotter_table* awtk_get_plotters_table(void) {
  return &fb_plotters;
}
