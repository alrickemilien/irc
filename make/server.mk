#
# SERVER
#

SERVER=server

SERVER_SRC=src/server/main.c \
			src/server/ipv4.c \
			src/server/ipv6.c \
			src/server/on_connect.c \
			src/server/broadcast.c \
			src/server/disconnect.c \
			src/server/daemonize.c \
			src/server/do_select.c \
			src/server/client_read.c \
			src/server/client_write.c

# Utils
SERVER_SRC+=src/utils/log.c \
			src/utils/ato32.c \
			src/utils/i64toa.c \
			src/utils/memrpl.c \
			src/utils/time2iso.c

# irc utils
SERVER_SRC+=src/utils/irc/is_valid_channel.c

# fd
SERVER_SRC+=src/server/fd/init_fd.c \
			src/server/fd/check_fd.c \
			src/server/fd/clear_fd.c

# irc
SERVER_SRC+=src/server/irc/irc_command.c \
			src/server/irc/tokenize.c \
			src/server/irc/tokenizechr.c \
			src/server/irc/irc_join.c \
			src/server/irc/irc_reply.c \
			src/server/irc/irc_nick.c \
			src/server/irc/irc_user.c \
			src/server/irc/irc_quit.c \
			src/server/irc/irc_pass.c \
			src/server/irc/irc_notice.c \
			src/server/irc/irc_names.c \
			src/server/irc/irc_away.c \
			src/server/irc/irc_part.c \
			src/server/irc/irc_who.c \
			src/server/irc/irc_whois.c \
			src/server/irc/irc_list.c \
			src/server/irc/irc_time.c \
			src/server/irc/irc_privmsg.c

#cbuffer
SERVER_SRC+=src/cbuffer/cbuffer.c \
			src/cbuffer/cbuffer_io.c \
			src/cbuffer/cbuffer_utils.c \
			src/cbuffer/cbuffer_debug.c \
			src/cbuffer/cbuffer_ssl.c

# ssl
SERVER_SRC+=src/server/ssl.c

# options
SERVER_SRC+=src/server/options/utils.c \
			src/server/options/read_port_option.c \
			src/server/options/read_backlog_option.c \
			src/server/options/read_bind_option.c \
			src/server/options/read_ssl_key_file_option.c \
			src/server/options/read_ssl_crt_file_option.c \
			src/server/options/read_options_arguments.c

SERVER_OBJ=$(SERVER_SRC:.c=.o)

# %.o: %.c
# 	@gcc $(DEBUG) -o $@ -c $< $(INCLUDE) $(CFLAGS)
