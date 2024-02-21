/**
 *  Alex Carbajal
 *  CPT_S 360, Spring 2024
 *  whocan
 */

#include "helpers.h"

void print_err_file(const char *fsobj)
{
    fprintf(stderr, "%s: %s\n", fsobj, strerror(errno));
}

void print_invalid_action(const char *action)
{
    printf("ERROR: Invalid action '%s'\n\n", action);

    printf("Enter './whocan [ACTION] [FSOBJ]' ");
    printf("where ACTION is one of the below valid actions and FSOBJ is a filesystem object (file, directory, or device)\n\n");

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
