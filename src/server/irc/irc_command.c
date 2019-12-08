/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc_command.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aemilien <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/08 15:11:40 by aemilien          #+#    #+#             */
/*   Updated: 2019/12/08 15:11:41 by aemilien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <server/irc.h>

static const t_irc_cmd g_irc_commands[IRC_COMMANDS_NUMBER] = {
	[IRC_JOIN] = {"JOIN", &irc_join},
	[IRC_NICK] = {"NICK", &irc_nick},
	[IRC_USER] = {"USER", &irc_user},
	[IRC_QUIT] = {"QUIT", &irc_quit},
	[IRC_NAMES] = {"NAMES", &irc_names},
	[IRC_PRIVMSG] = {"PRIVMSG", &irc_privmsg},
	[IRC_NOTICE] = {"NOTICE", &irc_notice},
	[IRC_PASS] = {"PASS", &irc_pass},
	[IRC_AWAY] = {"AWAY", &irc_away},
	[IRC_PART] = {"PART", &irc_part},
	[IRC_WHOIS] = {"WHOIS", &irc_whois},
	[IRC_WHO] = {"WHO", &irc_who},
	[IRC_LIST] = {"LIST", &irc_list},
	[IRC_TIME] = {"TIME", &irc_time},
	[IRC_PING] = {"PING", &irc_ping},
	[IRC_PONG] = {"PONG", &irc_pong},
	[IRC_VERSION] = {"VERSION", &irc_version},
	[IRC_INVITE] = {"INVITE", &irc_invite},
	[IRC_TOPIC] = {"TOPIC", &irc_topic},
};

static void	extract_command(
		t_env *e, int cs,
		char *command, size_t end_command_index)
{
	memset(command, 0, CBUFFSIZE);
	if (e->fds[cs].buf_read.tail < end_command_index)
		memcpy(command, e->fds[cs].buf_read.buffer + e->fds[cs].buf_read.tail,
				end_command_index - e->fds[cs].buf_read.tail);
	else
	{
		memcpy(command, e->fds[cs].buf_read.buffer + e->fds[cs].buf_read.tail,
				CBUFFSIZE - e->fds[cs].buf_read.tail);
		memcpy(command + CBUFFSIZE - e->fds[cs].buf_read.tail,
				e->fds[cs].buf_read.buffer, end_command_index);
	}
}

int			irc_command(
		t_env *e, int cs, size_t end_command_index)
{
	size_t	i;
	size_t	offset;
	t_token	tokens[30];
	char	command[CBUFFSIZE + 1];

	extract_command(e, cs, command, end_command_index);
	offset = 0;
	while (command[offset] == 0x20)
		offset++;
	i = 0;
	while (i < IRC_COMMANDS_NUMBER)
	{
		if (strncmp(command + offset, g_irc_commands[i].command,
					strlen(g_irc_commands[i].command)) == 0)
		{
			tokenize(command + offset, tokens, 30);
			return (g_irc_commands[i].f(e, cs, tokens));
		}
		i++;
	}
	return (-1);
}
