#
# Makefile for NetSurf Framebuffer frontend
#
# This file is part of NetSurf 
#
# ----------------------------------------------------------------------------
# Framebuffer flag setup (using pkg-config)
# ----------------------------------------------------------------------------

CFLAGS += -g -Dnsframebuffer -Dsmall -DPATH_MAX=256 -fPIC
LDFLAGS += -shared -liconv -lm

# S_FRONTEND are sources purely for the framebuffer build
S_FRONTEND := gui.c 

SOURCES = $(S_COMMON) $(S_IMAGE) $(S_BROWSER) $(S_FRONTEND) $(S_IMAGES) $(S_FONTS)
EXETARGET := libnsawtk.dylib

