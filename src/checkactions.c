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

/* sticky bit
    within a directory, it restricts the ability to delete and rename files to the file owner and root
    normally it is only relevant for directories
    represented symbolically as "t" or "T":
        t: no execute permission for others + sticky bit
        T: execute permissions for others + sticky bit
    represented numerically as "1" e.g. "1777"
*/

int checkfsobj(const char *fsobj, struct stat *fsobj_info)
{
    if (stat(fsobj, fsobj_info) == -1)
    {
        print_err_file(fsobj);
        return -1;
    }

    return 1; // fsobj is a valid filesystem object
}

int checkfsobj_dir(const char *fsobj, struct stat *fsobj_info)
{
    if (!S_ISDIR(fsobj_info->st_mode))
    {
        return -1;
    }

    return 1; // fsobj is a valid directory
}

int checkfsobj_file(const char *fsobj, struct stat *fsobj_info)
{
    if (!S_ISREG(fsobj_info->st_mode))
    {
        return -1;
    }

    return 1; // fsobj is a valid file
}

int checkfsobj_dev(const char *fsobj, struct stat *fsobj_info)
{
    if (!S_ISBLK(fsobj_info->st_mode) || !S_ISCHR(fsobj_info->st_mode))
    {
        return -1;
    }

    return 1; // fsobj is a valid device
}

int check_permissions_usr(struct passwd *pw_entry, struct stat *fsobj_info, const int PBITS)
{
    if (fsobj_info->st_uid == pw_entry->pw_uid) // current user is "user" owner of the file
    {
        switch (PBITS)
        {
        case PBITS_R:
            return fsobj_info->st_mode & S_IRUSR ? 1 : 0;
            break;
        case PBITS_W:
            return fsobj_info->st_mode & S_IWUSR ? 1 : 0;
            break;
        case PBITS_X:
            return fsobj_info->st_mode & S_IXUSR ? 1 : 0;
            break;
        case PBITS_RW:
            return ((fsobj_info->st_mode & S_IRUSR) && (fsobj_info->st_mode & S_IWUSR)) ? 1 : 0;
            break;
        case PBITS_RX:
            return ((fsobj_info->st_mode & S_IRUSR) && (fsobj_info->st_mode & S_IXUSR)) ? 1 : 0;
            break;
        case PBITS_WX:
            return ((fsobj_info->st_mode & S_IWUSR) && (fsobj_info->st_mode & S_IXUSR)) ? 1 : 0;
            break;
        case PBITS_RWX:
            return ((fsobj_info->st_mode & S_IRUSR) && (fsobj_info->st_mode & S_IWUSR) && (fsobj_info->st_mode & S_IXUSR)) ? 1 : 0;
            break;
        }
    }

    return 0;
}

int check_permissions_grp(struct passwd *pw_entry, struct stat *fsobj_info, const int PBITS, char ***valid_users, int valid_users_count)
{
    struct group *grp_entry;
    gid_t *grps;
    int num_grps = 0;

    getgrouplist(pw_entry->pw_name, pw_entry->pw_gid, NULL, &num_grps); // get number of groups
    grps = (gid_t *)malloc(sizeof(*grps) * num_grps);
    if (grps == NULL)
    {
        free_valid_users(valid_users, valid_users_count);
        print_err_exit();
    }
    getgrouplist(pw_entry->pw_name, pw_entry->pw_gid, grps, &num_grps); // get groups

    for (size_t i = 0; i < num_grps; i++)
    {
        grp_entry = getgrgid(grps[i]);
        if (fsobj_info->st_gid == grp_entry->gr_gid) // current group is "group" owner of the file
        {
            free(grps);
            switch (PBITS)
            {
            case PBITS_R:
                return fsobj_info->st_mode & S_IRGRP ? 1 : 0;
                break;
            case PBITS_W:
                return fsobj_info->st_mode & S_IWGRP ? 1 : 0;
                break;
            case PBITS_X:
                return fsobj_info->st_mode & S_IXGRP ? 1 : 0;
                break;
            case PBITS_RW:
                return ((fsobj_info->st_mode & S_IRGRP) && (fsobj_info->st_mode & S_IWGRP)) ? 1 : 0;
                break;
            case PBITS_RX:
                return ((fsobj_info->st_mode & S_IRGRP) && (fsobj_info->st_mode & S_IXGRP)) ? 1 : 0;
                break;
            case PBITS_WX:
                return ((fsobj_info->st_mode & S_IWGRP) && (fsobj_info->st_mode & S_IXGRP)) ? 1 : 0;
                break;
            case PBITS_RWX:
                return ((fsobj_info->st_mode & S_IRGRP) && (fsobj_info->st_mode & S_IWGRP) && (fsobj_info->st_mode & S_IXGRP)) ? 1 : 0;
                break;
            }
        }
    }

    free(grps);
    return 0; // no group owner found for user
}

