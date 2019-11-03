#
# SERVER
#

SERVER=server

SERVER_SRC=src/server/main.c \
			src/server/serv.c \
			src/server/server_create.c \
			src/server/on_connect.c \
			src/server/do_select.c \
			src/server/client_read.c \
			src/server/client_write.c

# Utils
SERVER_SRC+=src/utils/safe.c

# fd
SERVER_SRC+=src/server/init_fd.c \
			src/server/check_fd.c \
			src/server/clear_fd.c

# ipv6
SERVER_SRC+=src/server/ipv6/create_server_ipv6.c

SERVER_OBJ=$(SERVER_SRC:.c=.o)
