#include <arpa/inet.h>
#include <client/irc.h>
#include <client/ui/panel.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

static gboolean on_keypress(GtkWidget *  widget,
                            GdkEventKey *event,
                            gpointer     data)
{
    (void)widget;
    (void)data;

    if (event->keyval == GDK_KEY_Escape)
        gtk_main_quit();
    return FALSE;
}

static void chat_entry_send(GtkWidget *widget, gpointer data)
{
    const char *text;
    t_env *     e;

    e = (t_env *)data;

    text = gtk_entry_get_text(GTK_ENTRY(widget));

    printf("Entry contents: %s\n", text);

    c2s(e, e->sock, strdup(text));

    gtk_entry_set_text(GTK_ENTRY(widget), "");
}

// void on_full(GtkWidget *widget, GtkAllocation *allocation, void *data) {
//     printf("width = %d, height = %d\n", allocation->width,
//     allocation->height);
// }

// g_signal_connect(mywidget, "size-allocate", G_CALLBACK(my_getsize), NULL);

void scroll_to_bottom(t_ui_panel *ui)
{
    static gdouble last_upper = 0;

    GtkAdjustment *verticalAdjust;
    gdouble        adjust = 0;
    gdouble        lower = 0;
    gdouble        upper = 0;
    gdouble        page_size = 0;
    gdouble        step_size = 0.1;

    ui->scrollwin =
        GTK_WIDGET(gtk_builder_get_object(ui->builder, "scrollwin"));
    gtk_widget_show_all(ui->scrollwin);

    verticalAdjust =
        gtk_scrolled_window_get_vadjustment(GTK_SCROLLED_WINDOW(ui->scrollwin));
    upper = gtk_adjustment_get_upper(verticalAdjust);

    if (last_upper == 0)
        last_upper = upper;
    else if (last_upper == upper)
        return;

    lower = gtk_adjustment_get_lower(verticalAdjust);
    upper = gtk_adjustment_get_upper(verticalAdjust);
    page_size = gtk_adjustment_get_page_size(verticalAdjust);
    step_size = ((upper - page_size) - lower) / page_size;

    logdebug("lower: %f\n", lower);
    logdebug("upper: %f\n", upper);
    logdebug("page_size: %f\n", page_size);
    logdebug("step_size: %f\n", step_size);
    logdebug("last_upper: %f\n", last_upper);

    adjust = upper - page_size - 1;

    logdebug("adjust: %f\n", adjust);
    logdebug("new adjust: %f\n",
             upper - page_size + (2 * upper - 2 * last_upper));

    gtk_adjustment_set_value(verticalAdjust,
                             upper - page_size + (upper - last_upper));
    gtk_scrolled_window_set_vadjustment(GTK_SCROLLED_WINDOW(ui->scrollwin),
                                        verticalAdjust);

    last_upper = upper;
    gtk_widget_show_all(ui->scrollwin);
}

