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
    if (PATHNAME_MAX == -1 || NAME_MAX == -1) // can't get max sizes
    {
        print_err_exit();
    }

    if (argc != 3) // invalid number of arguments
    {
        printf("Usage: ./whocan [ACTION] [FSOBJ]\n");
        exit(EXIT_FAILURE);
    }

    // action & fsobj specified in the command line
    char *action = argv[1];
    char fsobj[PATHNAME_MAX];
    if (realpath(argv[2], fsobj) == NULL)
    {
        print_err_exit();
    }
    struct stat fsobj_info;

    // array of string that will hold the valid users
    int valid_users_count = 0, can_everyone = 0;
    char **valid_users = (char **)malloc(INIT_NUM_USERS * sizeof(char *));
    if (valid_users == NULL)
    {
        print_err_exit();
    }

    // check if the filesystem object is valid
    int is_valid_fsobj = 0, is_dir = 0, is_file = 0, is_dev = 0;
    is_valid_fsobj = checkfsobj(fsobj, &fsobj_info) == 1;
    if (!is_valid_fsobj)
    {
        fprintf(stderr, "%s: Not a valid filesystem object\n", fsobj);
        free_valid_users(&valid_users, valid_users_count);
        exit(EXIT_FAILURE);
    }

    // parent directory of the filesystem object
    char temp_parentdir[PATHNAME_MAX];
    strncpy(temp_parentdir, fsobj, PATHNAME_MAX);
    char *parentdir = dirname(temp_parentdir);
    struct stat parentdir_info;
    if (stat(parentdir, &parentdir_info) == -1)
    {
        free_valid_users(&valid_users, valid_users_count);
        print_err_file(fsobj);
    }

    /************************************************** ACTIONS BEGIN **************************************************/

    // actions can be performed on a directory, file, or device
    if (strcmp(action, "cd") == 0 || strcmp(action, "search") == 0) // directory
    {
        is_dir = checkfsobj_dir(fsobj, &fsobj_info) == 1;
        if (!is_dir)
        {
            fprintf(stderr, "%s: Not a directory\n", fsobj);
            free_valid_users(&valid_users, valid_users_count);
            exit(EXIT_FAILURE);
        }

        valid_users_count = check_cd_search(&fsobj_info, &valid_users, &can_everyone);
    }
    else if (strcmp(action, "delete") == 0) // all
    {
        is_dir = checkfsobj_dir(fsobj, &fsobj_info) == 1;
        is_file = checkfsobj_file(fsobj, &fsobj_info) == 1;
        is_dev = checkfsobj_dev(fsobj, &fsobj_info) == 1;
        if (!is_dir && !is_file && !is_dev)
        {
            fprintf(stderr, "%s: Not a directory, file, or device\n", fsobj);
            free_valid_users(&valid_users, valid_users_count);
            exit(EXIT_FAILURE);
        }

        valid_users_count = check_delete(&fsobj_info, &parentdir_info, &valid_users, &can_everyone);
    }
    else if (strcmp(action, "execute") == 0) // file
    {
        is_file = checkfsobj_file(fsobj, &fsobj_info) == 1;
        if (!is_file)
        {
            fprintf(stderr, "%s: Not a file\n", fsobj);
            free_valid_users(&valid_users, valid_users_count);
            exit(EXIT_FAILURE);
        }

        valid_users_count = check_execute(&fsobj_info, &valid_users, &can_everyone);
    }
    else if (strcmp(action, "ls") == 0) // directory OR file, device (all, but different permissions)
    {
        is_dir = checkfsobj_dir(fsobj, &fsobj_info) == 1;
        is_file = checkfsobj_file(fsobj, &fsobj_info) == 1;
        is_dev = checkfsobj_dev(fsobj, &fsobj_info) == 1;
        if (!is_dir && !is_file && !is_dev)
        {
            fprintf(stderr, "%s: Not a directory, file, or device\n", fsobj);
            free_valid_users(&valid_users, valid_users_count);
            exit(EXIT_FAILURE);
        }

        if (is_dir) // directory
        {
            valid_users_count = check_ls_dir_read(&fsobj_info, &valid_users, &can_everyone);
        }
        else if (is_file || is_dev) // file, device
        {
            valid_users_count = check_ls_file_dev(&parentdir_info, &valid_users, &can_everyone);
        }
    }
    else if (strcmp(action, "read") == 0) // all
    {
        is_dir = checkfsobj_dir(fsobj, &fsobj_info) == 1;
        is_file = checkfsobj_file(fsobj, &fsobj_info) == 1;
        is_dev = checkfsobj_dev(fsobj, &fsobj_info) == 1;
        if (!is_dir && !is_file && !is_dev)
        {
            fprintf(stderr, "%s: Not a directory, file, or device\n", fsobj);
            free_valid_users(&valid_users, valid_users_count);
            exit(EXIT_FAILURE);
        }

        valid_users_count = check_ls_dir_read(&fsobj_info, &valid_users, &can_everyone);
    }
    else if (strcmp(action, "write") == 0) // directory OR file, device (all, but different permissions)
    {
        is_dir = checkfsobj_dir(fsobj, &fsobj_info) == 1;
        is_file = checkfsobj_file(fsobj, &fsobj_info) == 1;
        is_dev = checkfsobj_dev(fsobj, &fsobj_info) == 1;
        if (!is_dir && !is_file && !is_dev)
        {
            fprintf(stderr, "%s: Not a directory, file, or device\n", fsobj);
            free_valid_users(&valid_users, valid_users_count);
            exit(EXIT_FAILURE);
        }

        if (is_dir) // directory
        {
            valid_users_count = check_write_dir(&fsobj_info, &valid_users, &can_everyone);
        }
        else if (is_file || is_dev) // file, device
        {
            valid_users_count = check_write_file_dev(&fsobj_info, &valid_users, &can_everyone);
        }
    }
    else
    {
        free_valid_users(&valid_users, valid_users_count);
        print_invalid_action(action);
    }

    /************************************************** ACTIONS END **************************************************/

    if (can_everyone) // every single user in the system can perform the action
    {
        printf("(everyone)\n");
    }
    else // sort and print the valid users in ascending ASCII order
    {
        qsort(valid_users, valid_users_count, sizeof(char *), compare_users);
        print_valid_users(&valid_users, valid_users_count);
    }

    free_valid_users(&valid_users, valid_users_count);
    return 0;
}
