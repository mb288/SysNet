# Compiler
CXX := g++

# Compiler flags
CXXFLAGS := -std=c++11 -Wall -Wextra

# Source files
SRCS := tcp_server.cpp main.cpp

# Object files
OBJS := $(SRCS:.cpp=.o)

# Executable name
EXEC := tcp_server

.PHONY: all clean

all: $(EXEC)

# Linking object files to create executable
$(EXEC): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(EXEC)

# Compile source files into object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS) $(EXEC)
