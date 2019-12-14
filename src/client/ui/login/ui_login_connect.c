/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui_login_connect.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aemilien <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/14 13:35:07 by aemilien          #+#    #+#             */
/*   Updated: 2019/12/14 13:35:09 by aemilien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <arpa/inet.h>
#include <client/irc.h>
#include <client/ui/login.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

static int	ui_login_connect_fetch_entrys_content(t_env *e,
		t_ui_login *ui,
		t_ui_credentials *crd)
{
	crd->host = gtk_entry_get_text(GTK_ENTRY(ui->host_entry));
	crd->port = gtk_entry_get_text(GTK_ENTRY(ui->port_entry));
	crd->username = gtk_entry_get_text(GTK_ENTRY(ui->username_entry));
	crd->pass = gtk_entry_get_text(GTK_ENTRY(ui->pass_entry));
	crd->nick = gtk_entry_get_text(GTK_ENTRY(ui->nick_entry));
	if (!crd->nick || crd->nick[0] == 0)
	{
		logerror("Nickname must be provided\n");
		return (-1);
	}
	if (ato32(crd->port[0] ? crd->port : "5555",
				(uint32_t *)&e->options.port) != 0 ||
			e->options.port < 3 || e->options.port > 99999)
	{
		logerror("port must be a vakue between 1000 an 99999'%s'.\n",
				crd->port ? crd->port : "5555");
		return (-1);
	}
	memcpy(e->options.host, crd->host && crd->host[0] ? crd->host : "127.0.0.1",
		crd->host && crd->host[0] ? strlen(crd->host) : strlen("127.0.0.1"));
	return (0);
}

void		ui_login_connect(
	GtkWidget *widget, gpointer data)
{
	t_env				*e;
	t_ui_login			*ui;
	t_ui_credentials	crd;

	(void)widget;
	ui = (t_ui_login *)data;
	e = ui->e;
	gtk_button_set_label(GTK_BUTTON(ui->button_go), "Connecting ...");
	if (ui_login_connect_fetch_entrys_content(e, ui, &crd) < 0)
		return ;
	do_c2s_pass(e, strtrim(crd.pass), strlentrim(crd.pass));
	do_c2s_nick(e, strtrim(crd.nick), strlentrim(crd.nick));
	do_c2s_connect(e, crd.username[0] ? crd.username : NULL, NULL,
			e->options.host);
	if (e->sock != -1)
		g_idle_add((GSourceFunc)gtk_do_select, e);
	else
	{
		gtk_entry_set_text(GTK_ENTRY(ui->pass_entry), "");
		gtk_button_set_label(GTK_BUTTON(ui->button_go), "Log in");
	}
}
