# module makefile
# Auth: Ronghua Gao


OBJ_PATH := $(shell pwd)/../obj

BINS := nameserver

nameserver_SRC := nameserver.c
nameserver_LDLIBS := -llog -lpub -pthread -lipc -lJSON -lrt -lm
nameserver_LDFLAGS :=

include $(PLATHOME)/super.mk
