/**
 *  Alex Carbajal
 *  CPT_S 360, Spring 2024
 *  whocan
 */

#include "helpers.h"
#include "checkactions.h"

void print_err_exit(void)
{
    fprintf(stderr, "%s\n", strerror(errno));
    exit(EXIT_FAILURE);
}

void print_err_file(const char *fsobj)
{
    fprintf(stderr, "%s: %s\n", fsobj, strerror(errno));
}

void print_invalid_action(const char *action)
{
    printf("ERROR: Invalid action \"%s\"\n\n", action);

    printf("Enter \"./whocan [ACTION] [FSOBJ]\" ");
    printf("where ACTION is one of the below valid actions and FSOBJ is a filesystem object (file, directory, or device).\n\n");

    printf("VALID ACTIONS:\n");
    printf("cd (directory)\n");
    printf("delete (all)\n");
    printf("execute (file)\n");
    printf("ls (directory, file, device)\n");
    printf("read (all)\n");
    printf("search (directory)\n");
    printf("write (directory, file, device)\n");

    exit(EXIT_FAILURE);
}

void add_valid_users_entry(char ***valid_users, int valid_users_count)
{
    // valid_users full, allocate more memory
    if (valid_users_count != 0 && valid_users_count % INIT_NUM_USERS == 0)
    {
        realloc_valid_users(valid_users, valid_users_count);
    }

    // allocate memory for the valid user username string
    (*valid_users)[valid_users_count] = (char *)malloc(NAME_MAX);
    if ((*valid_users)[valid_users_count] == NULL)
    {
        free_valid_users(valid_users, valid_users_count);
        print_err_exit();
    }
}

void realloc_valid_users(char ***valid_users, int valid_users_count)
{
    *valid_users = (char **)realloc(*valid_users, (valid_users_count + INIT_NUM_USERS) * sizeof(char *));
    if (*valid_users == NULL)
    {
        free_valid_users(valid_users, valid_users_count);
        print_err_exit();
    }
}

void free_valid_users(char ***valid_users, int valid_users_count)
{
    for (size_t i = 0; i < valid_users_count; i++)
    {
        free((*valid_users)[i]);
    }
    free((*valid_users));
}


void print_valid_users(char ***valid_users, int valid_users_count)
{
    for (size_t i = 0; i < valid_users_count; i++)
    {
        printf("%s\n", (*valid_users)[i]);
    }
}

int compare_users(const void *user1, const void *user2)
{
    return strcmp(*(const char **)user1, *(const char **)user2);
}
