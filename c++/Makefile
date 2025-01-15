# Compiler and flags
CXX = g++
CXXFLAGS = -O3 -g -Wall -Wextra -pedantic -std=c++20 -I./src -I./tests
LDFLAGS =

# Directories
SRC_DIR = src
TEST_DIR = tests
BUILD_DIR = build

# Source files
SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp)
SRC_FILES_NO_MAIN = $(filter-out $(SRC_DIR)/main.cpp, $(SRC_FILES))
TEST_FILES = $(wildcard $(TEST_DIR)/*.cpp)

# Object files
OBJ_FILES = $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRC_FILES))
TEST_OBJ_FILES = $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRC_FILES_NO_MAIN)) $(patsubst $(TEST_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(TEST_FILES))

# Headers
HEADERS = $(wildcard $(SRC_DIR)/*.h)

# Targets
MAIN_EXECUTABLE = $(BUILD_DIR)/main_executable
TEST_EXECUTABLE = $(BUILD_DIR)/test_executable

# Default target
.DEFAULT_GOAL := all

# Create build directory if it doesn't exist
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Build main executable
$(MAIN_EXECUTABLE): $(OBJ_FILES) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(OBJ_FILES) -o $@ $(LDFLAGS)

# Build test executable
$(TEST_EXECUTABLE): $(TEST_OBJ_FILES) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(TEST_OBJ_FILES) -o $@ $(LDFLAGS)

# Compile source files into object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp $(HEADERS) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Compile test files into object files
$(BUILD_DIR)/%.o: $(TEST_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Phony targets
.PHONY: all clean run test

# Build both executables
all: $(MAIN_EXECUTABLE) $(TEST_EXECUTABLE)

# Clean build files
clean:
	rm -rf $(BUILD_DIR)

# Run main executable
run: $(MAIN_EXECUTABLE)
	./$(MAIN_EXECUTABLE)

# Run test executable
test: $(TEST_EXECUTABLE)
	./$(TEST_EXECUTABLE)
