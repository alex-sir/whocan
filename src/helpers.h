/**
 *  Alex Carbajal
 *  CPT_S 360, Spring 2024
 *  whocan
 */

#ifndef HELPERS
#define HELPERS

#include <stdio.h>  // for printf(), fprintf()
#include <stdlib.h> // for exit()
#include <string.h> // for strerror()
#include <errno.h>  // for errno

extern void print_err_file(const char *fsobj);
/**
 * @brief inform the user of an invalid action and exit
 *
 * @param action string of the invalid action specified by the user
 */
extern void print_invalid_action(const char *action);

#endif
