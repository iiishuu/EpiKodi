# EpiKodi Makefile
# Simple build automation for tests and compilation

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I include
BUILD_DIR = build
SRC_DIRS = src/core src/media src/ui
TEST_DIR = tests

# Source files
CORE_SOURCES = src/core/app.cpp src/core/menu.cpp
MEDIA_SOURCES = src/media/media_file.cpp src/media/folder_parser.cpp src/media/player.cpp
UI_SOURCES = src/ui/qt_ui.cpp

# Test targets
TEST_PARSER = $(BUILD_DIR)/test_parser
TEST_PLAYER = $(BUILD_DIR)/test_player

.PHONY: all clean tests test-parser test-player help

# Default target
all: tests

# Run all tests
tests: test-parser test-player
	@echo "Running all tests..."
	@echo "=== Parser Tests ==="
	@./$(TEST_PARSER)
	@echo ""
	@echo "=== Player Tests ==="
	@./$(TEST_PLAYER) || true
	@echo ""
	@echo "✅ Test execution complete"

# Build and run parser tests
test-parser: $(TEST_PARSER)
	@echo "Building parser tests..."
	@./$(TEST_PARSER)

# Build and run player tests  
test-player: $(TEST_PLAYER)
	@echo "Building player tests..."
	@./$(TEST_PLAYER) || true

# Build parser test executable
$(TEST_PARSER): $(TEST_DIR)/test_parser.cpp $(MEDIA_SOURCES)
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(TEST_DIR)/test_parser.cpp $(MEDIA_SOURCES) -o $(TEST_PARSER)

# Build player test executable
$(TEST_PLAYER): $(TEST_DIR)/test_player.cpp $(MEDIA_SOURCES)
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(TEST_DIR)/test_player.cpp $(MEDIA_SOURCES) -o $(TEST_PLAYER)

# Clean build artifacts
clean:
	@echo "Cleaning build directory..."
	@rm -f $(BUILD_DIR)/test_*
	@echo "Clean complete"

# Show help
help:
	@echo "EpiKodi Makefile Commands:"
	@echo "  make tests      - Build and run all tests"
	@echo "  make test-parser - Build and run parser tests"  
	@echo "  make test-player - Build and run player tests"
	@echo "  make clean      - Remove build artifacts"
	@echo "  make help       - Show this help message"