/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gtk_do_select.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aemilien <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/14 13:24:08 by aemilien          #+#    #+#             */
/*   Updated: 2019/12/14 13:24:10 by aemilien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <client/irc.h>
#include <client/ui/ui.h>

gboolean	gtk_do_select(void *e)
{
	do_select(e);
	return (TRUE);
}
