# super makefile
# Auth: Ronghua Gao

include $(PLATTOP)/common.mk

PLATTOP?=/home/ezgaoro/grh/MyResource/cppfram
BIN_PATH?=$(PLATTOP)/bin
LIB_PATH?=$(PLATTOP)/lib

MV?=mv
CP?=cp
RM?=rm

#Compile parameter settings
CC=/home/ezgaoro/grh/MyResource/gcc-4.8.5/bin/gcc
#CC=gcc
CXX=/home/ezgaoro/grh/MyResource/gcc-4.8.5/bin/g++
#CXX=g++
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/home/ezgaoro/grh/MyResource/gmp-6.1.0/lib:/home/ezgaoro/grh/MyResource/mpfr-3.1.4/lib://home/ezgaoro/grh/MyResource/mpc-1.0.3/lib

AR=ar
OPTIMIZATION=-O2
WARNINGS=-Wall 
DEBUG= -g -ggdb

COMM_INCLUDES := $(foreach INC,$(shell find $(PLATTOP) -type d -name inc),-I$(INC))
COMM_CFLAGS := -fPIC $(OPTIMIZATION)  $(WARNINGS) $(DEBUG) -D_GNU_SOURCE $(COMM_INCLUDES)
COMM_CXXFLAGS := -fPIC $(OPTIMIZATION)  $(WARNINGS) $(DEBUG) -D_GNU_SOURCE $(COMM_INCLUDES)
COMM_LDFLAGS := -L$(LIB_PATH)
COMM_LDLIBS := 

all:
%.o:%.c
	$(CC) -std=c99 -pedantic -c $(COMM_CFLAGS) $(CFLAGS) $<

%.o:%.cc
	$(CXX) -std=c++11 -c $(COMM_CXXFLAGS) $(CXXFLAGS) $<

%.o:%.cpp
	$(CXX) -std=c++11 -pedantic -c $(COMM_CXXFLAGS) $(CXXFLAGS) $<

#Rule for building so
define build_so
.PHONY: $(1).so
SO_TARGET += $(1).so
$(1)_REAL_LDFLAGS := $(COMM_LDFLAGS) $($(1)_LDFLAGS)
$(1)_REAL_LDLIBS := $(COMM_LDLIBS) $($(1)_LDLIBS)
$(1)_SOLIB_SRC := $(patsubst %.cpp,%.o,$(patsubst %.cc,%.o,$(patsubst %.c,%.o,$($(1)_SRC))))

$(1).so: $$($(1)_SOLIB_SRC)
	$(CXX) -shared -fPIC -std=c++0x -o $$@ $$($(1)_SOLIB_SRC) $$($(1)_REAL_LDFLAGS) $$($(1)_REAL_LDLIBS)
	$(MV) $$@ $$(LIB_PATH)
endef

#Rule for building a
define build_a
.PHONY: $(1).a
A_TARGET += $(1).a
$(1)_REAL_LDFLAGS := $(COMM_LDFLAGS) $($(1)_LDFLAGS)
$(1)_REAL_LDLIBS := $(COMM_LDLIBS) $($(1)_LDLIBS)
$(1)_ALIB_SRC := $(patsubst %.cpp,%.o,$(patsubst %.cc,%.o,$(patsubst %.c,%.o,$($(1)_SRC))))

$(1).a: $$($(1)_ALIB_SRC)
	$(AR) rcs $$@ $$($(1)_REAL_CFLAGS) $$($(1)_ALIB_SRC)
	$(MV) $$@ $$(LIB_PATH)
endef

#Rule for building bin
define build_bin
.PHONY: $(1)
BIN_TARGET += $(1)
$(1)_REAL_LDFLAGS := $(COMM_LDFLAGS) $($(1)_LDFLAGS)
$(1)_REAL_LDLIBS := $(COMM_LDLIBS) $($(1)_LDLIBS)
$(1)_BIN_SRC := $($(1)_SRC:.c=.o)
$(1)_BIN_SRC := $$($(1)_BIN_SRC:.cc=.o)

$(1): $$($(1)_BIN_SRC)
	$(CXX) -o $$@ $$($(1)_REAL_CFLAGS) $$($(1)_BIN_SRC) $$($(1)_REAL_LDFLAGS) $$($(1)_REAL_LDLIBS) 
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


