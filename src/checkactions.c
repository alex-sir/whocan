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

    setpwent();
    while ((pw_entry = getpwent()) != NULL)
    {
        // don't forget to check the sticky bit
        // root can cd into a file no matter no what
        if (strcmp(pw_entry->pw_name, "root") == 0 ||
            check_permissions_usr(pw_entry, fsobj_info, 'x') ||
            check_permissions_grp(pw_entry, fsobj_info, 'x') ||
            check_permissions_other(pw_entry, fsobj_info, 'x'))
        {
            printf("%s\n", pw_entry->pw_name);
        }
    }
    endpwent();
}

int check_permissions_usr(struct passwd *pw_entry, struct stat *fsobj_info, const char permission)
{
    if (fsobj_info->st_uid == pw_entry->pw_uid)
    {
        // REM: do any of the actions require multiple permissions? might have to modify these switch statements if so.
        switch (permission)
        {
        case 'r':
            return fsobj_info->st_mode & S_IRUSR ? 1 : 0;
            break;
        case 'w':
            return fsobj_info->st_mode & S_IWUSR ? 1 : 0;
            break;
        case 'x':
            return fsobj_info->st_mode & S_IXUSR ? 1 : 0;
            break;
        default:
            break;
        }
    }

    return 0;
}

int check_permissions_grp(struct passwd *pw_entry, struct stat *fsobj_info, const char permission)
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
        // current group is group owner of file
        if (fsobj_info->st_gid == grp_entry->gr_gid)
        {
            free(grps);
            switch (permission)
            {
            case 'r':
                return fsobj_info->st_mode & S_IRGRP ? 1 : 0;
                break;
            case 'w':
                return fsobj_info->st_mode & S_IWGRP ? 1 : 0;
                break;
            case 'x':
                return fsobj_info->st_mode & S_IXGRP ? 1 : 0;
                break;
            default:
                break;
            }
        }
    }

    free(grps);
    return 0; // no group owner found for user
}

int check_permissions_other(struct passwd *pw_entry, struct stat *fsobj_info, const char permission)
{
    switch (permission)
    {
    case 'r':
        return fsobj_info->st_mode & S_IROTH ? 1 : 0;
        break;
    case 'w':
        return fsobj_info->st_mode & S_IWOTH ? 1 : 0;
        break;
    case 'x':
        return fsobj_info->st_mode & S_IXOTH ? 1 : 0;
        break;
    default:
        break;
    }

    return 0;
}
