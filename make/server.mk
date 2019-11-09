#
# SERVER
#

SERVER=server

SERVER_SRC=src/server/main.c \
			src/server/serv.c \
			src/server/ipv4.c \
			src/server/ipv6.c \
			src/server/on_connect.c \
			src/server/broadcast.c \
			src/server/daemonize.c \
			src/server/do_select.c \
			src/server/cbuffer.c \
			src/server/client_read.c \
			src/server/client_write.c

# Utils
SERVER_SRC+=src/utils/safe.c \
			src/utils/ato32.c \
			src/utils/i64toa.c \
			src/utils/time2iso.c


# fd
SERVER_SRC+=src/server/fd/init_fd.c \
			src/server/fd/check_fd.c \
			src/server/fd/clear_fd.c

# irc
SERVER_SRC+=src/server/irc/irc_command.c \
			src/server/irc/tokenize.c \
			src/server/irc/irc_join.c \
			src/server/irc/irc_reply.c \
			src/server/irc/irc_nick.c \
			src/server/irc/irc_user.c \
			src/server/irc/irc_quit.c \
			src/server/irc/irc_msg.c

# options
SERVER_SRC+=src/server/options/utils.c \
			src/server/options/read_port_option.c \
			src/server/options/read_backlog_option.c \
			src/server/options/read_bind_option.c \
			src/server/options/read_options_arguments.c

SERVER_OBJ=$(SERVER_SRC:.c=.o)
