/**
 *  Alex Carbajal
 *  CPT_S 360, Spring 2024
 *  whocan
 */

#include "checkactions.h"
#include "helpers.h"

#include <stdio.h>    // for printf()
#include <stdlib.h>   // for malloc()
#include <string.h>   // for strcmp()
#include <sys/stat.h> // for struct stat
#include <unistd.h>   // for sysconf()
#include <limits.h>   // for UID_MAX

#define UID_MAX_SIZE sysconf(_SC_LOGIN_NAME_MAX)     // max size of a username
#define PATHNAME_MAX pathconf(".", _PC_PATH_MAX) + 1 // max size of a file pathname

int main(int argc, char *argv[])
{
    if (UID_MAX_SIZE == -1)
    {
        print_err_exit();
    }

    // invalid number of arguments
    if (argc != 3)
    {
        printf("Usage: ./whocan [ACTION] [FSOBJ]\n");
        exit(EXIT_FAILURE);
    }

    char *action = argv[1];
    char fsobj[PATHNAME_MAX];
    if (realpath(argv[2], fsobj) == NULL)
    {
        print_err_exit();
    }
    struct stat fsobj_info;
    int valid_users_count = 0, canEveryone = 0;
    char **valid_users = (char **)malloc(INIT_NUM_USERS * sizeof(char *));
    if (valid_users == NULL)
    {
        print_err_exit();
    }
    // allocate memory for the initial number of users that can be stored
    for (size_t i = 0; i < INIT_NUM_USERS; i++)
    {
        valid_users[i] = (char *)malloc(UID_MAX_SIZE * sizeof(char));
        if (valid_users[i] == NULL)
        {
            free_valid_users(&valid_users, i + 1); // free all previously allocated strings
            print_err_exit();
        }
    }

    if (strcmp(action, "cd") == 0) // directory
    {
        if (checkfsobj_dir(fsobj, &fsobj_info) == -1)
        {
            free_valid_users(&valid_users, INIT_NUM_USERS);
            exit(EXIT_FAILURE);
        }
        valid_users_count = checkcd(&fsobj_info, &valid_users, &canEveryone);
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
        free_valid_users(&valid_users, INIT_NUM_USERS);
        print_invalid_action(action);
    }

    if (canEveryone)
    {
        printf("(everyone)\n");
    }
    else
    {
        qsort(valid_users, valid_users_count, sizeof(char *), compare_users);
        print_valid_users(&valid_users, valid_users_count);
    }

    free_valid_users(&valid_users, INIT_NUM_USERS);
    return 0;
}
