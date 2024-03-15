#include "utils.h"

void cwdJoinPath(char* path, char* dest)
{
	char cwd[PATH_MAX];

	if (getcwd(cwd, sizeof(cwd)) != NULL) {
		strcat(cwd, path);
		strcpy(dest, cwd);
	} else {
		debug_print("Failed to get cwd ! \n");
		return;
	}
}

int cwdPathLenght(char* path)
{
	char cwd[PATH_MAX];

	if (getcwd(cwd, sizeof(cwd)) != NULL) {
		strcat(cwd, path);

		for (int i = 0; i < PATH_MAX; i++) {
			if (cwd[i] == 0) {
				return i;
			}
		}
	}

	debug_print("Failed to get cwd ! \n");
	return -1;
}

void ut_print2dArray(char** array, unsigned int width, unsigned int height)
{
	for (unsigned int j = 0; j < height; ++j) {
		for (unsigned int i = 0; i < width; ++i) {
			printf("%d ", (int)array[j][i]);
		}

		printf("\n");
	}
}
