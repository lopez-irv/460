# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -ggdb -Wall -Wextra -pedantic

# Executable name
TARGET = assignment4

# Source and object files
SRCS = main.cpp
OBJS = $(SRCS:.cpp=.o)

# Header dependencies
DEPS = TreeNode.hpp LCRSTree.hpp symbolTable.hpp

# Default rule
all: $(TARGET)

# Link the final executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(TARGET)

# Compile main.o, recompile if headers change
main.o: main.cpp $(DEPS)
	$(CXX) $(CXXFLAGS) -c main.cpp -o main.o

# Clean build artifacts
clean:
	rm -f *.o $(TARGET) *~
