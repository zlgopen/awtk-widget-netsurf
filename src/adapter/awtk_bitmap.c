/**
 * File:   awtk_bitmap.h
 * Author: AWTK Develop Team
 * Brief:  awtk_bitmap
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

#include "awtk_bitmap.h"

static void* awtk_bitmap_create(int width, int height, unsigned int state) {
  return bitmap_create_ex(width, height, 0, BITMAP_FMT_RGBA8888);
}

static unsigned char* awtk_bitmap_get_buffer(void* bitmap) {
  bitmap_t* img = (bitmap_t*)bitmap;

  return bitmap_lock_buffer_for_write(img);
}

static size_t awtk_bitmap_get_rowstride(void* bitmap) {
  bitmap_t* img = (bitmap_t*)bitmap;

  return bitmap_get_line_length(img);
}

static void awtk_bitmap_destroy(void* bitmap) {
  bitmap_t* img = (bitmap_t*)bitmap;
  bitmap_unlock_buffer(img);

  bitmap_destroy(img);
}

static bool awtk_bitmap_save(void* bitmap, const char* path, unsigned flags) {
  assert(!"not impl");
  return true;
}

static void awtk_bitmap_modified(void* bitmap) {
}

static void awtk_bitmap_set_opaque(void* bitmap, bool opaque) {
}

static bool awtk_bitmap_test_opaque(void* bitmap) {
  return true;
}

bool framebuffer_awtk_bitmap_get_opaque(void* bitmap) {
  return true;
}

static int awtk_bitmap_get_width(void* bitmap) {
  bitmap_t* img = (bitmap_t*)bitmap;
  return img->w;
}

static int awtk_bitmap_get_height(void* bitmap) {
  bitmap_t* img = (bitmap_t*)bitmap;
  return img->h;
}

/* get bytes per pixel */
static size_t awtk_bitmap_get_bpp(void* bitmap) {
  bitmap_t* img = (bitmap_t*)bitmap;
  return bitmap_get_bpp_of_format(img->format) / 8;
}

static nserror awtk_bitmap_render(struct bitmap* bitmap, struct hlcache_handle* content) {
  //assert(!"not impl");
  return NSERROR_OK;
}

static struct gui_bitmap_table awtk_awtk_bitmap_table = {
    .create = awtk_bitmap_create,
    .destroy = awtk_bitmap_destroy,
    .set_opaque = awtk_bitmap_set_opaque,
    .get_opaque = framebuffer_awtk_bitmap_get_opaque,
    .test_opaque = awtk_bitmap_test_opaque,
    .get_buffer = awtk_bitmap_get_buffer,
    .get_rowstride = awtk_bitmap_get_rowstride,
    .get_width = awtk_bitmap_get_width,
    .get_height = awtk_bitmap_get_height,
    .get_bpp = awtk_bitmap_get_bpp,
    .save = awtk_bitmap_save,
    .modified = awtk_bitmap_modified,
    .render = awtk_bitmap_render,
};

struct gui_bitmap_table* awtk_get_bitmap_table(void) {
  return &awtk_awtk_bitmap_table;
}
