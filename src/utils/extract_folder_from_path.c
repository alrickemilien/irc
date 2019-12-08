/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extract_folder_from_path.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aemilien <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/08 13:20:54 by aemilien          #+#    #+#             */
/*   Updated: 2019/12/08 13:20:57 by aemilien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

static const char	*loc_dirname(const char *path)
{
	static char			buffer[PATH_MAX];
	static const char	dot[] = ".";
	int					last_slash_pos;
	size_t				i;

	last_slash_pos = -1;
	if (!path)
		return (dot);
	i = 0;
	while (path[i])
	{
		if (i >= PATH_MAX)
			return (dot);
		if (path[i] == '/')
			last_slash_pos = i;
		i++;
	}
	if (last_slash_pos == -1)
		return (dot);
	strncpy(buffer, path, last_slash_pos);
	buffer[last_slash_pos] = '\0';
	return (buffer);
}

char				*extract_folder_from_path(
		const char *path)
{
	return (strdup(loc_dirname(path)));
}
