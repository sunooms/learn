CC := g++

DSP_DIR =  ../..

DIR_OBJ = ./obj

SRCS := dispatcher_test.cpp \
	  ../../core/session.cpp \
	  ../../interface/flow.cpp \
	  ../../driver/dispatcher.cpp \
	  ../../driver/module_manager.cpp \
	  ../../driver/mod_config_helper.cpp 


#OBJS := $(patsubst %cpp,%o,$(SRCS))
OBJS := $(addsuffix .o, $(basename $(SRCS)))


CXXFLAGS = -g -Wall -I. -I$(DSP_DIR) -I$(DSP_DIR)/interface  -I$(DSP_DIR)/driver -I$(DSP_DIR)/common -I$(DSP_DIR)/core

all: test

%.o:%.cpp
	$(CC) $(CXXFLAGS) -c $<

test: $(SRCS)
	$(CC) $(CXXFLAGS) -o $@ $^


clean:
	rm -rf *.o a.out test
