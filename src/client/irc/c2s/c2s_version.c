#include <client/irc.h>

int	c2s_version(t_env *e, t_token *tokens)
{
	(void)tokens;
	if (e->sock == -1)
		return (irc_error(e, ERR_NOT_CONNECTED));
	cbuffer_putcmd(&e->self->buf_write, "VERSION\x0D\x0A");
	return (IRC_C2S_VERSION);
}
