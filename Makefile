NAME = scl

TARGET = $(NAME).out

SRC_DIR = src
INC_DIR = $(SRC_DIR)/include
BUILD_DIR = build
OBJ_DIR = $(BUILD_DIR)/obj
GRAM_DIR = $(BUILD_DIR)/grammars
TEST_DIR = test
TEST_BUILD_DIR = $(BUILD_DIR)/test
TEST_OBJ_DIR = $(TEST_BUILD_DIR)/obj

CC = clang
LINK = clang
CFLAGS = -Wall -DDBG -ggdb
LDFLAGS = -lm
BATS = bats
BISON = bison
PRINT = echo -e

SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC_FILES))
OBJ_FILES_NOMAIN = $(filter-out $(OBJ_DIR)/main.o, $(OBJ_FILES)) # Object files without main.c.
GRAM_FILES = $(GRAM_DIR)/grammar.tab.c $(GRAM_DIR)/grammar.tab.h
UNITY_DIR = $(TEST_DIR)/Unity
UNITY_C = $(UNITY_DIR)/src/unity.c
UNITY_H = $(UNITY_DIR)/src/unity.h
UNITY_OBJ = $(TEST_BUILD_DIR)/unity.o
TEST_SRC_FILES = $(wildcard $(TEST_DIR)/*.c)
TEST_OBJ_FILES = $(patsubst $(TEST_DIR)/%.c, $(TEST_OBJ_DIR)/%.o, $(TEST_SRC_FILES))
TEST_BIN_FILES = $(patsubst $(TEST_DIR)/%.c, $(TEST_BUILD_DIR)/%.out, $(TEST_SRC_FILES))
TEST_VAL_DIR = $(TEST_DIR)/validation

RESETCOLOR = \033[0m
WHITE = $(RESETCOLOR)\033[37m
WHITE_BOLD = $(RESETCOLOR)\033[37;1m

all: $(TARGET)

release: clean
release: CFLAGS = -Wall -O2
release: $(TARGET)

# Run the target.
run: $(TARGET)
	./$(TARGET)

# Generate grammars with bison.
$(GRAM_FILES): $(SRC_DIR)/grammar.y
	@ mkdir -p $(GRAM_DIR)
	@ $(PRINT) "$(WHITE_BOLD)Generating grammars...$(RESETCOLOR)"
	$(BISON) $< -o$(GRAM_DIR)/grammar.tab.c -H$(GRAM_DIR)/grammar.tab.h

# Compile grammars.
$(OBJ_DIR)/grammar.o: $(GRAM_DIR)/grammar.tab.c $(GRAM_DIR)/grammar.tab.h $(OBJ_DIR)/lexer.o
	@ $(PRINT) "$(WHITE_BOLD)Compiling grammars...$(RESETCOLOR)"
	$(CC) $(CFLAGS) -c $< -o $@

# Lexer depends on grammars.
$(OBJ_DIR)/lexer.o: $(SRC_DIR)/lexer.c $(GRAM_FILES)
	@ mkdir -p $(OBJ_DIR)
	@ $(PRINT) "$(WHITE_BOLD)Compiling source object $(WHITE)$@$(WHITE_BOLD)... $(RESETCOLOR)"
	$(CC) $(CFLAGS) -c $< -o $@

# Compile project source objects.
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(INC_DIR)/%.h
	@ mkdir -p $(OBJ_DIR)
	@ $(PRINT) "$(WHITE_BOLD)Compiling source object $(WHITE)$@$(WHITE_BOLD)... $(RESETCOLOR)"
	$(CC) $(CFLAGS) -c $< -o $@

# Link to final binary.
$(TARGET): $(OBJ_DIR)/grammar.o $(OBJ_FILES)
	@ $(PRINT) "$(WHITE_BOLD)Linking $(WHITE)$@$(WHITE_BOLD)...$(RESETCOLOR)"
	$(LINK) -o $(TARGET) $(OBJ_FILES) $(OBJ_DIR)/grammar.o $(LDFLAGS)

# Compile Unity object.
$(UNITY_OBJ): $(UNITY_C) $(UNITY_H)
	@ $(PRINT) "$(WHITE_BOLD)Compiling Unity...$(RESETCOLOR)"
	$(CC) $(CFLAGS) -D UNITY_OUTPUT_COLOR -c $< -o $@

# Compile test object.
$(TEST_OBJ_DIR)/test_%.o: $(TEST_DIR)/test_%.c
	@ $(PRINT) "$(WHITE_BOLD)Compiling test object $(WHITE)$@$(WHITE_BOLD)...$(RESETCOLOR)"
	$(CC) $(CFLAGS) -c $< -o $@

# Link final test binary.
$(TEST_BUILD_DIR)/test_%.out: $(TEST_OBJ_DIR)/test_%.o $(OBJ_DIR)/%.o $(UNITY_OBJ)
	@ $(PRINT) "$(WHITE_BOLD)Linking test binary $(WHITE)$@$(WHITE_BOLD)...$(RESETCOLOR)"
	$(LINK) -o $@ $? $(LDFLAGS)

# Run the test files.
test: $(TARGET) $(TEST_BIN_FILES)
	mkdir -p $(TEST_BUILD_DIR)
	@ $(PRINT) "$(WHITE_BOLD)Running unit tests...$(RESETCOLOR)"
	for test in $(TEST_BIN_FILES); do ./$${test}; done
	@ $(PRINT) "$(WHITE_BOLD)Running validation tests...$(RESETCOLOR)"
	$(BATS) $(TEST_VAL_DIR)


clean:
	@ $(PRINT) "$(WHITE_BOLD)Cleaning up...$(RESETCOLOR)"
	rm -rf $(OBJ_DIR)/*.o $(TEST_OBJ_DIR)/*.o $(TEST_BUILD_DIR)/test.out $(TARGET) $(GRAM_DIR)/* $(UNITY_OBJ)

.PHONY: all clean test nocolor release run
.PRECIOUS: $(TEST_OBJ_FILES)
