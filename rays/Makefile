CC := g++
LD := g++

INCLUDE_DIR := include
SRC_DIR := src
TEST_DIR := tests
BUILD := build


CFLAGS := -Wall -Wextra -Wpedantic  -I $(INCLUDE_DIR) -Isrc -fpermissive -Ofast -std=c++17 -march=native -ffast-math
# -fsanitize=address -ggdb
#-DDEBUG -D_FORTIFY_SOURCE=2 -Og -ggdb -std=c++23
# DEBUG_FLAGS := -Wall -Wextra -Wpedantic  -I $(INCLUDE_DIR) -Isrc -fpermissive -DDEBUG -lstdc++ -D_FORTIFY_SOURCE=2 -Og -ggdb -fsanitize=address
TEST_FLAGS := $(CFLAGS) -lgtest -lgmock
LDFLAGS := $(CFLAGS)


# create subdirs 

SUB_DIRS := $(shell find $(SRC_DIR) -type d)
SUB_DIRS := $(filter-out $(SRC_DIR), $(SUB_DIRS))
BUILD_SUB_DIRS := $(patsubst $(SRC_DIR)/%, $(BUILD)/%, $(SUB_DIRS))

TESTS_SUB_DIRS := $(shell find $(TEST_DIR) -type d)
TESTS_SUB_DIRS := $(filter-out $(TEST_DIR), $(TESTS_SUB_DIRS))
TESTS_SUB_DIRS := $(filter-out $(TEST_DIR)/codegen_output, $(TESTS_SUB_DIRS))
TESTS_BUILD_SUB_DIRS := $(patsubst $(TEST_DIR)/%, $(BUILD)/tests/%, $(TESTS_SUB_DIRS))

# INIT BUILD SUB DIRS
$(shell mkdir -p $(BUILD_SUB_DIRS) $(TESTS_BUILD_SUB_DIRS))


SOURCES := $(shell find $(SRC_DIR) -name "*.cpp")
SOURCES := $(filter-out $(SRC_DIR)/main.cpp, $(SOURCES))
OBJECTS := $(patsubst $(SRC_DIR)%.cpp, $(BUILD)%.o, $(SOURCES))

TEST_SOURCES := $(shell find $(TEST_DIR) -name "*.cpp")
TEST_SOURCES := $(filter-out $(TEST_DIR)/main.cpp, $(TEST_SOURCES))
TEST_SOURCES := $(filter-out $(TEST_DIR)/codegen_output/%, $(TEST_SOURCES))
TEST_OBJECTS := $(patsubst $(TEST_DIR)%.cpp, $(BUILD)/tests%.o, $(TEST_SOURCES))


tests: $(OBJECTS) $(TEST_OBJECTS)
	$(CC) $(OBJECTS) $(TEST_OBJECTS) $(TEST_FLAGS) -o $(BUILD)/test_runner

# debug: $(OBJECTS) $(BUILD)/main.o
# 	$(CC) $(OBJECTS) $(BUILD)/main.o $(LDFLAGS)  -o $(BUILD)/$@

main: $(OBJECTS) $(BUILD)/main.o
	$(CC) $(OBJECTS) $(BUILD)/main.o $(LDFLAGS)  -o $(BUILD)/$@


$(BUILD)/%.o: $(SRC_DIR)/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/tests/%.o: $(TEST_DIR)/%.cpp
	$(CC) $(TEST_FLAGS) -c $< -o $@


clean:
	rm -rf $(BUILD)/
	# find $(BUILD)/ -name "*.o" -type f -delete
	# @ rm $(OBJECTS) $(BUILD)/main.o $(BUILD)/main
