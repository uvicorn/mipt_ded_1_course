CC=gcc
INCLUDE_DIR = include
CFLAGS=-Wall -Wextra -Wpedantic -lm  -I $(INCLUDE_DIR)
LDFLAGS=$(CFLAGS)
SRC_DIR=src
SOURCES=main.c io.c solver.c
OBJECTS1=$(SOURCES:.c=.o)
OBJECTS=$(OBJECTS1:src=build)
EXECUTABLE=main
BUILD=build
TEST_DIR=tests
# test:
# 	echo $(OBJECTS)

run: $(OBJECTS) $(EXECUTABLE)
	$(CC) $(addprefix $(BUILD)/,$(OBJECTS)) $(CFLAGS)  -o $(BUILD)/$(EXECUTABLE)

# test: $(OBJECTS) $(EXECUTABLE)
# 	gcc test.c

# test: test.o	
# 	gcc test.c -o proga_test.exe

#test.o: test.cpp
#	gcc test.c -c -o test.o 

$(EXECUTABLE):  $(OBJECTS) 
	$(CC) $(addprefix $(BUILD)/,$(OBJECTS)) $(LDFLAGS)  -o $(BUILD)/$@

# %.o: $(SRC_DIR)/%.c
# 	$(CC) $(CFLAGS) -c $< -o $(BUILD)/$@


$(BUILD)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $(BUILD)/$@


# main.o: main.c
# 	$(CC) $(CFLAGS) -c main.c

# io.o: io.c
# 	$(CC) $(CFLAGS) -c io.c
# solver.o: solver.c
# 	$(CC) $(CFLAGS) -c solver.c
# tests.o: tests.c
# 	$(CC) $(CFLAGS) tests.c
clean:
	rm $(BUILD)/*.o $(BUILD)/main $(BUILD)/*.so
