NAME = scl

CC = clang
CFLAGS = -Wall --std=c11
LDFLAGS = 

SRC_DIR = src
OBJ_DIR = obj
TARGET = $(NAME).out

SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC_FILES))

all: $(TARGET)

$(TARGET): $(OBJ_FILES)
	@ echo -e "\x1b[32;1mLinking \x1b[0m\x1b[32m$(TARGET)\x1b[32;1m...\x1b[0m\x1b[37m $(CC) -o $(TARGET) $(OBJ_FILES) $(LDFLAGS)\x1b[0m"
	@ $(CC) -o $(TARGET) $(OBJ_FILES) $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(SRC_DIR)/include/%.h
	@ mkdir -p $(OBJ_DIR)
	@ echo -e "\x1b[32;1mCompiling \x1b[0m\x1b[32m$<\x1b[32;1m... \x1b[0m\x1b[37m$(CC) $(CFLAGS) -c $< -o $@\x1b[0m"
	@ $(CC) $(CFLAGS) -c $< -o $@

clean:
	@ echo -e "\x1b[32;1mCleaning up...\x1b[0m"
	@ rm -rf $(OBJ_DIR) $(TARGET)

.PHONY: all clean
