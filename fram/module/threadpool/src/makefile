# module makefile
# Auth: Ronghua Gao

include $(PLATHOME)/common.mk

OBJ_PATH := $(shell pwd)/../obj

BINS :=
$(BINS)_SRC :=
$(BINS)_LDLIBS :=

LIBS := libTpool
libTpool_SRC := workerlist.o   \
   eventlist.o \
   Tpoolevent.o  \
   Tpoolcore.o \
   Tpool.o

CFLAGS := -I$(shell pwd)/../inc 


include $(PLATHOME)/super.mk
