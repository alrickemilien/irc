/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   i18n.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aemilien <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/08 16:20:50 by aemilien          #+#    #+#             */
/*   Updated: 2019/12/08 16:22:49 by aemilien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <client/irc.h>

int	init_i18n(const char *bin_path)
{
	int		ret;
	char	*bin_dir_path;
	char	*locale_dir_path;

	ret = 0;
	setlocale(LC_ALL, "");
	bin_dir_path = extract_folder_from_path(bin_path);
	if (!bin_dir_path)
		return (-1);
	locale_dir_path = strjoin(bin_dir_path, LOCALE_DIR);
	if (!locale_dir_path)
	{
		free(bin_dir_path);
		return (-1);
	}
	if (bindtextdomain(GETTEXT_PACKAGE, locale_dir_path) == NULL)
		ret = logerrno("init_i18n::bindtextdomain");
	if (bind_textdomain_codeset(GETTEXT_PACKAGE, "UTF-8") == NULL)
		ret = logerrno("init_i18n::bind_textdomain_codeset");
	if (textdomain(GETTEXT_PACKAGE) == NULL)
		ret = logerrno("init_i18n::textdomain");
	free(bin_dir_path);
	free(locale_dir_path);
	return (ret);
}
