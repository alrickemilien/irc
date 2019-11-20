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

char *merge_and_extract_folder_from_path(const char *a, const char *b)
{
    char *tmp_1;
    char *tmp_2;
    char *ret;

    if (a[strlen(a) - 1] != '/')
        tmp_1 = strjoin(a, "/");
    else
        tmp_1 = strjoin(a, "");

    tmp_2 = strjoin(tmp_1, b);

    printf("tmp_2: %s\n", tmp_2);

    ret = dirname(tmp_2);
    free(tmp_1);
    free(tmp_2);
    return (ret);
}
