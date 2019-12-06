#include <server/irc.h>

/*
** "<version>.<debuglevel> <server> :<comments>"
*/

int irc_version(t_env *e, int cs, t_token *tokens)
{
    char local_hostname[NI_MAXHOST + 1];

    (void)tokens;

    if (gethostname(local_hostname, sizeof(local_hostname)) == -1)
        return (logerrno("irc_time::gethostname"));

    irc_reply(e, cs, RPL_VERSION, IRC_SERVER_VERSION, "0", local_hostname, IRC_SERVER_VERSION_COMMENT);

    return (IRC_VERSION);
}
