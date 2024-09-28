NAME = scl

CC = gcc
CFLAGS_REG = -Wall -O2
CFLAGS_DBG = -ggdb -fsanitize=address -O0
LDFLAGS = -lm

SRC_DIR = src
OBJ_DIR = obj
TARGET = $(NAME).out

SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC_FILES))

COL_BOLD = \x1b[37;1m
COL_CLEAR = \x1b[0m

all: reg

reg: CFLAGS = $(CFLAGS_REG)
reg: $(TARGET)

dbg: CFLAGS = $(CFLAGS_DBG)
dbg: clean
dbg: $(TARGET)

msg_compiling:
	@ echo -e "$(COL_BOLD)Compiling...$(COL_CLEAR)"

msg_linking:
	@ echo -e "$(COL_BOLD)Linking...$(COL_CLEAR)"

msg_cleaning:
	@ echo -e "$(COL_BOLD)Cleaning up...$(COL_CLEAR)"

$(TARGET): msg_compiling $(OBJ_FILES) msg_linking
	$(CC) $(LDFLAGS) $(OBJ_FILES) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@ mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean: msg_cleaning
	rm -rf $(OBJ_DIR) $(TARGET) $(TARGET_DBG)

.PHONY: all clean reg dbg msg_compiling msg_linking msg_cleaning
