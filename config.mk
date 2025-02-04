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
CFLAGS = -Wall -DDBG -ggdb -fsanitize=leak
LDFLAGS = -lm
BATS = bats
BISON = bison
PRINT = echo -e

SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
INC_FILES = $(wildcard $(INC_DIR)/*.h)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC_FILES))
OBJ_FILES_NOMAIN = $(filter-out $(OBJ_DIR)/main.o, $(OBJ_FILES)) # Object files without main.c.
GRAM_SRC = $(SRC_DIR)/grammar.y
GRAM_FILES = $(GRAM_DIR)/grammar.tab.c $(GRAM_DIR)/grammar.tab.h
UNITY_DIR = $(TEST_DIR)/Unity
UNITY_C = $(UNITY_DIR)/src/unity.c
UNITY_H = $(UNITY_DIR)/src/unity.h
UNITY_OBJ = $(TEST_BUILD_DIR)/unity.o
TEST_SRC_FILES = $(wildcard $(TEST_DIR)/*.c)
TEST_OBJ_FILES = $(patsubst $(TEST_DIR)/%.c, $(TEST_OBJ_DIR)/%.o, $(TEST_SRC_FILES))
TEST_BIN_FILES = $(patsubst $(TEST_DIR)/%.c, $(TEST_BUILD_DIR)/%.out, $(TEST_SRC_FILES))
TEST_VAL_DIR = $(TEST_DIR)/val

RESETCOLOR = \033[0m
WHITE = $(RESETCOLOR)\033[37m
WHITE_BOLD = $(RESETCOLOR)\033[37;1m
RED_BOLD = $(RESETCOLOR)\033[31;1m
