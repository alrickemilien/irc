#include <client/irc.h>
#include <client/ui/login.h>

static gboolean close_login(GtkWidget *  widget,
                            GdkEventKey *event,
                            gpointer     data)
{
    (void)widget;

    if (event->keyval == GDK_KEY_Escape)
    {
        ui_clear_login_window(data);
        gtk_main_quit();
    }
    return FALSE;
}

int ui_clear_login_window(t_ui_login *ui)
{
    free(ui->window_color);
    g_object_unref(G_OBJECT(ui->builder));
    return (0);
}

int ui_init_login_window(t_env *e, t_ui_login *ui)
{
    GtkCssProvider *css_provider;

    ui->e = e;

    ui->builder = gtk_builder_new();
    if (gtk_builder_load(ui->builder, e->argv_0, "/ui/login/login.glade") < 0)
        return (-1);

    // Init window and close events
    ui->window =
        GTK_WIDGET(gtk_builder_get_object(ui->builder, "window_login"));

    gtk_widget_add_events(ui->window, GDK_KEY_PRESS_MASK);
    g_signal_connect(ui->window, "key_press_event", G_CALLBACK(close_login),
                     ui);
    g_signal_connect(ui->window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Set login button event
    ui->button_go =
        GTK_WIDGET(gtk_builder_get_object(ui->builder, "button_go"));
    g_signal_connect(ui->button_go, "clicked", G_CALLBACK(ui_login_connect),
                     ui);

    // Apply style to window
    css_provider = gtk_css_provider_new();
    if (gtk_provider_load_css(css_provider, e->argv_0, "/ui/login/login.css") <
        0)
        return (-1);
    gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),
                                              GTK_STYLE_PROVIDER(css_provider),
                                              GTK_STYLE_PROVIDER_PRIORITY_USER);

    ui->window_color = gtk_new_rgba(1, 1, 1, 0.96);

    gtk_set_transparent_window(ui->window, ui->window_color);

    g_object_unref(G_OBJECT(css_provider));

    gtk_widget_show_all(ui->window);

    return (0);
}