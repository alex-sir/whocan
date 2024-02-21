/**
 *  Alex Carbajal
 *  CPT_S 360, Spring 2024
 *  whocan
 */

#include "checkactions.h"
#include "helpers.h"

#include <stdio.h>    // for printf()
#include <string.h>   // for strcmp()
#include <sys/stat.h> // for struct stat

int main(int argc, char const *argv[])
{
    // invalid arguments
    if (argc != 3)
    {
        printf("Usage: ./whocan [ACTION] [FSOBJ]\n");
        return 0;
    }

    const char *action = argv[1], *fsobj = argv[2];
    struct stat fsobj_info;
    if (strcmp(action, "cd") == 0) // directory
    {
        checkfsobj_dir(fsobj, &fsobj_info);
        checkcd(&fsobj_info);
    }
    else if (strcmp(action, "delete") == 0) // all
    {
        printf("delete\n");
    }
    else if (strcmp(action, "execute") == 0) // file
    {
        printf("execute\n");
    }
    else if (strcmp(action, "ls") == 0) // directory OR file, device
    {
        printf("ls\n");
    }
    else if (strcmp(action, "read") == 0) // all
    {
        printf("read\n");
    }
    else if (strcmp(action, "search") == 0) // directory
    {
        printf("search\n");
    }
    else if (strcmp(action, "write") == 0) // directory OR file, device
    {
        printf("write\n");
    }
    else
    {
        print_invalid_action(action);
    }

    return 0;
}
