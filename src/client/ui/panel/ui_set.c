#include <client/irc.h>
#include <client/ui/panel.h>

void	ui_set_channel_name(t_ui_panel *ui, const char *msg)
{
	ui->channel_label =
		GTK_WIDGET(gtk_builder_get_object(ui->builder, "channel_label"));
	gtk_label_set_text(GTK_LABEL(ui->channel_label), msg);
	gtk_label_set_selectable(GTK_LABEL(ui->channel_label), TRUE);
	gtk_widget_show_all(ui->channel_label);
}

void	ui_set_nick(t_ui_panel *ui, const char *msg)
{
	ui->nick_label =
		GTK_WIDGET(gtk_builder_get_object(ui->builder, "nickname_label"));
	gtk_label_set_text(GTK_LABEL(ui->nick_label), msg);
	gtk_label_set_selectable(GTK_LABEL(ui->nick_label), TRUE);
	gtk_widget_show_all(ui->nick_label);
}

void	ui_set_username(t_ui_panel *ui, const char *msg)
{
	ui->user_label =
		GTK_WIDGET(gtk_builder_get_object(ui->builder, "username_label"));
	gtk_label_set_text(GTK_LABEL(ui->user_label), msg);
	gtk_label_set_selectable(GTK_LABEL(ui->user_label), TRUE);
	gtk_widget_show_all(ui->user_label);
}
