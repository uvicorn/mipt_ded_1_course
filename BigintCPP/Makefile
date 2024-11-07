CC := g++
LD := g++

INCLUDE_DIR := include
SRC_DIR := src
EXE := main
BUILD := build

CFLAGS := -Wall -Wextra -Wpedantic  -I $(INCLUDE_DIR) -Isrc -fpermissive -DDEBUG -lstdc++ -D_FORTIFY_SOURCE=2 -Og -ggdb
# CFLAGS := -Isrc
# -fsanitize=address
LDFLAGS := $(CFLAGS)


# SOURCES := $(wildcard $(SRC_DIR)/*.cpp)
# SOURCES := $(filter-out $(SRC_DIR)/main.cpp, $(SOURCES))
# OBJECTS=$(SOURCES:$(SRC_DIR)/%.c=$(BUILD)/%.o)

SUB_DIRS := $(shell find $(SRC_DIR) -type d)
SUB_DIRS := $(filter-out $(SRC_DIR), $(SUB_DIRS))
BUILD_SUB_DIRS := $(patsubst $(SRC_DIR)/%, $(BUILD)/%, $(SUB_DIRS))

# INIT BUILD SUB DIRS
$(shell mkdir -p $(BUILD_SUB_DIRS))


SOURCES := $(shell find $(SRC_DIR) -name "*.cpp")
SOURCES := $(filter-out $(SRC_DIR)/main.cpp, $(SOURCES))
OBJECTS := $(patsubst $(SRC_DIR)%.cpp, $(BUILD)%.o, $(SOURCES))


# test:
# 	echo $(SOURCES)
# 	echo $(OBJECTS)
# 	echo $(BUILD_SUB_DIRS)

$(EXE): $(OBJECTS) $(BUILD)/main.o
	$(CC) $(OBJECTS) $(BUILD)/main.o $(LDFLAGS)  -o $(BUILD)/$@

$(BUILD)/%.o: $(SRC_DIR)/%.cpp #$(SOURCES)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm $(OBJECTS) $(BUILD)/main.o $(BUILD)/main
