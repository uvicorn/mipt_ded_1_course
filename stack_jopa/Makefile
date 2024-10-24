CC=g++
INCLUDE_DIR = include
# CFLAGS = -lm -I $(INCLUDE_DIR)
CFLAGS=-Wall -Wextra -Wpedantic  -I $(INCLUDE_DIR) -fpermissive -O2 -DDEBUG -lgtest -lstdc++ -D_FORTIFY_SOURCE=2
#-fsanitize=address -Og -DDEBUG -ggdb -D_FORTIFY_SOURCE=2 -lgtest -fpermissive -lstdc++
TEST_CFLAGS=-Wall -Wextra -Wpedantic -lm  -I $(INCLUDE_DIR) -fpermissive -O2 -DDEBUG
LDFLAGS=$(CFLAGS)
TEST_BUILD=build_tests
SRC_DIR=src
EXECUTABLE=main
TEST_EXECUTABLE=testrunner
BUILD=build
TEST_DIR=tests

SOURCES :=$(wildcard $(SRC_DIR)/*.c) #main.c archive.c bitstream.c utils.c output.c
SOURCES := $(filter-out $(SRC_DIR)/main.c, $(SOURCES))
OBJECTS=$(SOURCES:$(SRC_DIR)/%.c=$(BUILD)/%.o)

# TESTS=$(TEST_DIR)/runner.cpp
TESTS=$(wildcard $(TEST_DIR)/*.cpp)
TEST_OBJECTS=$(TESTS:$(TEST_DIR)/%.cpp=$(BUILD)/%.o)

# run: $(BUILD)/main.o $(OBJECTS) $(EXECUTABLE)
# 	$(CC) $(OBJECTS) $(BUILD)/main.o $(CFLAGS) -o $(BUILD)/$(EXECUTABLE)

# test: $(OBJECTS) $(TEST_OBJECTS) $(TEST_EXECUTABLE) 
# 	$(CC) $(TEST_OBJECTS) $(TEST_CFLAGS) -o $(TEST_BUILD)/$(TEST_EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) $(BUILD)/main.o
	$(CC) $(OBJECTS) $(BUILD)/main.o $(LDFLAGS)  -o $(BUILD)/$@

$(TEST_EXECUTABLE): $(OBJECTS) $(TEST_OBJECTS)
	$(CC) $(OBJECTS) $(TEST_OBJECTS) $(LDFLAGS)  -o $(BUILD)/$@

# %.o: $(SRC_DIR)/%.c
#       $(CC) $(CFLAGS) -c $< -o $(BUILD)/$@


$(BUILD)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/%.o: $(TEST_DIR)/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

# $(TEST_BUILD)/%.o: 
# 	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm $(BUILD)/* #$(TEST_BUILD)/*
