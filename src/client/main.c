#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "client/irc.h"
#include "client/ui/ui.h"
#include "client/ui/login.h"


int main(int argc,char *argv[]){
	
	gtk_init(&argc, &argv);
	
	login_window_init();

	g_object_unref(G_OBJECT(builder));

	gtk_widget_show_all(window);

	gtk_main();
	
    return 0;
}