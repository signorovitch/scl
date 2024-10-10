#ifndef UTIL_H
#define UTIL_H

#ifdef DBG
#define log_dbg(msg) \
    printf("[dbg:%s:%s:%d] %s\n", __FILE__, __func__, __LINE__, msg); \
    fflush(stdout);
#else
#define log_dbg(msg)
#endif

#endif
