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

/**
 * @brief check if a filesystem object is a directory; if not, exit the program
 *
 * @param fsobj pathname to a filesystem object
 * @param fsobj_info structure that will be filled with information about fsobj if it is a directory
 */
extern void checkfsobj_dir(const char *fsobj, struct stat *fsobj_info);
/**
 * @brief print a list of which users have permissions to "cd" into a directory
 *
 * @param fsobj_info structure containing information about a directory
 */
extern void checkcd(struct stat *fsobj_info);
/**
 * @brief check "user" permission bits for a user: read (r), write (w), or execute (x)
 *
 * @param pw_entry password file entry containing information about a user
 * @param fsobj_info structure containing information about a filesystem object
 * @param permission permission bit to check: read (r), write (w), or execute (x)
 * @return int 1: user has "user" permission for the specified permission |
 *             0: user does not have "user" permission for the specified permission
 */
extern int check_permissions_usr(struct passwd *pw_entry, struct stat *fsobj_info, const char permission);
/**
 * @brief check "group" permission bits for a user: read (r), write (w), or execute (x)
 *
 * @param pw_entry password file entry containing information about a user
 * @param fsobj_info structure containing information about a filesystem object
 * @param permission permission bit to check: read (r), write (w), or execute (x)
 * @return int 1: user has "group" permission for the specified permission |
 *             0: user does not have "group" permission for the specified permission
 */
extern int check_permissions_grp(struct passwd *pw_entry, struct stat *fsobj_info, const char permission);
/**
 * @brief check "other" permission bits for a user: read (r), write (w), or execute (x)
 *
 * @param pw_entry password file entry containing information about a user
 * @param fsobj_info structure containing information about a filesystem object
 * @param permission permission bit to check: read (r), write (w), or execute (x)
 * @return int 1: user has "other" permission for the specified permission |
 *             0: user does not have "other" permission for the specified permission
 */
extern int check_permissions_other(struct passwd *pw_entry, struct stat *fsobj_info, const char permission);

#endif