int check_permissions_other(struct passwd *pw_entry, struct stat *fsobj_info, const int PBITS)
{
    // "other" includes anyone that doesn't fall into the categories of "user" or "group"
    switch (PBITS)
    {
    case PBITS_R:
        return fsobj_info->st_mode & S_IROTH ? 1 : 0;
        break;
    case PBITS_W:
        return fsobj_info->st_mode & S_IWOTH ? 1 : 0;
        break;
    case PBITS_X:
        return fsobj_info->st_mode & S_IXOTH ? 1 : 0;
        break;
    case PBITS_RW:
        return ((fsobj_info->st_mode & S_IROTH) && (fsobj_info->st_mode & S_IWOTH)) ? 1 : 0;
        break;
    case PBITS_RX:
        return ((fsobj_info->st_mode & S_IROTH) && (fsobj_info->st_mode & S_IXOTH)) ? 1 : 0;
        break;
    case PBITS_WX:
        return ((fsobj_info->st_mode & S_IWOTH) && (fsobj_info->st_mode & S_IXOTH)) ? 1 : 0;
        break;
    case PBITS_RWX:
        return ((fsobj_info->st_mode & S_IROTH) && (fsobj_info->st_mode & S_IWOTH) && (fsobj_info->st_mode & S_IXOTH)) ? 1 : 0;
        break;
    }

    return 0;
}

int check_cd_search(struct stat *fsobj_info, char ***valid_users, int *can_everyone)
{
    struct passwd *pw_entry;
    int total_users_count = 0, valid_users_count = 0;
    int has_x_perms = 0, is_root = 0;

    setpwent();
    while ((pw_entry = getpwent()) != NULL) // get every single user from the "passwd" file
    {
        // has execute bit OR is root
        has_x_perms = check_permissions_usr(pw_entry, fsobj_info, PBITS_X) ||
                      check_permissions_grp(pw_entry, fsobj_info, PBITS_X, valid_users, valid_users_count) ||
                      check_permissions_other(pw_entry, fsobj_info, PBITS_X);
        is_root = strcmp(pw_entry->pw_name, "root") == 0;

        if (has_x_perms || is_root) // check that it's a valid user
        {
            add_valid_users_entry(valid_users, valid_users_count);
            strncpy((*valid_users)[valid_users_count], pw_entry->pw_name, NAME_MAX);
            valid_users_count++;
        }
        total_users_count++;
    }
    endpwent();

    if (valid_users_count == total_users_count) // (everyone)
    {
        *can_everyone = 1;
    }

    return valid_users_count;
}