void ui_new_chat_message(t_ui_panel *ui, const char *msg)
{
    GtkWidget *         w;
    t_ui_chat_msg_bloc *bloc;
    size_t              i;
    GList *             children;
    // char                t[ISOTIMESTRSIZE];

    // Message size + ISO date
    // char label[510 + ISOTIMESTRSIZE];

    logdebug("ui::ui_new_chat_message:: %s\n", msg);

    ui->chat_box = GTK_WIDGET(gtk_builder_get_object(ui->builder, "chat_box"));

    ui->msg_count++;

    i = 0;
    while (i < UI_CHAT_BOX_BLOC_MAX && ui->chat_msg_bloc_list[i].box &&
           ui->chat_msg_bloc_list[i].count == UI_CHAT_BOX_MSG_COUNT_MAX)
        i++;

    // When bloc msg list is full
    if (i < UI_CHAT_BOX_BLOC_MAX && ui->chat_msg_bloc_list[i].box == NULL)
    {
        ui->chat_msg_bloc_list[i].box = gtk_list_box_new();
        gtk_set_class(ui->chat_msg_bloc_list[i].box, "chat-message-box");
        ui->chat_msg_bloc_list[i].count = 0;
        bloc = &ui->chat_msg_bloc_list[i];

        // Insert into list of bloc message
        gtk_list_box_insert(GTK_LIST_BOX(ui->chat_box), bloc->box, -1);
    }
    else if (i == UI_CHAT_BOX_BLOC_MAX)
    {
        // Delete first element
        children = gtk_container_get_children(GTK_CONTAINER(ui->chat_box));
        gtk_widget_destroy(GTK_WIDGET(children->data));
        g_list_free(children);

        i = 0;
        while (i < (UI_CHAT_BOX_BLOC_MAX - 1))
        {
            ui->chat_msg_bloc_list[i].box = ui->chat_msg_bloc_list[i + 1].box;
            ui->chat_msg_bloc_list[i].count =
                ui->chat_msg_bloc_list[i + 1].count;

            // // Insert into list of bloc message
            // gtk_list_box_insert(GTK_LIST_BOX(ui->chat_box),
            //                     ui->chat_msg_bloc_list[i + 1].box, -1);
            i++;
        }

        bloc = &ui->chat_msg_bloc_list[i];
        bloc->box = gtk_list_box_new();
        gtk_set_class(bloc->box, "chat-message-box");
        bloc->count = 0;

        // Insert into list of bloc message
        gtk_list_box_insert(GTK_LIST_BOX(ui->chat_box), bloc->box, -1);
    }
    else
    {
        bloc = &ui->chat_msg_bloc_list[i];
    }

    // memset(label, msg, strlen(msg));
    // memset(label, msg, strlen(msg));

    // time2iso(t);
    // printf(is_tty ? "[%s] "
    //                 "\x1b[31m"
    //                 "ERROR:"
    //                 "\x1b[0m"
    //                 " %s: %s\n"
    //               : "[%s] ERROR: %s: %s\n",
    //        t, str,
    //        errno | h_errno ? strerror(errno | h_errno) : "Unknown error");

    w = gtk_label_new(msg);
    gtk_label_set_xalign(GTK_LABEL(w), 0);
    gtk_widget_set_margin_start(GTK_WIDGET(w), 12);
    gtk_list_box_insert(GTK_LIST_BOX(bloc->box), w, -1);
    bloc->count++;

    // if (bloc->count == CHAT_BOX_MSG_COUNT_MAX)
    // {
    //     w = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);
    //     gtk_set_class(ui->chat_msg_bloc_list[i].box,
    //                   "chat-message-box-separator");
    //     gtk_list_box_insert(GTK_LIST_BOX(ui->chat_box), w, -1);
    // }

    // End by scroll and show
    scroll_to_bottom(ui);
    gtk_widget_show_all(ui->chat_box);
}

void set_channel_name(t_ui_panel *ui, const char *msg)
{
    logdebug("ui::set_channel_name:: %s\n", msg);

    ui->channel_label =
        GTK_WIDGET(gtk_builder_get_object(ui->builder, "channel_label"));

    gtk_label_set_text(GTK_LABEL(ui->channel_label), msg);
    gtk_widget_show_all(ui->channel_label);
}

void ui_set_nick(t_ui_panel *ui, const char *msg)
{
    logdebug("ui::ui_set_nick:: %s\n", msg);

    ui->nick_label =
        GTK_WIDGET(gtk_builder_get_object(ui->builder, "nickname_label"));

    gtk_label_set_text(GTK_LABEL(ui->nick_label), msg);
    gtk_widget_show_all(ui->nick_label);
}

void set_user_name(t_ui_panel *ui, const char *msg)
{
    logdebug("ui::set_user_name:: %s\n", msg);

    ui->user_label =
        GTK_WIDGET(gtk_builder_get_object(ui->builder, "username_label"));

    gtk_label_set_text(GTK_LABEL(ui->user_label), msg);
    gtk_widget_show_all(ui->user_label);
}

int ui_set_status(t_ui_panel *ui, int status)
{
    ui->status_image =
        GTK_WIDGET(gtk_builder_get_object(ui->builder, "status_image"));

    printf("status_ok_image: %s\n", ui->status_ok_image);

    if (status == 0)
        gtk_image_set_from_file(GTK_IMAGE(ui->status_image),
                                ui->status_ok_image);

    if (status == 2)
        gtk_image_set_from_file(GTK_IMAGE(ui->status_image),
                                ui->status_away_image);

    return (0);
}

void ui_join_from_side_channel(GtkWidget *widget, gpointer data)
{
    const char *channel;
    t_ui_panel *ui;

    ui = (t_ui_panel *)data;

    channel = gtk_button_get_label(GTK_BUTTON(widget));

    logdebug("ui::ui_join_from_side_channel:: %s\n", channel);

    // When already into channel
    if (memcmp(ui->e->fds[ui->e->sock].channelname, channel, strlen(channel)) ==
        0)
        return;

    _c2s_join(&ui->e->fds[ui->e->sock], channel, strlen(channel));
}

