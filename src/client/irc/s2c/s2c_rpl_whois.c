/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   s2c_rpl_whois.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aemilien <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/14 13:09:08 by aemilien          #+#    #+#             */
/*   Updated: 2019/12/14 13:09:09 by aemilien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <client/irc.h>
#include <client/ui/panel.h>

static int	g_s2c_rpl_whois_state = 0;
static char	g_s2c_rpl_whois_buffer[512];

int			s2c_rpl_whoisuser(t_env *e, t_token *tokens)
{
	if (g_s2c_rpl_whois_state == 0)
	{
		memset(g_s2c_rpl_whois_buffer, 0, sizeof(g_s2c_rpl_whois_buffer));
		g_s2c_rpl_whois_state = 1;
	}
	if (!tokens[1].addr)
		return (-1);
	if (strlen(tokens[1].addr) >
			(sizeof(g_s2c_rpl_whois_buffer) - strlen(g_s2c_rpl_whois_buffer)))
	{
		loginfo(g_s2c_rpl_whois_buffer);
		if (e->options.gui)
			ui_new_message(e->ui, g_s2c_rpl_whois_buffer, UI_INFO_MSG);
		memset(g_s2c_rpl_whois_buffer, 0, sizeof(g_s2c_rpl_whois_buffer));
	}
	strcat(g_s2c_rpl_whois_buffer, tokens[1].addr);
	strcat(g_s2c_rpl_whois_buffer, " ");
	return (IRC_S2C_RPL_WHOISUSER);
}

int			s2c_rpl_whoischannels(t_env *e, t_token *tokens)
{
	if (g_s2c_rpl_whois_state == 0)
		return (-1);
	if (strlen(tokens[1].addr) >
			(sizeof(g_s2c_rpl_whois_buffer) - strlen(g_s2c_rpl_whois_buffer)))
	{
		loginfo(g_s2c_rpl_whois_buffer);
		if (e->options.gui)
			ui_new_message(e->ui, g_s2c_rpl_whois_buffer, UI_INFO_MSG);
		memset(g_s2c_rpl_whois_buffer, 0, sizeof(g_s2c_rpl_whois_buffer));
	}
	strcat(g_s2c_rpl_whois_buffer, tokens[1].addr);
	strcat(g_s2c_rpl_whois_buffer, " ");
	return (IRC_S2C_RPL_WHOISCHANNELS);
}

int			s2c_rpl_endofwhois(t_env *e, t_token *tokens)
{
	(void)tokens;
	if (g_s2c_rpl_whois_state == 0)
		return (-1);
	loginfo(g_s2c_rpl_whois_buffer);
	g_s2c_rpl_whois_state = 0;
	if (e->options.gui)
		ui_whois(e->ui, g_s2c_rpl_whois_buffer);
	return (IRC_S2C_RPL_ENDOFWHOIS);
}
