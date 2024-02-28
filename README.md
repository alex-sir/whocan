# Whocan

Prints a list of the users on a Unix-like system that can perform a specified action on a file, directory, or device based on the permissions set on that filesystem object.

## How To Use

``` bash
./whocan [ACTION] [FSOBJ]
```

**ACTION** is one of the below valid actions and **FSOBJ** is a filesystem object (file, directory, or device).

## Options

| ACTION | Type of FSOBJ | Description |
| -------- | -------- | -------- |
| cd | directory | Change the current directory to **FSOBJ** |
| delete | all | Delete **FSOBJ** |
| execute | file | Execute **FSOBJ** |
| ls | all | List the contents of the directory **FSOBJ** OR list the file or device **FSOBJ** |
| read | all | Open **FSOBJ** for reading |
| search | directory | Search **FSOBJ** |
| write | all | Add or delete files in directory **FSOBJ** OR open the file or device **FSOBJ** for writing |
