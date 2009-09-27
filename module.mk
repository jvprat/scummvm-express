MODULE := engines/express

MODULE_OBJS = \
	bg.o \
	cursor.o \
	detection.o \
	express.o \
	font.o \
	hpf.o \
	sbe.o \
	seq.o \
	snd.o

# This module can be built as a plugin
ifeq ($(ENABLE_EXPRESS), DYNAMIC_PLUGIN)
PLUGIN := 1
endif

# Include common rules
include $(srcdir)/rules.mk
