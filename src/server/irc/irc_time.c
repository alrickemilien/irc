#include <server/irc.h>

int irc_time(t_env *e, int cs, t_token *tokens)
{
    char t[ISOTIMESTRSIZE];
    char local_hostname[NI_MAXHOST + 1];

    (void)tokens;

    if (gethostname(local_hostname, sizeof(local_hostname)) == -1)
        return (logerrno("irc_time::gethostname"));

    time2iso(t);
    irc_reply(e, cs, RPL_TIME, local_hostname, t);

    return (IRC_TIME);
}
