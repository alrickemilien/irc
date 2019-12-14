/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui_join_channels_index_of.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aemilien <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/14 13:54:47 by aemilien          #+#    #+#             */
/*   Updated: 2019/12/14 13:54:48 by aemilien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <client/ui/panel.h>

int		ui_join_channels_index_of(t_ui_panel *ui,
		const char *needle,
		size_t needle_size)
{
	int i;

	i = 0;
	while (i < ui->channels_count)
	{
		if (memcmp(ui->channels[i].label, needle, needle_size) == 0 &&
				(ui->channels[i].label[needle_size] == ' ' ||
				ui->channels[i].label[needle_size] == 0))
			return (i);
		i++;
	}
	return (-1);
}
