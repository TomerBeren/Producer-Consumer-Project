# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -std=c++11 -pthread -Iinclude

# Executable name
TARGET = ex3.out

# Source files
SRCS = src/main.cpp src/BoundedBuffer.cpp src/Producer.cpp src/Dispatcher.cpp src/CoEditor.cpp src/ScreenManager.cpp

# Object files
OBJS = $(SRCS:.cpp=.o)

# Rule to build the executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

# Rule to build object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up build files
clean:
	rm -f $(TARGET) $(OBJS)

# PHONY targets
.PHONY: clean
