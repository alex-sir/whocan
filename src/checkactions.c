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
    within a directory, it restricts the ability to delete and rename files to the file owner, group owner, or root
    even if "others" have write permissions
    represented symbolically as "t"
    represented numerically as "1" e.g. "1777"
*/

int checkfsobj_dir(const char *fsobj, struct stat *fsobj_info)
{
    if (stat(fsobj, fsobj_info) == -1)
    {
        print_err_file(fsobj);
        return -1;
    }
    if (!S_ISDIR(fsobj_info->st_mode))
    {
        fprintf(stderr, "%s: Not a directory\n", fsobj);
        return -1;
    }

    return 1; // fsobj is a valid directory
}

int checkcd(struct stat *fsobj_info, char ***valid_users, int *canEveryone)
{
    struct passwd *pw_entry;
    int total_users_count = 0, valid_users_count = 0;

    setpwent();
    while ((pw_entry = getpwent()) != NULL)
    {

        if (strcmp(pw_entry->pw_name, "root") == 0 ||
            check_permissions_usr(pw_entry, fsobj_info, PBITS_X) ||
            check_permissions_grp(pw_entry, fsobj_info, PBITS_X, valid_users, valid_users_count) ||
            check_permissions_other(pw_entry, fsobj_info, PBITS_X))
        {
            strcpy((*valid_users)[valid_users_count], pw_entry->pw_name);
            valid_users_count++;
        }
        total_users_count++;
    }
    endpwent();

    if (valid_users_count == total_users_count)
    {
        *canEveryone = 1;
    }

    return valid_users_count;
}

int check_permissions_usr(struct passwd *pw_entry, struct stat *fsobj_info, const int PBITS)
{
    if (fsobj_info->st_uid == pw_entry->pw_uid)
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
        default:
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
        // current group is group owner of file
        if (fsobj_info->st_gid == grp_entry->gr_gid)
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
            default:
                break;
            }
        }
    }

    free(grps);
    return 0; // no group owner found for user
}

int check_permissions_other(struct passwd *pw_entry, struct stat *fsobj_info, const int PBITS)
{
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
    default:
        break;
    }

    return 0;
}
