#include <client/irc.h>
#include <client/ui/ui.h>

gboolean	gtk_do_select(void *e)
{
	do_select(e);
	return (TRUE);
}
