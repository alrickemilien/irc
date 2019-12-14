/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   s2c_rpl_list.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aemilien <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/14 13:13:29 by aemilien          #+#    #+#             */
/*   Updated: 2019/12/14 13:15:01 by aemilien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <client/irc.h>
#include <client/ui/panel.h>

static int	g_s2c_rpl_listrply_state = 0;
static char	g_s2c_rpl_listrply_buffer[512];

int			s2c_rpl_liststart(t_env *e, t_token *tokens)
{
	(void)e;
	(void)tokens;
	if (g_s2c_rpl_listrply_state == 0)
	{
		memset(g_s2c_rpl_listrply_buffer, 0, sizeof(g_s2c_rpl_listrply_buffer));
		g_s2c_rpl_listrply_state = 1;
	}
	return (IRC_S2C_RPL_LISTSTART);
}

int			s2c_rpl_list(t_env *e, t_token *tokens)
{
	if (g_s2c_rpl_listrply_state == 0 || !tokens[1].addr)
		return (-1);
	if (tokens[1].len >
		(sizeof(g_s2c_rpl_listrply_buffer) - strlen(g_s2c_rpl_listrply_buffer)))
	{
		loginfo(g_s2c_rpl_listrply_buffer);
		if (e->options.gui)
			ui_new_message(e->ui, g_s2c_rpl_listrply_buffer, UI_INFO_MSG);
		memset(g_s2c_rpl_listrply_buffer, 0, sizeof(g_s2c_rpl_listrply_buffer));
	}
	if (g_s2c_rpl_listrply_buffer[0] != 0)
		strcat(g_s2c_rpl_listrply_buffer, "\n");
	strncat(g_s2c_rpl_listrply_buffer, tokens[1].addr, tokens[1].len);
	return (IRC_S2C_RPL_LIST);
}

int			s2c_rpl_listend(t_env *e, t_token *tokens)
{
	(void)tokens;
	if (g_s2c_rpl_listrply_state == 0)
		return (-1);
	g_s2c_rpl_listrply_state = 0;
	if (strlen(g_s2c_rpl_listrply_buffer) == 0)
		return (IRC_S2C_RPL_LISTEND);
	loginfo(g_s2c_rpl_listrply_buffer);
	if (e->options.gui)
		ui_new_message(e->ui, g_s2c_rpl_listrply_buffer, UI_INFO_MSG);
	return (IRC_S2C_RPL_LISTEND);
}
