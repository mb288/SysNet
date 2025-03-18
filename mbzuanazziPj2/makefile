# Compiler
CXX := g++

# Compiler flags
CXXFLAGS := -std=c++11 -Wall -Wextra

# Source files for tcp_server
SRCS_SERVER := tcp_server.cpp main.cpp

# Source files for tcp_client
SRCS_CLIENT := tcp_client.cpp tcp_server.cpp

# Object files for tcp_server
OBJS_SERVER := $(SRCS_SERVER:.cpp=.o)

# Object files for tcp_client
OBJS_CLIENT := $(SRCS_CLIENT:.cpp=.o)

# Executables
EXEC_SERVER := tcp_server
EXEC_CLIENT := tcp_client

.PHONY: all clean

all: $(EXEC_SERVER) $(EXEC_CLIENT)

# Linking object files to create executables
$(EXEC_SERVER): $(OBJS_SERVER)
	$(CXX) $(CXXFLAGS) $(OBJS_SERVER) -o $(EXEC_SERVER)

$(EXEC_CLIENT): $(OBJS_CLIENT)
	$(CXX) $(CXXFLAGS) $(OBJS_CLIENT) -o $(EXEC_CLIENT)

# Compile source files into object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS_SERVER) $(OBJS_CLIENT) $(EXEC_SERVER) $(EXEC_CLIENT)
