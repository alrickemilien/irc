/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gtk_builder_load.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aemilien <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/14 13:28:24 by aemilien          #+#    #+#             */
/*   Updated: 2019/12/14 13:28:25 by aemilien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <client/irc.h>
#include <client/ui/ui.h>

int	gtk_builder_load(
		GtkBuilder *b,
		const char *bin_path,
		const char *ui_path)
{
	char	*bin_folder_path;
	char	*template_path;
	int		err;
	GError	*gerror;

	err = 0;
	gerror = NULL;
	bin_folder_path = extract_folder_from_path(bin_path);
	if (!bin_folder_path)
		return (-1);
	template_path = strjoin(bin_folder_path, ui_path);
	if (!template_path)
		return (-1);
	gtk_builder_set_translation_domain(b, GETTEXT_PACKAGE);
	if (gtk_builder_add_from_file(b, template_path, &gerror) == 0)
	{
		logerror("Unable to read file :: %s\n", gerror->message);
		g_error_free(gerror);
		err = -1;
	}
	free(bin_folder_path);
	free(template_path);
	return (err);
}