int check_delete(struct stat *fsobj_info, struct stat *parentdir_info, char ***valid_users, int *can_everyone)
{
    struct passwd *pw_entry;
    int total_users_count = 0, valid_users_count = 0;
    int is_root = 0, has_w_perms_parent = 0, is_owner_file = 0, is_owner_parent = 0, has_wx_perms_parent = 0;
    int is_sticky = parentdir_info->st_mode & __S_ISVTX ? 1 : 0;

    setpwent();
    while ((pw_entry = getpwent()) != NULL)
    {
        /*
            NON-STICKY: has write AND execute bit
                permissions on the file are irrelevant, only the permissions on the PARENT directory matter
            STICKY: has write bit on parent directory AND (owns the files OR owns the parent directory)
                permissions on the file are still irrelevant, but OWNER of file matters
            EITHER: OR is root
        */
        has_wx_perms_parent = check_permissions_usr(pw_entry, parentdir_info, PBITS_WX) ||
                              check_permissions_grp(pw_entry, parentdir_info, PBITS_WX, valid_users, valid_users_count) ||
                              check_permissions_other(pw_entry, parentdir_info, PBITS_WX);
        has_w_perms_parent = check_permissions_usr(pw_entry, parentdir_info, PBITS_W) ||
                             check_permissions_grp(pw_entry, parentdir_info, PBITS_W, valid_users, valid_users_count) ||
                             check_permissions_other(pw_entry, parentdir_info, PBITS_W);
        is_owner_file = pw_entry->pw_uid == fsobj_info->st_uid;
        is_owner_parent = pw_entry->pw_uid == parentdir_info->st_uid;
        is_root = strcmp(pw_entry->pw_name, "root") == 0;

        if ((!is_sticky && has_wx_perms_parent) ||
            (is_sticky && has_w_perms_parent && (is_owner_file || is_owner_parent)) ||
            is_root)
        {
            add_valid_users_entry(valid_users, valid_users_count);
            strncpy((*valid_users)[valid_users_count], pw_entry->pw_name, NAME_MAX);
            valid_users_count++;
        }
        total_users_count++;
    }
    endpwent();

    if (valid_users_count == total_users_count)
    {
        *can_everyone = 1;
    }

    return valid_users_count;
}

int check_execute(struct stat *fsobj_info, char ***valid_users, int *can_everyone)
{
    struct passwd *pw_entry;
    int total_users_count = 0, valid_users_count = 0;
    int has_x_perms = 0, is_root = 0;
    // execute bit MUST be set on at least one of the user, group, or other permission groups for the file to be executable
    int is_executable = fsobj_info->st_mode & S_IXUSR || fsobj_info->st_mode & S_IXGRP || fsobj_info->st_mode & S_IXOTH ? 1 : 0;

    setpwent();
    while ((pw_entry = getpwent()) != NULL)
    {
        // has execute bit OR is root AND has execute bit set on at least one permission group
        has_x_perms = check_permissions_usr(pw_entry, fsobj_info, PBITS_X) ||
                      check_permissions_grp(pw_entry, fsobj_info, PBITS_X, valid_users, valid_users_count) ||
                      check_permissions_other(pw_entry, fsobj_info, PBITS_X);
        is_root = strcmp(pw_entry->pw_name, "root") == 0;

        if (has_x_perms || (is_root && is_executable))
        {
            add_valid_users_entry(valid_users, valid_users_count);
            strncpy((*valid_users)[valid_users_count], pw_entry->pw_name, NAME_MAX);
            valid_users_count++;
        }
        total_users_count++;
    }
    endpwent();

    if (valid_users_count == total_users_count)
    {
        *can_everyone = 1;
    }

    return valid_users_count;
}

int check_ls_dir_read(struct stat *fsobj_info, char ***valid_users, int *can_everyone)
{
    struct passwd *pw_entry;
    int total_users_count = 0, valid_users_count = 0;
    int has_r_perms = 0, is_root = 0;

    setpwent();
    while ((pw_entry = getpwent()) != NULL)
    {
        // has read bit OR is root
        has_r_perms = check_permissions_usr(pw_entry, fsobj_info, PBITS_R) ||
                      check_permissions_grp(pw_entry, fsobj_info, PBITS_R, valid_users, valid_users_count) ||
                      check_permissions_other(pw_entry, fsobj_info, PBITS_R);
        is_root = strcmp(pw_entry->pw_name, "root") == 0;

        if (has_r_perms || is_root)
        {
            add_valid_users_entry(valid_users, valid_users_count);
            strncpy((*valid_users)[valid_users_count], pw_entry->pw_name, NAME_MAX);
            valid_users_count++;
        }
        total_users_count++;
    }
    endpwent();

    if (valid_users_count == total_users_count)
    {
        *can_everyone = 1;
    }

    return valid_users_count;
}

