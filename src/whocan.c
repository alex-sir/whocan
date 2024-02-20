/**
 *  Alex Carbajal
 *  CPT_S 360, Spring 2024
 *  whocan
 */

#include "checkactions.h"
#include "helpers.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
    // invalid arguments
    if (argc <= 2 || argc > 3)
    {
        printf("Usage: ./whocan [ACTION] [FSOBJ]\n");
        return 0;
    }

    // TODO: first check if fsobj is a valid file
    const char *action = argv[1], *fsobj = argv[2];
    if (strcmp(action, "cd") == 0) // directory
    {
        checkcd(fsobj);
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
        printInvalidAction(action);
    }

    return 0;
}
