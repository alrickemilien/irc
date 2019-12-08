#
# CLIENT
#

CLIENT=client

GTK_DPKG_CFLAGS=$(shell pkg-config --cflags gtk+-3.0)
GTK_DPKG_LIBS=$(shell pkg-config --libs gtk+-3.0)

CLIENT_SRC=src/client/main.c \
			src/client/gui.c \
			src/client/ipv4.c \
			src/client/ipv6.c \
			src/client/i18n.c \
			src/client/irc/tokenize.c \
			src/client/server_read.c \
			src/client/server_write.c \
			src/client/std/stdin_read.c \
			src/client/do_select.c

# options
CLIENT_SRC+=src/client/options/utils.c \
			src/client/options/read_options.c \
			src/client/options/read_command_option.c \
			src/client/options/read_port_option.c \
			src/client/options/read_host_option.c

# irc client to server
CLIENT_SRC+=src/client/irc/c2s/c2s.c \
			src/client/irc/c2s/c2s_join.c \
			src/client/irc/c2s/c2s_nick.c \
			src/client/irc/c2s/c2s_msg.c \
			src/client/irc/c2s/c2s_who.c \
			src/client/irc/c2s/c2s_pass.c \
			src/client/irc/c2s/c2s_away.c \
			src/client/irc/c2s/c2s_leave.c \
			src/client/irc/c2s/c2s_whois.c \
			src/client/irc/c2s/c2s_list.c \
			src/client/irc/c2s/c2s_time.c \
			src/client/irc/c2s/c2s_topic.c \
			src/client/irc/c2s/c2s_version.c \
			src/client/irc/c2s/c2s_invite.c \
			src/client/irc/c2s/c2s_connect.c

# irc server to client
CLIENT_SRC+=src/client/irc/s2c/s2c.c \
			src/client/irc/s2c/s2c_names.c \
			src/client/irc/s2c/s2c_nick.c \
			src/client/irc/s2c/s2c_join.c \
			src/client/irc/s2c/s2c_privmsg.c \
			src/client/irc/s2c/s2c_rpl_away.c \
			src/client/irc/s2c/s2c_rpl_topic.c \
			src/client/irc/s2c/s2c_rpl_whois.c \
			src/client/irc/s2c/s2c_rpl_who.c \
			src/client/irc/s2c/s2c_rpl_away.c \
			src/client/irc/s2c/s2c_rpl_list.c \
			src/client/irc/s2c/s2c_rpl_time.c \
			src/client/irc/s2c/s2c_rpl_version.c \
			src/client/irc/s2c/s2c_rpl_inviting.c \
			src/client/irc/s2c/s2c_invite.c \
			src/client/irc/s2c/s2c_ping.c \
			src/client/irc/s2c/s2c_rpl_welcome.c

# irc error
CLIENT_SRC+=src/client/irc/irc_error.c

# ssl
CLIENT_SRC+=src/client/ssl.c

#cbuffer
CLIENT_SRC+=src/cbuffer/cbuffer.c \
			src/cbuffer/cbuffer_indexof.c \
			src/cbuffer/cbuffer_io.c \
			src/cbuffer/cbuffer_utils.c \
			src/cbuffer/cbuffer_debug.c \
			src/cbuffer/cbuffer_ssl.c

# utils
CLIENT_SRC+=src/utils/log.c \
			src/utils/ato32.c \
			src/utils/extract_folder_from_path.c \
			src/utils/strjoin.c \
			src/utils/i64toa.c \
			src/utils/memrpl.c \
			src/utils/strtrim.c \
			src/utils/fmttime.c \
			src/utils/time2iso.c

# irc utils
CLIENT_SRC+=src/utils/irc/is_valid_channel.c \
			src/utils/irc/is_valid_nick.c

# ui
CLIENT_SRC+=src/client/ui/gtk_builder_load.c \
			src/client/ui/gtk_set_transparent_window.c \
			src/client/ui/gtk_do_select.c \
			src/client/ui/gtk_provider_load_css.c \
			src/client/ui/gtk_set_class.c \
			src/client/ui/gtk_new_rgba.c \
			src/client/ui/gtk_set_cursor_style.c \
			src/client/ui/gtk_pango_find_base_dir.c \
			src/client/ui/ui_utils.c \
			src/client/ui/gtk_get_assets.c

# ui login
CLIENT_SRC+=src/client/ui/login/login.c	\
			src/client/ui/login/ui_login_connect.c	\

# ui panel
CLIENT_SRC+=src/client/ui/panel/panel.c	\
			src/client/ui/panel/ui_join.c \
			src/client/ui/panel/ui_status.c \
			src/client/ui/panel/ui_who.c \
			src/client/ui/panel/ui_message.c \
			src/client/ui/panel/ui_events.c \
			src/client/ui/panel/ui_set.c \
			src/client/ui/panel/ui_leave.c \
			src/client/ui/panel/ui_chat.c

# ui msg
CLIENT_SRC+=src/client/ui/panel/ui_message/ui_chat_message.c

CLIENT_OBJ=$(CLIENT_SRC:.c=.o)

%.o: %.c
	@gcc $(DEBUG) -o $@ -c $< $(INCLUDE) $(OPENSSL_CFLAGS) $(GTK_DPKG_CFLAGS) $(CFLAGS)

# glade files + css
GLADE_SRC_PREFIX=src/client/ui/
GLADE_BUILD_PREFIX=build/ui/
GLADE_SRC=login/login.glade \
				login/login.css \
				panel/panel.glade \
				panel/panel.css
GLADE=$(patsubst %, $(GLADE_BUILD_PREFIX)%, $(GLADE_SRC))

$(GLADE_BUILD_PREFIX)%: $(GLADE_SRC_PREFIX)%
	@mkdir -p $(dir $@)
	@cp -f $< $@
