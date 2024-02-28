/**
 *  Alex Carbajal
 *  CPT_S 360, Spring 2024
 *  whocan
 */

#include "checkactions.h"
#include "helpers.h"

#include <stdio.h>    // for printf()
#include <stdlib.h>   // for malloc(), qsort()
#include <string.h>   // for strcmp()
#include <sys/stat.h> // for struct stat
#include <unistd.h>   // for sysconf()
#include <limits.h>   // for UID_MAX

#define PATHNAME_MAX pathconf(".", _PC_PATH_MAX) + 1 // max size of a file pathname

int main(int argc, char *argv[])
{
    if (NAME_MAX == -1)
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
    int valid_users_count = 0, can_everyone = 0;
    char **valid_users = (char **)malloc(INIT_NUM_USERS * sizeof(char *));
    if (valid_users == NULL)
    {
        print_err_exit();
    }

    // actions can be performed on a directory, file, or device
    if (strcmp(action, "cd") == 0 || strcmp(action, "search") == 0) // directory
    {
        if (checkfsobj(fsobj, &fsobj_info) == -1 || checkfsobj_dir(fsobj, &fsobj_info) == -1)
        {
            fprintf(stderr, "%s: Not a directory\n", fsobj);
            free_valid_users(&valid_users, INIT_NUM_USERS);
            exit(EXIT_FAILURE);
        }
        valid_users_count = check_cd(&fsobj_info, &valid_users, &can_everyone);
    }
    else if (strcmp(action, "delete") == 0) // all
    {
        if (checkfsobj(fsobj, &fsobj_info) == -1 ||
            (checkfsobj_dir(fsobj, &fsobj_info) == -1 &&
             checkfsobj_file(fsobj, &fsobj_info) == -1 &&
             checkfsobj_device(fsobj, &fsobj_info) == -1))
        {
            fprintf(stderr, "%s: Not a directory, file, or device\n", fsobj);
            free_valid_users(&valid_users, INIT_NUM_USERS);
            exit(EXIT_FAILURE);
        }

        char *parentdir = dirname(fsobj);
        struct stat parentdir_info;
        if (stat(parentdir, &parentdir_info) == -1)
        {
            free_valid_users(&valid_users, INIT_NUM_USERS);
            print_err_file(fsobj);
        }
        valid_users_count = check_delete(&fsobj_info, &parentdir_info, &valid_users, &can_everyone);
    }
    else if (strcmp(action, "execute") == 0) // file
    {
        if (checkfsobj(fsobj, &fsobj_info) == -1 || checkfsobj_file(fsobj, &fsobj_info) == -1)
        {
            fprintf(stderr, "%s: Not a file\n", fsobj);
            free_valid_users(&valid_users, INIT_NUM_USERS);
            exit(EXIT_FAILURE);
        }
        valid_users_count = check_execute(&fsobj_info, &valid_users, &can_everyone);
    }
    else if (strcmp(action, "ls") == 0) // directory OR file, device
    {
        printf("ls\n");
    }
    else if (strcmp(action, "read") == 0) // all
    {
        printf("read\n");
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

    if (can_everyone)
    {
        printf("(everyone)\n");
    }
    else
    {
        qsort(valid_users, valid_users_count, sizeof(char *), compare_users);
        print_valid_users(&valid_users, valid_users_count);
    }

    if (valid_users_count % INIT_NUM_USERS == 0)
    {
        free_valid_users(&valid_users, INIT_NUM_USERS * (valid_users_count / INIT_NUM_USERS));
    }
    else
    {
        free_valid_users(&valid_users, INIT_NUM_USERS * (valid_users_count / INIT_NUM_USERS) + INIT_NUM_USERS);
    }

    return 0;
}
