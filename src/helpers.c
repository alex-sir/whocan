/**
 *  Alex Carbajal
 *  CPT_S 360, Spring 2024
 *  whocan
 */

#include "helpers.h"
#include "checkactions.h"

/**
 * @brief print an error message and exit the program
 */
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

void print_valid_users(char ***valid_users, int valid_users_count)
{
    for (size_t i = 0; i < valid_users_count; i++)
    {
        printf("%s\n", (*valid_users)[i]);
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
