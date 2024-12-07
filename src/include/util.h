#ifndef UTIL_H
#define UTIL_H

#ifdef DBG // Debug macros

// Log a message.
#define log_dbg(msg)                                                           \
    printf("\033[37;1mdbg\033[0m:\033[37m%s\033[0m:\033[32m " msg "\033[0m\n", \
           __func__);

// Log a message with formatting.
#define log_dbgf(msg, ...)                                                     \
    printf("\033[37;1mdbg\033[0m:\033[37m%s\033[0m:\033[32m " msg "\033[0m\n", \
           __func__, __VA_ARGS__);

#else // ifdef DBG
#define log_dbg(msg)
#define log_dbgf(msg, ...)
#endif // ifdef DBG else

// Resent color code.
#define COL_RESET "\e[0m"

// Regular color codes.
#define COL_BLA "\e[0;30m"
#define COL_RED "\e[0;31m"
#define COL_GRE "\e[0;32m"
#define COL_YEL "\e[0;33m"
#define COL_BLU "\e[0;34m"
#define COL_MAG "\e[0;35m"
#define COL_CYA "\e[0;36m"
#define COL_WHI "\e[0;37m"

// Bold color codes.
#define COL_BBLA "\e[1;30m"
#define COL_BRED "\e[1;31m"
#define COL_BGRE "\e[1;32m"
#define COL_BYEL "\e[1;33m"
#define COL_BBLU "\e[1;34m"
#define COL_BMAG "\e[1;35m"
#define COL_BCYA "\e[1;36m"
#define COL_BWHI "\e[1;37m"

// Start in indent block.
#define INDENT_BEGIN(ILVL)                                                     \
    __attribute__((unused)) int INDENT_lvl = ILVL;                             \
    Dstr* INDENT_spacing = dstr_init();                                        \
    for (int INDENT_j = 0; INDENT_j < ILVL; INDENT_j++)                        \
        dstr_appendch(INDENT_spacing, ' ');

// Print & indent the title of a section.
#define INDENT_TITLE(THING, WHERE)                                             \
    printf("%s" COL_BCYA THING COL_RESET " @" COL_MAG " %p\n" COL_RESET, INDENT_spacing->buf, WHERE);

// Print & indent a thing.
#define INDENT_FIELD(FIELD, VAL, ...)                                          \
    printf("%s " COL_BWHI FIELD ": " COL_RESET COL_WHI VAL COL_RESET "\n",     \
           INDENT_spacing->buf, __VA_ARGS__);

// Print & indent a thing with a newline before the val.
#define INDENT_FIELD_NL(FIELD, VAL, ...)                                       \
    printf("%s " COL_BWHI FIELD ":" COL_RESET "\n %s " COL_WHI VAL COL_RESET   \
           "\n",                                                               \
           INDENT_spacing->buf, INDENT_spacing->buf, __VA_ARGS__);

// Print & indent a thing without any newline.
#define INDENT_FIELD_EXT_NONL_START(FIELD)                                         \
    printf("%s " COL_BWHI FIELD ":\n" COL_RESET COL_WHI, INDENT_spacing->buf);
#define INDENT_FIELD_NONL_END printf( "\n" COL_RESET);

// Print an array A of N things, by calling the function F.
#define INDENT_FIELD_LIST(FIELD, A, N, F)                                      \
    printf("%s " COL_BWHI FIELD ": [\n" COL_RESET, INDENT_spacing->buf);       \
    for (int INDENT_i = 0; INDENT_i < N; INDENT_i++) {                         \
        F(A[INDENT_i], INDENT_lvl + 2);                                        \
    }                                                                          \
    printf(COL_BWHI "%s ]\n" COL_RESET, INDENT_spacing->buf);

// End an indent block.
#define INDENT_END printf(COL_RESET); dstr_destroy(INDENT_spacing);

// Allocate a pointer with a type.
#define talloc(T, X) T* X = malloc(sizeof(T));

#endif
