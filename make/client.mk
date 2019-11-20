#
# CLIENT
#

CLIENT=client

CLIENT_SRC=src/client/main.c \
			src/client/ipv4.c \
			src/client/ipv6.c \
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

#irc client to server
CLIENT_SRC+=src/client/irc/c2s/c2s.c \
			src/client/irc/c2s/c2s_join.c \
			src/client/irc/c2s/c2s_nick.c \
			src/client/irc/c2s/c2s_msg.c \
			src/client/irc/c2s/c2s_who.c \
			src/client/irc/c2s/c2s_pass.c \
			src/client/irc/c2s/c2s_connect.c

#irc server to client
CLIENT_SRC+=src/client/irc/s2c/s2c.c \
			src/client/irc/s2c/s2c_names.c \
			src/client/irc/s2c/s2c_privmsg.c \
			src/client/irc/s2c/s2c_rpl_welcome.c

#irc s2c
# CLIENT_SRC+=

#cbuffer
CLIENT_SRC+=src/cbuffer/cbuffer.c

# utils
CLIENT_SRC+=src/utils/safe.c \
			src/utils/log.c \
			src/utils/ato32.c \
			src/utils/extract_folder_from_path.c \
			src/utils/merge_and_extract_folder_from_path.c \
			src/utils/strjoin.c \
			src/utils/i64toa.c \
			src/utils/time2iso.c

CLIENT_OBJ=$(CLIENT_SRC:.c=.o)

# ui
CLIENT_SRC+=src/client/ui/gtk_builder_load.c \
			src/client/ui/gtk_provider_load_css.c \
			src/client/ui/login/login.c \
			src/client/ui/panel/panel.c


GTK_DPKG=$(shell pkg-config --cflags --libs gtk+-3.0)

%.o: %.c
	@gcc $(DEBUG) -o $@ -c $< $(INCLUDE) $(GTK_DPKG) $(CFLAGS)

# glade files
CLIENT_SRC_GLADE_PREFIX=src/client/ui/
CLIENT_BUILD_GLADE_PREFIX=build/ui/
CLIENT_SRC_GLADE=login/login.glade login/login.css \
				panel/panel.glade panel/panel.css
CLIENT_UI_COPY_FILES=$(addprefix $(CLIENT_BUILD_GLADE_PREFIX), $(CLIENT_SRC_GLADE))

$(CLIENT_BUILD_GLADE_PREFIX)%: $(CLIENT_SRC_GLADE_PREFIX)%
	@mkdir -p $(dir $@)
	@cp -f $< $@

# assets files
CLIENT_SRC_ASSETS_PREFIX=assets/
CLIENT_BUILD_ASSETS_PREFIX=build/ui/
CLIENT_SRC_ASSETS=login-1920x1080.jpg
ASSETS_COPY_FILES=$(addprefix $(CLIENT_BUILD_ASSETS_PREFIX), $(CLIENT_SRC_ASSETS))

$(CLIENT_BUILD_ASSETS_PREFIX)%: $(CLIENT_SRC_ASSETS_PREFIX)%
	@mkdir -p $(CLIENT_BUILD_ASSETS_PREFIX)
	@cp -f $< $@
