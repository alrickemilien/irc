/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   s2c_names.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aemilien <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/14 13:20:47 by aemilien          #+#    #+#             */
/*   Updated: 2019/12/14 13:21:47 by aemilien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <client/irc.h>
#include <client/ui/panel.h>

static int	g_s2c_rpl_namreply_state = 0;
static char	g_s2c_rpl_namreply_buffer[512];

int			s2c_rpl_namreply(t_env *e, t_token *tokens)
{
	if (g_s2c_rpl_namreply_state == 0)
	{
		memset(g_s2c_rpl_namreply_buffer, 0, sizeof(g_s2c_rpl_namreply_buffer));
		g_s2c_rpl_namreply_state = 1;
	}
	if (!tokens[1].addr)
		return (-1);
	if (strlen(tokens[1].addr) >
		(sizeof(g_s2c_rpl_namreply_buffer) - strlen(g_s2c_rpl_namreply_buffer)))
	{
		loginfo(g_s2c_rpl_namreply_buffer);
		if (e->options.gui)
			ui_new_message(e->ui, g_s2c_rpl_namreply_buffer, UI_INFO_MSG);
		memset(g_s2c_rpl_namreply_buffer, 0, sizeof(g_s2c_rpl_namreply_buffer));
	}
	strcat(g_s2c_rpl_namreply_buffer, tokens[1].addr);
	return (IRC_S2C_RPL_NAMREPLY);
}

int			s2c_rpl_endofnames(t_env *e, t_token *tokens)
{
	(void)tokens;
	if (g_s2c_rpl_namreply_state == 0)
		return (-1);
	g_s2c_rpl_namreply_state = 0;
	loginfo(g_s2c_rpl_namreply_buffer);
	if (e->options.gui)
		ui_names(e->ui, g_s2c_rpl_namreply_buffer);
	return (IRC_S2C_RPL_ENDOFNAMES);
}
