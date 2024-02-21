/**
 *  Alex Carbajal
 *  CPT_S 360, Spring 2024
 *  whocan
 */

#ifndef CHECKACTIONS
#define CHECKACTIONS

#include <stdio.h>     // for printf()
#include <stdlib.h>    // for exit()
#include <unistd.h>    // for stat(), chdir()
#include <sys/stat.h>  // for struct stat
#include <pwd.h>       // for getpwent(), setpwent(), endpwent()
#include <grp.h>       // for getgrent(), setgrent(), endgrent(), getgrouplist()
#include <sys/types.h> // for gid_t

extern void checkfsobj_dir(const char *fsobj, struct stat *fsobj_info);
extern void checkcd(struct stat *fsobj_info);
extern int check_permissions_usr(struct passwd *pw_entry, struct stat *fsobj_info);
extern int check_permissions_grp(struct passwd *pw_entry, struct stat *fsobj_info);
extern int check_permissions_other(struct passwd *pw_entry, struct stat *fsobj_info);

#endif
