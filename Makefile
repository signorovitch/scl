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

# Stupid things.
RESETCOLOR = \x1b[0m
WHITE = $(RESETCOLOR)\x1b[37m
WHITE_BOLD = $(RESETCOLOR)\x1b[37;1m

all: $(TARGET)

# Link to final binary.
$(TARGET): $(OBJ_FILES)
	@ echo -e "$(WHITE_BOLD)Linking $(WHITE)$(TARGET)$(WHITE_BOLD)...$(RESETCOLOR) $(CC) -o $(TARGET) $(OBJ_FILES) $(LDFLAGS)"
	@ $(LINK) -o $(TARGET) $(OBJ_FILES) $(LDFLAGS)

# Compile project sources.
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(SRC_DIR)/include/%.h
	@ mkdir -p $(OBJ_DIR)
	@ echo -e "$(WHITE_BOLD)Compiling $(WHITE)$<$(WHITE_BOLD)... $(RESETCOLOR)$(CC) $(CFLAGS) -c $< -o $@"
	@ $(CC) $(CFLAGS) -c $< -o $@

# Compile test sources.
$(TEST_OBJ_DIR)/%.o: $(TEST_DIR)/%.c
	@ mkdir -p $(TEST_OBJ_DIR)
	@ echo -e "$(WHITE_BOLD)Compiling Test $(WHITE)$<$(WHITE_BOLD)... $(WHITE)$(CC) $(CFLAGS) -I$(SRC_DIR)/include -c $< -o $@$(RESETCOLOR)"
	@ $(CC) $(CFLAGS) -I$(SRC_DIR)/include -c $< -o $@

# Link the test executable.
test: $(TEST_OBJ_FILES) $(OBJ_FILES_NOMAIN) $(UNITY_C)
	@ echo -e "$(WHITE_BOLD)Linking test binary$(WHITE)...$(RESETCOLOR)"
	@ $(LINK) -DUNITY_OUTPUT_COLOR $(TEST_OBJ_FILES) $(OBJ_FILES_NOMAIN) $(UNITY_C) -o $(TEST_BUILD_DIR)/test.out
	@ echo -e "$(WHITE_BOLD)Running tests$(WHITE)...$(RESETCOLOR)"
	@ ./$(TEST_BUILD_DIR)/test.out

clean:
	@ echo -e "$(WHITE_BOLD)Cleaning up...$(WHITE) $(OBJ_DIR)/*.o $(TEST_OBJ_DIR)/*.o $(TEST_BUILD_DIR)/test.out $(TARGET)$(RESETCOLOR)"
	@ rm -rf $(OBJ_DIR)/*.o $(TEST_OBJ_DIR)/*.o $(TEST_BUILD_DIR)/test.out $(TARGET)

.PHONY: all clean test nocolor

