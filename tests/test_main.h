#ifndef TEST_MAIN_H_
#define TEST_MAIN_H_
#include "vecMath.h"
#include <stdbool.h>
#include <stdio.h>

// total number of tests to run
#define NUM_TESTS 3

// define Color for Terminal output
#define COL_RED(s) "\033[0;31m" #s "\x1b[0m"
#define COL_GREEN(s) "\033[0;32m" #s "\x1b[0m"

#ifdef NOTEST
#define __TESTING__ 0
#else
#define __TESTING__ 1
#endif

#define CHECK_EQUAL_FLOAT(EXPECTED, ACTUAL)                                    \
  ({                                                                           \
    bool __equal = false;                                                      \
    do {                                                                       \
      if (__TESTING__) {                                                       \
        float expected = EXPECTED;                                             \
        float actual = ACTUAL;                                                 \
        __equal = (expected == actual);                                        \
        if (!__equal) {                                                        \
          fprintf(stderr, "%s, line %d: For '%s', expected %f, got %f\n",      \
                  __FILE__, __LINE__, #ACTUAL, expected, actual);              \
        }                                                                      \
      }                                                                        \
    } while (0);                                                               \
    __equal;                                                                   \
  })

bool test_math_fcross2(void);
bool test_math_fdot2(void);
bool test_math_fdist2(void);

#endif // TEST_MAIN_H_
