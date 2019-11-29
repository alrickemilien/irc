#include "irc.h"
#include <stdio.h>
#include <string.h>
#include <libgen.h>
#include <stdlib.h>
#include <limits.h>

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
** dirname not used, see https://www.jefftk.com/p/dirname-is-evil
*/

static const char *_dirname(const char *path)
{
    static char       buffer[PATH_MAX];
    static const char dot[] = ".";
    int            last_slash_pos = -1;
    size_t            i;

    if (!path)
        return (dot);
    for (i = 0; path[i]; i++)
    {
        if (i >= PATH_MAX)
            return dot;
        if (path[i] == '/')
            last_slash_pos = i;
    }
    if (last_slash_pos == -1)
        return (dot);
    strncpy(buffer, path, last_slash_pos);
    buffer[last_slash_pos] = '\0';
    return (buffer);
}

char *extract_folder_from_path(const char *path)
{
    return (strdup(_dirname(path)));
}