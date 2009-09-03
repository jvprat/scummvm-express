MODULE := engines/express

MODULE_OBJS = \
	cursor.o \
	detection.o \
	express.o \
	hpf.o

# This module can be built as a plugin
ifeq ($(ENABLE_EXPRESS), DYNAMIC_PLUGIN)
PLUGIN := 1
endif

# Include common rules
include $(srcdir)/rules.mk
