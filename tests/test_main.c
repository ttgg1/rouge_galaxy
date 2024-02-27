#include "test_main.h"

int main(void) {
  printf("Running Tests ...\n\n");
  bool (*test_list[NUM_TESTS])(void) = {test_math_fcross2, test_math_fdot2,
                                        test_math_fdist2};

  bool hasPassed = false;
  unsigned int num_failed = 0;

  for (int i = 0; i < NUM_TESTS; ++i) {
    printf("TEST [%d/%d]: ", i + 1, NUM_TESTS);
    hasPassed = test_list[i]();
    printf("\t Result: %s \n", hasPassed ? COL_GREEN(PASSED) : COL_RED(FAILED));
    if (!hasPassed)
      ++num_failed;
  }
  if (num_failed == 0) {
    printf("\n%s\n", COL_GREEN(All Tests have passed !));
  } else {
    printf("\n%s %u out of %d Tests Failed \n",
           COL_RED(Some Tests have failed !), num_failed, NUM_TESTS);
  }

  return (int)num_failed;
}
