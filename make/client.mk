#
# CLIENT
#

CLIENT=client

CLIENT_SRC=src/client/main.c \
			src/client/ipv4.c \
			src/client/ipv6.c \
			src/server/cbuffer.c \
			src/client/irc/tokenize.c \
			src/client/server_read.c \
			src/client/server_write.c \
			src/client/do_select.c

# options
CLIENT_SRC+=src/client/options/utils.c \
			src/client/options/read_options.c \
			src/client/options/read_command_option.c \
			src/client/options/read_port_option.c \
			src/client/options/read_host_option.c

#irc
CLIENT_SRC+=src/client/irc/irc_join.c \
			src/client/irc/irc_nick.c \
			src/client/irc/irc_msg.c \
			src/client/irc/irc_command.c

# utils
CLIENT_SRC+=src/utils/safe.c \
			src/utils/log.c \
			src/utils/ato32.c \
			src/utils/i64toa.c \
			src/utils/time2iso.c

CLIENT_OBJ=$(CLIENT_SRC:.c=.o)

# ui
CLIENT_SRC+=src/client/ui/login.c


GTK_DPKG=$(shell pkg-config --cflags --libs gtk+-2.0)

%.o: %.c
	@gcc $(DEBUG) -o $@ -c $< $(INCLUDE) $(GTK_DPKG) $(CFLAGS)

# glade files
CLIENT_SRC_GLADE_PREFIX=src/client/ui/
CLIENT_BUILD_GLADE_PREFIX=build/ui/
CLIENT_SRC_GLADE=login.xml
CLIENT_UI_COPY_FILES=$(addprefix $(CLIENT_BUILD_GLADE_PREFIX), $(CLIENT_SRC_GLADE))

$(CLIENT_BUILD_GLADE_PREFIX)%: $(CLIENT_SRC_GLADE_PREFIX)%
	@mkdir -p $(CLIENT_BUILD_GLADE_PREFIX)
	@cp -f $< $@
