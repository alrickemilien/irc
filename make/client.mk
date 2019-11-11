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
			src/client/irc/irc_command.c

# utils
CLIENT_SRC+=src/utils/safe.c \
			src/utils/ato32.c \
			src/utils/i64toa.c \
			src/utils/time2iso.c

CLIENT_OBJ=$(CLIENT_SRC:.c=.o)
