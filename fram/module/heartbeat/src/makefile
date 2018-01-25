# module makefile
# Auth: Ronghua Gao

include $(PLATHOME)/common.mk

OBJ_PATH := $(shell pwd)/../obj

BINS := hbserver hbclient hbcmd

hbserver_SRC := hbserver.c packpkg.c
hbserver_LDLIBS := -llog -lpub -pthread -lipc -lJSON -lrt -lm
hbserver_LDFLAGS :=

hbclient_SRC := hbclient.c packpkg.c
hbclient_LDLIBS := -llog -lpub -pthread -lipc -lJSON -lrt -lm

hbcmd_SRC := hbcmd.c
hbcmd_LDLIBS := -llog -lipc -lpub -pthread -lrt

LIBS := 
libpub_SRC := 

include $(PLATHOME)/super.mk
