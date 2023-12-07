# Makefile for the libnymphcast C11 binding example.
# 2023/12/07, Maya Posch

TARGET := nymphcast_client

GCC ?= gcc
GPP ?= g++
MKDIR := mkdir -p

ARCH := $(shell g++ -dumpmachine)

CFLAGS := -std=c11 -g3
CCFLAGS := -std=c++11 -g3
LDFLAGS := 
SRC := $(wildcard *.c)
CPPSRC := $(wildcard ../src/*.cpp)
OBJ := $(addprefix obj/$(ARCH)/,$(notdir $(SRC:.c=.o)))
CPPOBJ := $(addprefix obj/$(ARCH)/,$(notdir $(CPPSRC:.cpp=.o)))
LIBS := -L../../lib/$(ARCH)/ -lnymphcast -lnymphrpc -lPocoNet -lPocoNetSSL \
							 -lPocoUtil -lPocoFoundation

all: makedir bin/$(ARCH)/$(TARGET)

makedir:
	$(MKDIR) obj/$(ARCH)
	$(MKDIR) bin/$(ARCH)
	
# obj/$(ARCH)/%.o: %.cpp
	# $(GPP) -c -o $@ $< $(CCFLAGS)
	
obj/$(ARCH)/%.o: $(CPPSRC)
	$(GPP) -c -o $@ $< $(CCFLAGS)
	
obj/$(ARCH)/%.o: %.c
	$(GCC) -c -o $@ $< $(CFLAGS)
	
bin/$(ARCH)/$(TARGET): $(OBJ) $(CPPOBJ)
	$(GPP) -o $@ $(OBJ) $(CPPOBJ) $(LDFLAGS) $(LIBS)
	
.PHONY: obj bin makedir
