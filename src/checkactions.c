/**
 *  Alex Carbajal
 *  CPT_S 360, Spring 2024
 *  whocan
 */

#include "checkactions.h"
#include "helpers.h"

/* Resources
    The Mode Bits for Access Permission: https://www.gnu.org/software/libc/manual/html_node/Permission-Bits.html
*/

void checkfsobj_dir(const char *fsobj, struct stat *fsobj_info)
{
    if (stat(fsobj, fsobj_info) == -1)
    {
        print_err_file(fsobj);
        exit(EXIT_FAILURE);
    }
    if (!S_ISDIR(fsobj_info->st_mode))
    {
        fprintf(stderr, "%s: Not a directory\n", fsobj);
        exit(EXIT_FAILURE);
    }
}

void checkcd(struct stat *fsobj_info)
{
    struct passwd *pw_entry;
    while ((pw_entry = getpwent()) != NULL)
    {
        /* TODO: check user, group, and other permissions
                 if the user has permission from at least one category, they are good
        */
        if (check_permissions_usr(pw_entry, fsobj_info) ||
            check_permissions_grp(pw_entry, fsobj_info) ||
            check_permissions_other(pw_entry, fsobj_info))
        {
            printf("%s\n", pw_entry->pw_name);
        }
    }
    setpwent();
    endpwent();
}

int check_permissions_usr(struct passwd *pw_entry, struct stat *fsobj_info)
{
    return 0;
}

int check_permissions_grp(struct passwd *pw_entry, struct stat *fsobj_info)
{
    struct group *grp_entry;
    gid_t *grps;
    int num_grps = 0;

    getgrouplist(pw_entry->pw_name, pw_entry->pw_gid, NULL, &num_grps); // get number of groups
    grps = malloc(sizeof(*grps) * num_grps);
    getgrouplist(pw_entry->pw_name, pw_entry->pw_gid, grps, &num_grps); // get groups

    for (int i = 0; i < num_grps; i++)
    {
        grp_entry = getgrgid(grps[i]);
        if (fsobj_info->st_gid == grp_entry->gr_gid)
        {
            free(grps);
            return fsobj_info->st_mode & S_IXGRP ? 1 : 0; // return whether grp_entry has execute permissions
        }
    }

    free(grps);
    return 0; // no group owner found for user
}

int check_permissions_other(struct passwd *pw_entry, struct stat *fsobj_info)
{
    return 0;
}
