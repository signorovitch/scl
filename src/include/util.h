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
// Bold while color code.
#define COL_BWHI "\e[1;37m"

// Start in indent block.
#define INDENT_BEGIN(ILVL)                                                     \
    int INDENT_lvl = ILVL;                                                     \
    Dstr* INDENT_spacing = dstr_init();                                        \
    for (int INDENT_j = 0; INDENT_j < ILVL; INDENT_j++)                        \
        dstr_appendch(INDENT_spacing, ' ');

// Print & indent the title of a section.
#define INDENT_TITLE(THING, WHERE)                                             \
    printf("%s" COL_BWHI THING " @ %p\n" COL_RESET, INDENT_spacing->buf, WHERE);

// Print & indent a thing.
#define INDENT_FIELD(FIELD, VAL, ...)                                          \
    printf("%s " FIELD ": " VAL "\n", INDENT_spacing->buf, __VA_ARGS__);

// Print & indent a thing with a newline before the val.
#define INDENT_FIELD_NL(FIELD, VAL, ...)                                       \
    printf("%s " FIELD ":\n %s " VAL "\n", INDENT_spacing->buf,                \
           INDENT_spacing->buf, __VA_ARGS__);

// Print & indent a thing without any newline.
#define INDENT_FIELD_NONL(FIELD) printf("%s " FIELD ": ", INDENT_spacing->buf);

// Print an array A of N things, by calling the function F.
#define INDENT_FIELD_LIST(FIELD, A, N, F)                                      \
    printf("%s " FIELD ": [\n", INDENT_spacing->buf);                          \
    for (int INDENT_i = 0; INDENT_i < N; INDENT_i++) {                         \
        F(A[INDENT_i], INDENT_lvl + 2);                                        \
        /*printf("%s \n", INDENT_spacing->buf); */                                 \
    }                                                                          \
    printf("%s ]\n", INDENT_spacing->buf);

// End an indent block.
#define INDENT_END dstr_destroy(INDENT_spacing);

// Allocate a pointer with a type.
#define talloc(T, X) T* X = malloc(sizeof(T));

#endif
