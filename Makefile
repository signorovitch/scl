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
LDFLAGS =

SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC_FILES))
OBJ_FILES_NOMAIN = $(filter-out $(OBJ_DIR)/main.o, $(OBJ_FILES)) # Object files without main.c.
UNITY_C = $(TEST_DIR)/unity/unity.c
TEST_SRC_FILES = $(wildcard $(TEST_DIR)/*.c)
TEST_OBJ_FILES = $(patsubst $(TEST_DIR)/%.c, $(TEST_OBJ_DIR)/%.o, $(TEST_SRC_FILES))

# Stupid things.
RESETCOLOR = \033[0m
WHITE = $(RESETCOLOR)\033[37m
WHITE_BOLD = $(RESETCOLOR)\033[37;1m

all: $(TARGET)

release: clean
release: CFLAGS = -Wall -O2
release: $(TARGET)

run: $(TARGET)
	@ echo -e "$(WHITE_BOLD)Running... $(RESETCOLOR)./$(TARGET)"
	@ ./$(TARGET)

# Generate grammars with bison.
grammars: $(SRC_DIR)/grammar.y
	@ mkdir -p $(GRAM_DIR)
	@ echo -e "$(WHITE_BOLD)Generating grammars...$(RESETCOLOR) bison $< -o$(GRAM_DIR)/grammar.tab.c -H$(GRAM_DIR)/grammar.tab.h"
	@ bison $< -o$(GRAM_DIR)/grammar.tab.c -H$(GRAM_DIR)/grammar.tab.h

# Link to final binary.
$(TARGET): $(OBJ_FILES)
	@ echo -e "$(WHITE_BOLD)Linking $(WHITE)$(TARGET)$(WHITE_BOLD)...$(RESETCOLOR) $(CC) -o $(TARGET) $(OBJ_FILES) $(LDFLAGS)"
	@ $(LINK) -o $(TARGET) $(OBJ_FILES) $(LDFLAGS)

# Lexer depends on grammars.
$(OBJ_DIR)/lexer.o: grammars

# Compile project sources.
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(INC_DIR)/%.h
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

.PHONY: all clean test nocolor release run

