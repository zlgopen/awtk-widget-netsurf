﻿import os
import scripts.app_helper as app

NETSURF_ROOT = os.path.normpath(os.path.abspath('3rd/netsurf'))
NETSURF_ROOT_INC = os.path.join(NETSURF_ROOT, 'inst-awtk/include')
NETSURF_ROOT_LIB = os.path.join(NETSURF_ROOT, 'inst-awtk/lib')
NETSURF_ROOT_SRC = os.path.join(NETSURF_ROOT, 'netsurf')
NETSURF_ROOT_SRC_INC = os.path.join(NETSURF_ROOT, 'netsurf/include')

helper = app.Helper(ARGUMENTS);

APP_CPPPATH=[
  NETSURF_ROOT_INC, 
  NETSURF_ROOT_SRC,
  NETSURF_ROOT_SRC_INC
]

APP_LIBPATH=[
  NETSURF_ROOT_LIB
]

APP_LIBS=[
"web_view",
"nsawtk",
"svgtiny",
"css",
"dom",
"hubbub",
"nsbmp",
"nsgif",
"nslog",
"nspsl",
"nsutils",
"parserutils",
"rosprite",
"utf8proc",
"wapcaplet",
"curl",
"ssl",
"crypto",
"jpeg",
"png",
"webp",
"expat",
"z"
]

helper.add_cpppath(APP_CPPPATH).add_libpath(APP_LIBPATH).add_libs(APP_LIBS)

helper.call(DefaultEnvironment)

SConscriptFiles = ['src/SConscript', 'demos/SConscript']
SConscript(SConscriptFiles)
