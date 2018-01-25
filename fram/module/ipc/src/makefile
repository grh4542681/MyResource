# module makefile
# Auth: Ronghua Gao

include $(PLATHOME)/common.mk

OBJ_PATH := $(shell pwd)/../obj

BINS :=
$(BINS)_SRC :=
$(BINS)_LDLIBS :=

LIBS := libipc
libipc_SRC := ipcsem.o \
				ipcshm.o \
				arrshm.o \
				ipcmsg.o

CFLAGS := -I$(shell pwd)/../inc 


include $(PLATHOME)/super.mk
