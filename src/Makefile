# Compiler and flags
CXX := g++
CXXFLAGS := -Wall -Wextra -pedantic -std=c++17
CXXFLAGS += -ILCBLog/src
# CXXFLAGS += -I../../LCBLog/src
LDFLAGS := 

# Directories
SRC_DIR := .
LCBLOG_DIR := LCBLog/src
#LCBLOG_DIR := ../../LCBLog/src

# Explicitly define sources WITHOUT trailing spaces:
SRCS := $(SRC_DIR)/ini_file.cpp $(LCBLOG_DIR)/lcblog.cpp
OBJS := $(SRC_DIR)/ini_file.o $(LCBLOG_DIR)/lcblog.o

# Test program sources (ONLY compiled in 'make test')
TEST_SRC := main.cpp
TEST_OBJ := $(TEST_SRC:.cpp=.o)
TEST_BIN := ini_test

# Default target: Build library only (EXCLUDES main.cpp)
all: $(OBJS)

# Rules for ini_file
$(SRC_DIR)/ini_file.o: $(SRC_DIR)/ini_file.cpp
	$(CXX) $(CXXFLAGS) -c "$<" -o "$@"

# Rules for lcblog
$(LCBLOG_DIR)/lcblog.o: $(LCBLOG_DIR)/lcblog.cpp
	$(CXX) $(CXXFLAGS) -c "$<" -o "$@"

# Clean all compiled files (INCLUDING dependency files)
clean:
	rm -f $(OBJS) $(TEST_OBJ) $(TEST_BIN) $(SRC_DIR)/*.d $(LCBLOG_DIR)/*.d

# Show detected source and object files
files:
	@echo "Library source files: $(SRCS)"
	@echo "Test source file: $(TEST_SRC)"
	@echo "Library object files: $(OBJS)"
	@echo "Test object file: $(TEST_OBJ)"

# Show all defined macros
define:
	@echo "Defined macros:"
	@$(CXX) -dM -E -x c++ /dev/null | sort

# Compile and run test program (INCLUDES main.cpp)
test: $(OBJS) $(TEST_OBJ)
	$(CXX) $(CXXFLAGS) $(OBJS) $(TEST_OBJ) -o $(TEST_BIN)
	./$(TEST_BIN)

# Run static analysis with cppcheck
lint:
	@which cppcheck >/dev/null 2>&1 || { echo "Error: cppcheck not found. Please install it."; exit 1; }
	cppcheck --enable=all --std=c++17 --quiet $(SRCS) $(TEST_SRC)

# Display available targets
help:
	@echo "Available targets:"
	@echo "  make (or make all)   - Build the ini_file library only"
	@echo "  make clean           - Remove all object files and test binary"
	@echo "  make files           - Show detected source and object files"
	@echo "  make defines         - Show all defined macros"
	@echo "  make test            - Build and run the test program"
	@echo "  make lint            - Run static analysis with cppcheck"
	@echo "  make help            - Show this help message"
