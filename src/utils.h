#ifndef UTILS_H_
#define UTILS_H_

// always use DEBUG mode -> remove on release
#ifndef DEBUG
#define DEBUG 1
#endif

#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#define debug_print(fmt, ...)                                                  \
  do {                                                                         \
    if (DEBUG)                                                                 \
      fprintf(stderr, "%s:%d:%s(): " fmt, __FILE__, __LINE__, __func__,        \
              ##__VA_ARGS__);                                                  \
  } while (0)

char *cwdJoinPath(char *path);

#endif // UTILS_H_
