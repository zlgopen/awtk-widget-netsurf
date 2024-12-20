﻿/**
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

#ifndef TK_WEB_VIEW_H
#define TK_WEB_VIEW_H

#include "base/widget.h"

BEGIN_C_DECLS

/**
 * @class web_view_t
 * @parent widget_t
 * @annotation ["scriptable","design","widget"]
 * 
 * web_view是一个专用容器，用来放 html 和 scroll bar，并在两者之间建立联系。
 *
 * web_view\_t是[widget\_t](widget_t.md)的子类控件，widget\_t的函数均适用于web_view\_t控件。
 *
 * 在xml中使用"web_view"标签创建web_view。如：
 *
 * ```xml
 * <web_view x="0" y="10" w="100%" h="300">
 *   <html x="0" y="10" w="-12" h="100%">
 *   ...
 *   </html>
 *   <scroll_bar_d name="bar" x="right" y="0" w="12" h="100%" value="0"/>
 * </web_view>
 * ```
 *
 * 可用通过style来设置控件的显示风格，如背景颜色等。如：
 *
 * ```xml
 * <style name="default" border_color="#a0a0a0">
 *   <normal     bg_color="#f0f0f0" />
 * </style>
 * ```
 */
typedef struct _web_view_t {
  widget_t widget;

  /*private*/
  widget_t* scroll_bar;
  widget_t* html;
} web_view_t;

/**
 * @method web_view_create
 * 创建web_view对象
 * @annotation ["constructor", "scriptable"]
 * @param {widget_t*} parent 父控件
 * @param {xy_t} x x坐标
 * @param {xy_t} y y坐标
 * @param {wh_t} w 宽度
 * @param {wh_t} h 高度
 *
 * @return {widget_t*} 对象。
 */
widget_t* web_view_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);

/**
 * @method web_view_cast
 * 转换为web_view对象(供脚本语言使用)。
 * @annotation ["cast", "scriptable"]
 * @param {widget_t*} widget web_view对象。
 *
 * @return {widget_t*} web_view对象。
 */
widget_t* web_view_cast(widget_t* widget);

#define WIDGET_TYPE_WEB_VIEW "web_view"

#define WEB_VIEW(widget) ((web_view_t*)(web_view_cast(WIDGET(widget))))

END_C_DECLS

#endif /*TK_WEB_VIEW_H*/
