# module makefile
# Auth: Ronghua Gao


OBJ_PATH := $(shell pwd)/../obj

BINS := log-chk-conf
$(BINS)_SRC := zlog-chk-conf.c
$(BINS)_LDLIBS := -llog -pthread

LIBS := liblog
liblog_SRC := buf.c	\
		category.c    \
		category_table.c    \
		conf.c    \
		event.c    \
		format.c    \
		level.c    \
		level_list.c    \
		mdc.c    \
		record.c    \
		record_table.c    \
		rotater.c    \
		rule.c    \
		spec.c    \
		thread.c    \
		zc_arraylist.c    \
		zc_hashtable.c    \
		zc_profile.c    \
		zc_util.c    \
		zlog.c \
		log.c

CFLAGS := -I$(shell pwd)/../inc
include $(PLATTOP)/super.mk
