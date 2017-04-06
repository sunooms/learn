# google test directory
# GTEST_DIR = /usr/src/gtest-1.5.0

# flags passed to the preprocessor
# CPPFLAGS += -I$(GTEST_DIR)/include

# Flags passed to the c++ compiler
# CXXFLAGS += -g -Wall -Wextra

# all google test headers. Usually you shouldn'g change this definition
# GTEST_HEADERS = $(GTEST_DIR)/include/gtest/*.h $(GTEST_DIR)/include/gtest/internal/*.h

# all google test sources
# GTEST_SRCS = $(GTEST_DIR)/src/*.cc $(GTEST_DIR)/src/*.h $(GTEST_HEADERS)

#here recursion three class directory
DIRS := $(shell find . -maxdepth 3 -type d)

#here reverse directory's cpp file
SRC_FILES=$(foreach dir,$(DIRS),$(wildcard $(dir)/*.cpp))

# all test produced by this Makefile. Remember to add new tests you created to the list.
# TESTS = test

all: gtest_demo

gtest_demo:
	g++ -o gtest_demo $(SRC_FILES) -lgtest -lpthread

clean:
	rm gtest_demo