int check_ls_file_dev(struct stat *parentdir_info, char ***valid_users, int *can_everyone)
{
    struct passwd *pw_entry;
    int total_users_count = 0, valid_users_count = 0;
    int has_x_perms_parent = 0, is_root = 0;

    setpwent();
    while ((pw_entry = getpwent()) != NULL)
    {
        // has execute bit on parent directory OR is root
        has_x_perms_parent = check_permissions_usr(pw_entry, parentdir_info, PBITS_X) ||
                             check_permissions_grp(pw_entry, parentdir_info, PBITS_X, valid_users, valid_users_count) ||
                             check_permissions_other(pw_entry, parentdir_info, PBITS_X);
        is_root = strcmp(pw_entry->pw_name, "root") == 0;

        if (has_x_perms_parent || is_root)
        {
            add_valid_users_entry(valid_users, valid_users_count);
            strncpy((*valid_users)[valid_users_count], pw_entry->pw_name, NAME_MAX);
            valid_users_count++;
        }
        total_users_count++;
    }
    endpwent();

    if (valid_users_count == total_users_count)
    {
        *can_everyone = 1;
    }

    return valid_users_count;
}

int check_write_dir(struct stat *fsobj_info, char ***valid_users, int *can_everyone)
{
    struct passwd *pw_entry;
    int total_users_count = 0, valid_users_count = 0;
    int has_wx_perms = 0, is_root = 0;

    setpwent();
    while ((pw_entry = getpwent()) != NULL)
    {
        // has write AND execute bit OR is root
        has_wx_perms = check_permissions_usr(pw_entry, fsobj_info, PBITS_WX) ||
                       check_permissions_grp(pw_entry, fsobj_info, PBITS_WX, valid_users, valid_users_count) ||
                       check_permissions_other(pw_entry, fsobj_info, PBITS_WX);
        is_root = strcmp(pw_entry->pw_name, "root") == 0;

        if (has_wx_perms || is_root)
        {
            add_valid_users_entry(valid_users, valid_users_count);
            strncpy((*valid_users)[valid_users_count], pw_entry->pw_name, NAME_MAX);
            valid_users_count++;
        }
        total_users_count++;
    }
    endpwent();

    if (valid_users_count == total_users_count)
    {
        *can_everyone = 1;
    }

    return valid_users_count;
}

int check_write_file_dev(struct stat *fsobj_info, char ***valid_users, int *can_everyone)
{
    struct passwd *pw_entry;
    int total_users_count = 0, valid_users_count = 0;
    int has_w_perms = 0, is_root = 0;

    setpwent();
    while ((pw_entry = getpwent()) != NULL)
    {
        // has write bit OR is root
        has_w_perms = check_permissions_usr(pw_entry, fsobj_info, PBITS_W) ||
                      check_permissions_grp(pw_entry, fsobj_info, PBITS_W, valid_users, valid_users_count) ||
                      check_permissions_other(pw_entry, fsobj_info, PBITS_W);
        is_root = strcmp(pw_entry->pw_name, "root") == 0;

        if (has_w_perms || is_root)
        {
            add_valid_users_entry(valid_users, valid_users_count);
            strncpy((*valid_users)[valid_users_count], pw_entry->pw_name, NAME_MAX);
            valid_users_count++;
        }
        total_users_count++;
    }
    endpwent();

    if (valid_users_count == total_users_count)
    {
        *can_everyone = 1;
    }

    return valid_users_count;
}
