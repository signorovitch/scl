include config.mk

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
	@ mkdir -p $(TEST_OBJ_DIR)
	@ $(PRINT) "$(WHITE_BOLD)Compiling test object $(WHITE)$@$(WHITE_BOLD)...$(RESETCOLOR)"
	$(CC) $(CFLAGS) -c $< -o $@

# Link final test binary.
$(TEST_BUILD_DIR)/test_%.out: $(TEST_OBJ_DIR)/test_%.o $(OBJ_DIR)/grammar.o $(OBJ_FILES_NOMAIN) $(UNITY_OBJ)
	@ mkdir -p $(TEST_BUILD_DIR)
	@ $(PRINT) "$(WHITE_BOLD)Linking test binary $(WHITE)$@$(WHITE_BOLD)...$(RESETCOLOR)"
	$(LINK) -o $@ $? $(LDFLAGS)

# Run the test files.
test: $(TARGET) $(TEST_BIN_FILES)
	@ $(PRINT) "$(WHITE_BOLD)Running unit tests...$(RESETCOLOR)"
	for test in $(TEST_BIN_FILES); do ./$${test} || echo -e "$(RED_BOLD) BAD EXIT ON $${test} $(RESETCOLOR)"; done
	@ $(PRINT) "$(WHITE_BOLD)Running validation tests...$(RESETCOLOR)"
	$(BATS) $(TEST_VAL_DIR)

# Clean out objects, binaries, and built artifacts.
clean:
	@ $(PRINT) "$(WHITE_BOLD)Cleaning up...$(RESETCOLOR)"
	rm -rf $(OBJ_DIR)/*.o $(TEST_OBJ_DIR)/*.o $(TEST_BUILD_DIR)/test.out $(TARGET) $(GRAM_DIR)/* $(UNITY_OBJ)

# Get LOC.
lines:
	@ wc -l $(SRC_FILES) $(INC_FILES) $(GRAM_SRC)

.PHONY: all clean test nocolor release run lines

# Run this intermediary even though make thinks it's useless.
.PRECIOUS: $(TEST_OBJ_FILES)
