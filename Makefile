NAME = scl

TARGET = $(NAME).out

SRC_DIR = src
BUILD_DIR = build
OBJ_DIR = $(BUILD_DIR)/obj
TEST_DIR = test
TEST_BUILD_DIR = $(BUILD_DIR)/test
TEST_OBJ_DIR = $(TEST_BUILD_DIR)/obj

CC = clang
LINK = clang
CFLAGS = -Wall
LDFLAGS =

SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC_FILES))
OBJ_FILES_NOMAIN = $(filter-out $(OBJ_DIR)/main.o, $(OBJ_FILES)) # Object files without main.c.
UNITY_C = $(TEST_DIR)/unity/unity.c
TEST_SRC_FILES = $(wildcard $(TEST_DIR)/*.c)
TEST_OBJ_FILES = $(patsubst $(TEST_DIR)/%.c, $(TEST_OBJ_DIR)/%.o, $(TEST_SRC_FILES))

all: $(TARGET)

# Link to final binary.
$(TARGET): $(OBJ_FILES)
	@ echo -e "\x1b[37;1mLinking \x1b[0m\x1b[37m$(TARGET)\x1b[37;1m...\x1b[0m\x1b[37m $(CC) -o $(TARGET) $(OBJ_FILES) $(LDFLAGS)\x1b[0m"
	@ $(LINK) -o $(TARGET) $(OBJ_FILES) $(LDFLAGS)

# Compile project sources.
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(SRC_DIR)/include/%.h
	@ mkdir -p $(OBJ_DIR)
	@ echo -e "\x1b[37;1mCompiling \x1b[0m\x1b[37m$<\x1b[37;1m... \x1b[0m\x1b[37m$(CC) $(CFLAGS) -c $< -o $@\x1b[0m"
	@ $(CC) $(CFLAGS) -c $< -o $@

# Compile test sources.
$(TEST_OBJ_DIR)/%.o: $(TEST_DIR)/%.c
	@ mkdir -p $(TEST_OBJ_DIR)
	@ echo -e "\x1b[37;1mCompiling Test \x1b[0m\x1b[37m$<\x1b[37;1m... \x1b[0m\x1b[37m$(CC) $(CFLAGS) -I$(SRC_DIR)/include -c $< -o $@\x1b[0m"
	@ $(CC) $(CFLAGS) -I$(SRC_DIR)/include -c $< -o $@

# Link the test executable.
test: $(TEST_OBJ_FILES) $(OBJ_FILES_NOMAIN) $(UNITY_C)
	@ echo -e "\x1b[37;1mLinking test binary\x1b[0m\x1b[37m...\x1b[0m"
	@ $(LINK) $(TEST_OBJ_FILES) $(OBJ_FILES_NOMAIN) $(UNITY_C) -o $(TEST_BUILD_DIR)/test.out
	@ echo -e "\x1b[37;1mRunning tests\x1b[0m\x1b[37m...\x1b[0m"
	@ ./$(TEST_BUILD_DIR)/test.out

clean:
	@ echo -e "\x1b[37;1mCleaning up...\x1b[0m\x1b[37m $(OBJ_DIR)/*.o $(TEST_OBJ_DIR)/*.o $(TEST_BUILD_DIR)/test.out $(TARGET)\x1b[0m"
	@ rm -rf $(OBJ_DIR)/*.o $(TEST_OBJ_DIR)/*.o $(TEST_BUILD_DIR)/test.out $(TARGET)

.PHONY: all clean test

