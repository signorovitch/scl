#ifndef UTIL_H
#define UTIL_H

#ifdef DBG

#define log_dbg(msg)                                                           \
    printf("\033[37;1mdbg\033[0m:\033[37;5m%s\033[0m:\033[32m " msg            \
           "\033[0m\n",                                                        \
           __func__);

#define log_dbgf(msg, ...)                                                     \
    printf("\033[37;1mdbg\033[0m:\033[37;5m%s\033[0m:\033[32m " msg            \
           "\033[0m\n",                                                        \
           __func__, __VA_ARGS__);

#else
#define log_dbg(msg)
#endif

#endif
