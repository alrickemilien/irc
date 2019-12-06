#include <server/irc.h>

int irc_pong(t_env *e, int cs, t_token *tokens)
{
    (void)e;
    (void)tokens;
    (void)cs;
    return (IRC_PONG);
}
