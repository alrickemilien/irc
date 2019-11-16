#include "irc.h"
#include <stdio.h>
#include <string.h>
#include <libgen.h>
#include <stdlib.h>

/*
** This function extract the folder from the provided path
** where it is located.
** =======================================================
** Examples : the binary is executed at /root/
** /path/to/file => /path/to/
** path => /root/
** ./path/to/file => /root/path/to/
** path/to/file => ./path/to/
** /path/to/ => /path/
** ../ => /
** ../../path/to/file => /path/to/
** ./ => /
** . => /
** .. => /
*/

char *extract_folder_from_path(const char *path)
{
    char *tmp;
    char *ret;

    tmp = strdup(path);
    ret = dirname(tmp);
    free(tmp);
    return (ret);
}
