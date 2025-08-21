CXX := g++
CXXFLAGS := -Wall -Wextra -Wpedantic -std=c++17

# Source files
SRCS := editor.cpp address.cpp parser.cpp executor.cpp command.cpp
OBJS := $(SRCS:.cpp=.o)

TEST_SRCS := test.cpp address.cpp parser.cpp command.cpp
TEST_OBJS := $(TEST_SRCS:.cpp=.o)

# Default target
all: editor

# Build the main editor executable
editor: $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $@

# Build the test executable
test: $(TEST_OBJS)
	$(CXX) $(CXXFLAGS) $(TEST_OBJS) -o $@
	./test

# Generic rule for building .o files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