int ui_join_channel_list_index_of(GtkWidget * list,
                                  const char *needle,
                                  size_t      needle_size)
{
    GList *     children;
    GList *     iter;
    int         i;
    const char *label;

    i = 0;
    children = gtk_container_get_children(GTK_CONTAINER(list));
    iter = children;
    while (iter != NULL)
    {
        label = gtk_button_get_label(
            GTK_BUTTON(gtk_bin_get_child(GTK_BIN(iter->data))));

        if (memcmp(label, needle, needle_size) == 0)
        {
            g_list_free(children);
            return (i);
        }

        iter = g_list_next(iter);

        i++;
    }

    g_list_free(children);

    return (-1);
}

int ui_join(t_ui_panel *ui, const char *channel)
{
    GtkWidget *w;

    logdebug("ui::ui_join:: %s\n", channel);

    set_channel_name(ui, channel);

    ui->channels_box =
        GTK_WIDGET(gtk_builder_get_object(ui->builder, "channels_box"));

    if (ui_join_channel_list_index_of(ui->channels_box, channel,
                                      strlen(channel)) != -1)
        return (0);

    ui->channels_count++;

    w = gtk_button_new();
    gtk_button_set_label(GTK_BUTTON(w), channel);
    gtk_set_class(w, "channels-box-item");
    g_signal_connect(w, "clicked", G_CALLBACK(ui_join_from_side_channel), ui);
    gtk_list_box_insert(GTK_LIST_BOX(ui->channels_box), w, -1);

    gtk_widget_show_all(ui->channels_box);

    return (0);
}

int ui_away(t_ui_panel *ui, const char *msg)
{
    logdebug("ui::ui_away:: %s\n", msg);

    ui_set_status(ui, 2);

    return (0);
}

int ui_unaway(t_ui_panel *ui)
{
    logdebug("ui::ui_unaway::\n");

    ui_set_status(ui, 0);

    return (0);
}

int ui_clear_panel_window(t_env *e, t_ui_panel *ui)
{
    (void)e;
    free(ui->window_color);
    g_object_unref(G_OBJECT(ui->builder));
    return (0);
}

int ui_init_panel_window(t_env *e, t_ui_panel *ui)
{
    GtkCssProvider *cssProvider;

    // Init builder
    ui->builder = gtk_builder_new();
    if (gtk_builder_load(ui->builder, e->argv_0, "/ui/panel/panel.glade") < 0)
        return (-1);

    // Init window
    ui->window =
        GTK_WIDGET(gtk_builder_get_object(ui->builder, "window_panel"));
    gtk_widget_add_events(ui->window, GDK_KEY_PRESS_MASK);
    g_signal_connect(ui->window, "key_press_event", G_CALLBACK(on_keypress),
                     NULL);
    g_signal_connect(ui->window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Init style css
    cssProvider = gtk_css_provider_new();
    if (gtk_provider_load_css(cssProvider, e->argv_0, "/ui/panel/panel.css") <
        0)
        return (-1);
    gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),
                                              GTK_STYLE_PROVIDER(cssProvider),
                                              GTK_STYLE_PROVIDER_PRIORITY_USER);
    g_object_unref(G_OBJECT(cssProvider));

    ui->chat_box = GTK_WIDGET(gtk_builder_get_object(ui->builder, "chat_box"));

    ui->msg_count = 0;

    memset(ui->chat_msg_bloc_list, 0, sizeof(ui->chat_msg_bloc_list));

    ui->chat_entry =
        GTK_WIDGET(gtk_builder_get_object(ui->builder, "chat_entry"));

    g_signal_connect(ui->chat_entry, "activate", G_CALLBACK(chat_entry_send),
                     e);

    ui->window_color = gtk_new_rgba(0.58431, 0.14902, 0.70196, 0.96);

    gtk_set_transparent_window(ui->window, ui->window_color);

    // Load assets
    ui->status_ok_image =
        gtk_get_assets(e->argv_0, "/ui/assets/icons8-ok-16.png");
    ui->status_not_ok_image =
        gtk_get_assets(e->argv_0, "/ui/assets/icons8-annuler-16.png");
    ui->status_away_image =
        gtk_get_assets(e->argv_0, "/ui/assets/icons8-mode-veille-16.png");

    ui->e = e;

    gtk_widget_show_all(ui->window);

    return (0);
}
