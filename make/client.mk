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
			src/client/irc/c2s/c2s_connect.c

#irc s2c
# CLIENT_SRC+=

#cbuffer
CLIENT_SRC+=src/cbuffer/cbuffer.c

# utils
CLIENT_SRC+=src/utils/safe.c \
			src/utils/log.c \
			src/utils/ato32.c \
			src/utils/i64toa.c \
			src/utils/time2iso.c

CLIENT_OBJ=$(CLIENT_SRC:.c=.o)
