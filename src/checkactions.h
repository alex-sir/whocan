/**
 *  Alex Carbajal
 *  CPT_S 360, Spring 2024
 *  whocan
 */

#ifndef CHECKACTIONS
#define CHECKACTIONS

#include <stdio.h>     // for printf()
#include <stdlib.h>    // for exit()
#include <unistd.h>    // for stat()
#include <sys/stat.h>  // for struct stat
#include <pwd.h>       // for getpwent(), setpwent(), endpwent()
#include <grp.h>       // for getgrent(), setgrent(), endgrent(), getgrouplist()
#include <sys/types.h> // for gid_t
#include <libgen.h>    // for dirname()

#define INIT_NUM_USERS 50
#define NAME_MAX sysconf(_SC_LOGIN_NAME_MAX) // max size of a username
// 7 rwx combinations
#define PBITS_R 4
#define PBITS_W 2
#define PBITS_X 1
#define PBITS_RW 6
#define PBITS_RX 5
#define PBITS_WX 3
#define PBITS_RWX 7

/**
 * @brief check if a filesystem object is a valid filesytsem object
 *
 * @param fsobj pathname to a filesystem object
 * @param fsobj_info structure that will be filled with information about fsobj if it is valid
 * @return int 1: fsobj is a valid filesytem object |
 *            -1: fsobj is not a valid filesystem object
 */
extern int checkfsobj(const char *fsobj, struct stat *fsobj_info);
/**
 * @brief check if a filesystem object is a directory
 *
 * @param fsobj pathname to a filesystem object
 * @param fsobj_info structure filled with information about fsobj
 * @return int 1: fsobj is a valid directory |
 *            -1: fsobj is not a valid directory
 */
extern int checkfsobj_dir(const char *fsobj, struct stat *fsobj_info);
/**
 * @brief check if a filesystem object is a regular file
 *
 * @param fsobj pathname to a filesystem object
 * @param fsobj_info structure filled with information about fsobj
 * @return int 1: fsobj is a valid regular file |
 *            -1: fsobj is not a valid regular file
 */
extern int checkfsobj_file(const char *fsobj, struct stat *fsobj_info);
/**
 * @brief check if a filesystem object is a block or character device
 *
 * @param fsobj pathname to a filesystem object
 * @param fsobj_info structure filled with information about fsobj
 * @return int 1: fsobj is a valid block or character device |
 *            -1: fsobj is not a valid block or character device
 */
extern int checkfsobj_dev(const char *fsobj, struct stat *fsobj_info);
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
/**
 * @brief check which users have permissions to "cd" or "search" into a directory
 *
 * @param fsobj_info structure containing information about a directory
 * @param valid_users array of strings that will be filled with the usernames of users who can "cd" or "search"
 * @param can_everyone will indicate if ALL users can "cd" or "search"
 * @return int number of users that can "cd" or "search"
 */
extern int check_cd_search(struct stat *fsobj_info, char ***valid_users, int *can_everyone);
/**
 * @brief check which users have permissions to "delete" a directory, file, or device
 *
 * @param fsobj_info structure containing information about a directory, file, or device
 * @param parentdir_info structure containing information about the parent directory of fsobj
 * @param valid_users array of strings that will be filled with the usernames of users who can "delete"
 * @param can_everyone will indicate if ALL users can "delete"
 * @return int number of users that can "delete"
 */
extern int check_delete(struct stat *fsobj_info, struct stat *parentdir_info, char ***valid_users, int *can_everyone);
/**
 * @brief check which users have permissions to "execute" a file
 *
 * @param fsobj_info structure containing information about a file
 * @param valid_users array of strings that will be filled with the usernames of users who can "execute"
 * @param can_everyone will indicate if ALL users can "execute"
 * @return int number of users that can "execute"
 */
extern int check_execute(struct stat *fsobj_info, char ***valid_users, int *can_everyone);
/**
 * @brief check which users have permissions to "ls" a directory or "read" a directory, file, or device
 *
 * @param fsobj_info structure containing information about a directory, file, or device
 * @param valid_users array of strings that will be filled with the usernames of users who can "ls" or "read"
 * @param can_everyone will indicate if ALL users can "ls" or "read"
 * @return int number of users that can "ls" or "read"
 */
extern int check_ls_dir_read(struct stat *fsobj_info, char ***valid_users, int *can_everyone);
/**
 * @brief check which users have permissions to "ls" a file or device
 *
 * @param parentdir_info structure containing information about the parent directory of a filesystem object
 * @param valid_users array of strings that will be filled with the usernames of users who can "ls"
 * @param can_everyone will indicate if ALL users can "ls"
 * @return int number of users that can "ls"
 */
extern int check_ls_file_dev(struct stat *parentdir_info, char ***valid_users, int *can_everyone);
/**
 * @brief check which users have permissions to "write" into directory
 *
 * @param fsobj_info structure containing information about a directory
 * @param valid_users array of strings that will be filled with the usernames of users who can "write"
 * @param can_everyone will indicate if ALL users can "write"
 * @return int number of users that can "write"
 */
extern int check_write_dir(struct stat *fsobj_info, char ***valid_users, int *can_everyone);
/**
 * @brief check which users have permissions to "write" into a file or device
 *
 * @param fsobj_info structure containing information about a file or device
 * @param valid_users array of strings that will be filled with the usernames of users who can "write"
 * @param can_everyone will indicate if ALL users can "write"
 * @return int number of users that can "write"
 */
extern int check_write_file_dev(struct stat *fsobj_info, char ***valid_users, int *can_everyone);

#endif
