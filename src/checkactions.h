/**
 *  Alex Carbajal
 *  CPT_S 360, Spring 2024
 *  whocan
 */

#ifndef CHECKACTIONS
#define CHECKACTIONS

#include <stdio.h>     // for printf()
#include <stdlib.h>    // for exit()
#include <unistd.h>    // for stat(), chdir(), access()
#include <sys/stat.h>  // for struct stat
#include <pwd.h>       // for getpwent(), setpwent(), endpwent()
#include <grp.h>       // for getgrent(), setgrent(), endgrent(), getgrouplist()
#include <sys/types.h> // for gid_t

#define INIT_NUM_USERS 50
// 7 rwx combinations
#define PBITS_R 4
#define PBITS_W 2
#define PBITS_X 1
#define PBITS_RW 6
#define PBITS_RX 5
#define PBITS_WX 3
#define PBITS_RWX 7

/**
 * @brief check if a filesystem object is a directory; if not, exit the program
 *
 * @param fsobj pathname to a filesystem object
 * @param fsobj_info structure that will be filled with information about fsobj if it is a directory
 * @return int 1: fsobj is a valid directory |
 *            -1: fsobj is not a valid directory or is not a directory
 */
extern int checkfsobj_dir(const char *fsobj, struct stat *fsobj_info);
/**
 * @brief print a list of which users have permissions to "cd" into a directory
 *
 * @param fsobj_info structure containing information about a directory
 * @param valid_users array of strings that will be filled with the uid of users who can "cd"
 * @param canEveryone will indicate if ALL users can "cd"
 * @return int number of users that can "cd"
 */
extern int checkcd(struct stat *fsobj_info, char ***valid_users, int *canEveryone);
/**
 * @brief check "user" permission bits for a user: read (r), write (w), or execute (x)
 *
 * @param pw_entry password file entry containing information about a user
 * @param fsobj_info structure containing information about a filesystem object
 * @param PBITS permission bits PBITS_[COMBO] to check any combination of read (r), write (w), and execute (x)
 * @return int 1: user has "user" permission for the specified permission |
 *             0: user does not have "user" permission for the specified permission
 */
extern int check_permissions_usr(struct passwd *pw_entry, struct stat *fsobj_info, const int PBITS);
/**
 * @brief check "group" permission bits for a user: read (r), write (w), or execute (x)
 *
 * @param pw_entry password file entry containing information about a user
 * @param fsobj_info structure containing information about a filesystem object
 * @param PBITS permission bits PBITS_[COMBO] to check any combination of read (r), write (w), and execute (x)
 * @param valid_users array of strings containing uids of valid users
 * @param valid_users_count number of valid users located in valid_users
 * @return int 1: user has "group" permission for the specified permission |
 *             0: user does not have "group" permission for the specified permission
 */
extern int check_permissions_grp(struct passwd *pw_entry, struct stat *fsobj_info, const int PBITS, char ***valid_users, int valid_users_count);
/**
 * @brief check "other" permission bits for a user: read (r), write (w), or execute (x)
 *
 * @param pw_entry password file entry containing information about a user
 * @param fsobj_info structure containing information about a filesystem object
 * @param PBITS permission bits PBITS_[COMBO] to check any combination of read (r), write (w), and execute (x)
 * @return int 1: user has "other" permission for the specified permission |
 *             0: user does not have "other" permission for the specified permission
 */
extern int check_permissions_other(struct passwd *pw_entry, struct stat *fsobj_info, const int PBITS);

#endif
