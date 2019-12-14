/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gtk_get_assets.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aemilien <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/14 13:24:29 by aemilien          #+#    #+#             */
/*   Updated: 2019/12/14 13:29:37 by aemilien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <client/irc.h>

char	*gtk_get_assets(
		const char *bin_path, const char *path)
{
	char	*bin_folder_path;

	bin_folder_path = extract_folder_from_path(bin_path);
	if (!bin_folder_path)
		return (NULL);
	return (strjoin(bin_folder_path, path));
}
