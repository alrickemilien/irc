#include <client/irc.h>
#include <client/ui/login.h>
#include <client/ui/panel.h>

int s2c_err_alreadyregistered(t_env *e, t_token *tokens)
{
	(void)e;

	logerror("%s",
            tokens[1].addr[0] == ':' ? tokens[1].addr + 1 : tokens[1].addr);

    return (IRC_S2C_ERR_ALREADYREGISTERED);
}
