#ifndef UTILS_H_
#define UTILS_H_

// always use DEBUG mode -> remove on release
#ifndef DEBUG
#define DEBUG 1
#endif

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define create2dArray(__w, __h, __type_t)                                      \
  ({                                                                           \
    __type_t **__arr;                                                          \
    do {                                                                       \
      __arr = (__type_t **)malloc(__h * sizeof(__type_t *));                   \
      for (int __i = 0; __i < __h; ++__i) {                                    \
        __arr[__i] = malloc(__w * sizeof(__type_t));                           \
      }                                                                        \
    } while (0);                                                               \
    __arr;                                                                     \
  })

#define free2dArray(__h, __arr)                                                \
  ({                                                                           \
    do {                                                                       \
      for (int __i = 0; __i < __h; ++__i) {                                    \
        free(__arr[__i]);                                                      \
      }                                                                        \
      free(__arr);                                                             \
    } while (0);                                                               \
  })

#define debug_print(fmt, ...)                                                  \
  do {                                                                         \
    if (DEBUG)                                                                 \
      fprintf(stderr, "%s:%d:%s(): " fmt, __FILE__, __LINE__, __func__,        \
              ##__VA_ARGS__);                                                  \
  } while (0)

void cwdJoinPath(char *path, char *dest);

int cwdPathLenght(char *path);

void ut_print2dArray(char **array, unsigned int width, unsigned int height);

#endif // UTILS_H_
