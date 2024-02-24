/**
 *  Alex Carbajal
 *  CPT_S 360, Spring 2024
 *  whocan
 */

#ifndef HELPERS
#define HELPERS

#include <stdio.h>  // for printf(), fprintf()
#include <stdlib.h> // for exit(), free()
#include <string.h> // for strerror()
#include <errno.h>  // for errno

/**
 * @brief print an error message and exit the program
 */
extern void print_err_exit(void);
/**
 * @brief print a message to standard error informing about the most recent error that occurred on a file
 *
 * @param fsobj file that caused an error to occur
 */
extern void print_err_file(const char *fsobj);
/**
 * @brief inform the user of an invalid action and exit
 *
 * @param action string of the invalid action specified by the user
 */
extern void print_invalid_action(const char *action);
/**
 * @brief print all valid users found to be able to perform an action
 *
 * @param valid_users array of strings containing uids of valid users to be printed
 * @param valid_users_count number of valid users located in valid_users
 */
extern void print_valid_users(char ***valid_users, int valid_users_count);
/**
 * @brief frees all allocated memory for valid_users
 *
 * @param valid_users array of strings containing uids of valid users which will be freed
 * @param valid_users_count number of valid users located in valid_users
 */
extern void free_valid_users(char ***valid_users, int valid_users_count);

#endif
