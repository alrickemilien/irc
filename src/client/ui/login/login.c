#include <client/irc.h>
#include <client/ui/login.h>

static gboolean	close_login(GtkWidget *widget,
                            GdkEventKey *event,
                            gpointer data)
{
    (void)widget;
    if (event->keyval == GDK_KEY_Escape)
    {
        ui_clear_login_window(data);
        gtk_main_quit();
    }
    return FALSE;
}

int				ui_clear_login_window(
	t_ui_login *ui)
{
    gtk_widget_hide(ui->window);
    free(ui->window_color);
    g_object_unref(G_OBJECT(ui->builder));
    free(ui);
    return (0);
}

static void		ui_login_connect_fetch_entrys(
	t_ui_login *ui)
{
    ui->host_entry =
        GTK_WIDGET(gtk_builder_get_object(ui->builder, "entry_host"));
    ui->port_entry =
        GTK_WIDGET(gtk_builder_get_object(ui->builder, "entry_port"));
    ui->username_entry =
        GTK_WIDGET(gtk_builder_get_object(ui->builder, "entry_name"));
    ui->pass_entry =
        GTK_WIDGET(gtk_builder_get_object(ui->builder, "entry_pass"));
    ui->nick_entry =
        GTK_WIDGET(gtk_builder_get_object(ui->builder, "entry_nick"));
}

static int		ui_init_login_window_credentials_entrys(
	t_env *e, t_ui_login *ui)
{
    char	portstr[PORTSTRSIZE];
    size_t	port_len;

    if (e->options.port != 0)
    {
        if ((port_len = i64toa(e->options.port, portstr, PORTSTRSIZE, 10)) ==
            (size_t)-1)
            (logerror("ui_init_login_window_credentials::i64toa"));
        else
            gtk_entry_set_text(GTK_ENTRY(ui->port_entry), portstr);
    }
    if (e->options.host[0] != 0)
        gtk_entry_set_text(GTK_ENTRY(ui->host_entry), e->options.host);
    return (0);
}

static int		ui_init_login_window(
	t_ui_login *ui)
{
    ui->window =
        GTK_WIDGET(gtk_builder_get_object(ui->builder, "window_login"));
    gtk_widget_add_events(ui->window, GDK_KEY_PRESS_MASK);
    g_signal_connect(ui->window, "key_press_event", G_CALLBACK(close_login),
                     ui);
    g_signal_connect(ui->window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    ui->window_color = gtk_new_rgba(1, 1, 1, 0.96);
    if (ui->window_color != NULL)
        gtk_set_transparent_window(ui->window, ui->window_color);
    ui->button_go =
        GTK_WIDGET(gtk_builder_get_object(ui->builder, "button_go"));
    g_signal_connect(ui->button_go, "clicked", G_CALLBACK(ui_login_connect),
                     ui);
    return (0);
}

int				ui_init_login(
	t_env *e, t_ui_login *ui)
{
    GtkCssProvider	*css;

    ui->e = e;
    ui->builder = gtk_builder_new();
    if (gtk_builder_load(ui->builder, e->argv_0, "/ui/login/login.glade") < 0)
        return (-1);
    ui_init_login_window(ui);
    css = gtk_css_provider_new();
    if (gtk_provider_load_css(css, e->argv_0, "/ui/login/login.css") < 0)
        return (-1);
    gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),
		GTK_STYLE_PROVIDER(css),
		GTK_STYLE_PROVIDER_PRIORITY_USER);
    g_object_unref(G_OBJECT(css));
    ui_login_connect_fetch_entrys(ui);
    ui_init_login_window_credentials_entrys(e, ui);
    gtk_widget_show_all(ui->window);
    return (0);
}
