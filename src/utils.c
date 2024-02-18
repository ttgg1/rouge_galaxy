#include "utils.h"

char *cwd_join_path(char *path){
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) != NULL)
    {
        strcat(cwd, path);
        for (int i = 0; i < PATH_MAX; i++)
        {
            if (cwd[i] == 0)
            {
                char path[i];
                strcpy(path, cwd);
                return path;
            }
        }
    }
    else
    {
        debug_print("Failed to get cwd ! \n");
        return "";
    }
}