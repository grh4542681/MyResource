# module makefile
# Auth: Ronghua Gao

include $(PLATHOME)/common.mk

OBJ_PATH := $(shell pwd)/../obj

BINS := Tpooltest initest ipctest

Tpooltest_SRC := Tpooltest.c
Tpooltest_LDLIBS := -llog -lTpool -lpub -pthread
Tpooltest_LDFLAGS :=

initest_SRC := initest.c
initest_LDLIBS := -llog -lpINI -lpub -pthread

ipctest_SRC := ipctest.c
ipctest_LDLIBS := -llog -lipc -lpub -pthread -lrt

LIBS := 
libpub_SRC := 

include $(PLATHOME)/super.mk
