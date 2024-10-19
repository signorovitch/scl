#ifndef UTIL_H
#define UTIL_H

#ifdef DBG // Debug macros

// Log a message.
#define log_dbg(msg)                                                           \
    printf("\033[37;1mdbg\033[0m:\033[37m%s\033[0m:\033[32m " msg            \
           "\033[0m\n",                                                        \
           __func__);

// Log a message with formatting.
#define log_dbgf(msg, ...)                                                     \
    printf("\033[37;1mdbg\033[0m:\033[37m%s\033[0m:\033[32m " msg            \
           "\033[0m\n",                                                        \
           __func__, __VA_ARGS__);

#else // ifdef DBG
#define log_dbg(msg)
#endif // ifdef DBG else

// Maximum size of a string containing only an int.
#define MAXSTRINTSZ ((CHAR_BIT * sizeof(int) - 1) / 3 + 2)

// Maximum size of a string containing only a size_t.
#define MAXSTRIZE_TSZ ((CHAR_BIT * sizeof(size_t) - 1) / 3 + 2)

#endif
