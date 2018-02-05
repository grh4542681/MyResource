# super makefile
# Auth: Ronghua Gao

include $(PLATHOME)/common.mk

PLATHOME?=/home/ezgaoro/grh/MyResource/fram
BIN_PATH?=$(PLATHOME)/bin
LIB_PATH?=$(PLATHOME)/lib

MV?=mv
CP?=cp
RM?=rm

#Compile parameter settings
CC=gcc
AR=ar
OPTIMIZATION=-O2
WARNINGS=-Wall 
DEBUG= -g -ggdb

COMM_INCLUDES := $(foreach INC,$(shell find $(PLATHOME) -type d -name inc),-I$(INC))
COMM_CFLAGS := -fPIC $(OPTIMIZATION)  $(WARNINGS) $(DEBUG) -D_GNU_SOURCE $(COMM_INCLUDES)
COMM_LDFLAGS := -L$(LIB_PATH) 
COMM_LDLIBS := 

all:

%.o:%.c
	$(CC) -std=c99 -pedantic -c $(COMM_CFLAGS) $(CFLAGS) $<

#Rule for building so
define build_so
.PHONY: $(1).so
SO_TARGET += $(1).so
$(1)_REAL_LDFLAGS := $(COMM_LDFLAGS) $($(1)_LDFLAGS)
$(1)_REAL_LDLIBS := $(COMM_LDLIBS) $($(1)_LDLIBS)
$(1)_LIB_SRC := $($(1)_SRC:.c=.o)

$(1).so: $$($(1)_LIB_SRC)
	$(CC) -shared -o $$@ $$($(1)_REAL_CFLAGS) $$($(1)_LIB_SRC)
	$(MV) $$@ $$(LIB_PATH)
endef

#Rule for building a
define build_a
.PHONY: $(1).a
A_TARGET += $(1).a
$(1)_REAL_LDFLAGS := $(COMM_LDFLAGS) $($(1)_LDFLAGS)
$(1)_REAL_LDLIBS := $(COMM_LDLIBS) $($(1)_LDLIBS)
$(1)_LIB_SRC := $($(1)_SRC:.c=.o)

$(1).a: $$($(1)_LIB_SRC)
	$(AR) rcs $$@ $$($(1)_REAL_CFLAGS) $$($(1)_LIB_SRC)
	$(MV) $$@ $$(LIB_PATH)
endef

#Rule for building bin
define build_bin
.PHONY: $(1)
BIN_TARGET += $(1)
$(1)_REAL_LDFLAGS := $(COMM_LDFLAGS) $($(1)_LDFLAGS)
$(1)_REAL_LDLIBS := $(COMM_LDLIBS) $($(1)_LDLIBS)
$(1)_BIN_SRC := $($(1)_SRC:.c=.o)

$(1): $$($(1)_BIN_SRC)
	$(CC) -o $$@ $$($(1)_REAL_CFLAGS) $$($(1)_BIN_SRC) $$($(1)_REAL_LDFLAGS) $$($(1)_REAL_LDLIBS) 
#	$(MV) $$@ $$(BIN_PATH)
endef

$(foreach lib,$(LIBS),$(eval $(call build_so,$(lib))))
$(foreach lib,$(LIBS),$(eval $(call build_a,$(lib))))
$(foreach bin,$(BINS),$(eval $(call build_bin,$(bin))))

all:*.o $(SO_TARGET) $(A_TARGET) $(BIN_TARGET)
	#$(MV) *.o $(OBJ_PATH)

.PHONY: clean
clean:
	@$(RM) -f *.o $(SO_TARGET) $(A_TARGET) $(BIN_TARGET)


