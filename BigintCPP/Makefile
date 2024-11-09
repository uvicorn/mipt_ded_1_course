CC := g++
LD := g++

INCLUDE_DIR := include
SRC_DIR := src
TEST_DIR := tests
BUILD := build

CFLAGS := -Wall -Wextra -Wpedantic  -I $(INCLUDE_DIR) -Isrc -fpermissive -DDEBUG -lstdc++ -D_FORTIFY_SOURCE=2 -Og -ggdb
DEBUG_FLAGS := -Wall -Wextra -Wpedantic  -I $(INCLUDE_DIR) -Isrc -fpermissive -DDEBUG -lstdc++ -D_FORTIFY_SOURCE=2 -Og -ggdb -fsanitize=address
TEST_FLAGS := $(CFLAGS) -lgtest -lgmock
LDFLAGS := $(CFLAGS)


SUB_DIRS := $(shell find $(SRC_DIR) -type d)
SUB_DIRS := $(filter-out $(SRC_DIR), $(SUB_DIRS))
BUILD_SUB_DIRS := $(patsubst $(SRC_DIR)/%, $(BUILD)/%, $(SUB_DIRS))

# INIT BUILD SUB DIRS
$(shell mkdir -p $(BUILD_SUB_DIRS))


SOURCES := $(shell find $(SRC_DIR) -name "*.cpp")
SOURCES := $(filter-out $(SRC_DIR)/main.cpp, $(SOURCES))
OBJECTS := $(patsubst $(SRC_DIR)%.cpp, $(BUILD)%.o, $(SOURCES))

TEST_SOURCES := $(shell find $(TEST_DIR) -name "*.cpp")
TEST_SOURCES := $(filter-out $(TEST_DIR)/main.cpp, $(TEST_SOURCES))
TEST_OBJECTS := $(patsubst $(TEST_DIR)%.cpp, $(BUILD)%.o, $(TEST_SOURCES))


tests: $(OBJECTS) $(TEST_OBJECTS)
	$(CC) $(OBJECTS) $(TEST_OBJECTS) $(TEST_FLAGS) -o $(BUILD)/$@

# debug: $(OBJECTS) $(BUILD)/main.o
# 	$(CC) $(OBJECTS) $(BUILD)/main.o $(LDFLAGS)  -o $(BUILD)/$@

main: $(OBJECTS) $(BUILD)/main.o
	$(CC) $(OBJECTS) $(BUILD)/main.o $(LDFLAGS)  -o $(BUILD)/$@


$(BUILD)/%.o: $(SRC_DIR)/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/%.o: $(TEST_DIR)/%.cpp
	$(CC) $(TEST_FLAGS) -c $< -o $@


clean:
	rm $(OBJECTS) $(BUILD)/main.o $(BUILD)/main
