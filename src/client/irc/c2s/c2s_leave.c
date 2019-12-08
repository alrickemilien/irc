#include <client/irc.h>
#include <client/ui/panel.h>

/*
** the only restriction on a
** channel name is that it may not contain any spaces (' '), a control G
** (^G or ASCII 7), or a comma (',' which is used as a list item
** separator by the protocol)
*/

static int	c2s_leave_check_command(
		t_env *e, const t_token *tokens)
{
	const char	*channel;
	size_t		channel_len;

	(void)e;

	if (!tokens[1].addr)
		return (0);
	channel = tokens[1].addr;
	channel_len = tokens[1].len;
	if (channel_len - 1 > CHANNELSTRSIZE)
		return (irc_error(e, ERR_NOSUCHCHANNEL, channel));
	else if (!is_valid_chan_name(channel, channel_len))
		return (irc_error(e, ERR_NOSUCHCHANNEL, channel));
	else if (channel_len < 1)
		return (irc_error(e, ERR_NOSUCHCHANNEL, channel));
	return (0);
}

int			do_c2s_leave(
	t_fd *fd, const char *channel_name, size_t channel_name_len)
{
	return (cbuffer_putcmd(&fd->buf_write, "PART %.*s\x0D\x0A",
		channel_name_len, channel_name));
}

int			c2s_leave(t_env *e, t_token *tokens)
{
	if (e->sock == -1)
		return (irc_error(e, ERR_NOT_CONNECTED));
	if ((c2s_leave_check_command(e, tokens)) != 0)
		return (-1);
	do_c2s_leave(e->self,
		tokens[1].addr ? tokens[1].addr : e->self->channelname,
		tokens[1].addr ? tokens[1].len : strlen(e->self->channelname));
	if (e->options.gui &&
			ui_leave(e->ui,
				tokens[1].addr ? tokens[1].addr : e->self->channelname) < 0)
		return (-1);
	return (IRC_C2S_LEAVE);
}
