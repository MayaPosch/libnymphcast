# Makefile for the libnymphcast C11 binding example.
# 2023/12/07, Maya Posch

TARGET := nymphcast_client

GCC ?= gcc
GPP ?= g++
MKDIR := mkdir -p
RM	:= rm

ARCH := $(shell g++ -dumpmachine)

CFLAGS := -std=c11 -g3
CCFLAGS := -std=c++11 -g3
LDFLAGS := 
SRC := $(wildcard *.c)
CPPSRC := $(wildcard ../src/*.cpp)
OBJ := $(addprefix obj/$(ARCH)/c/,$(notdir $(SRC:.c=.o)))
CPPOBJ := $(addprefix obj/$(ARCH)/cpp/,$(notdir $(CPPSRC:.cpp=.o)))
LIBS := -L../../lib/$(ARCH)/ -lnymphcast -lnymphrpc -lPocoNet -lPocoNetSSL \
							 -lPocoUtil -lPocoFoundation
							 
$(info $$SRC is [${SRC}])

all: makedir bin/$(ARCH)/$(TARGET)

makedir:
	$(MKDIR) obj/$(ARCH)/cpp
	$(MKDIR) obj/$(ARCH)/c
	$(MKDIR) bin/$(ARCH)
	
obj/$(ARCH)/cpp/%.o: $(CPPSRC)
	$(GPP) -c -o $@ $< $(CCFLAGS)
	
obj/$(ARCH)/c/%.o: $(SRC)
	$(GCC) -c -o $@ $< $(CFLAGS)
	
bin/$(ARCH)/$(TARGET): $(OBJ) $(CPPOBJ)
	$(GPP) -o $@ $(OBJ) $(CPPOBJ) $(LDFLAGS) $(LIBS)
	
clean:
	$(RM) $(OBJ)
	$(RM) $(CPPOBJ)
	
.PHONY: obj bin makedir
