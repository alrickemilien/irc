#
# SERVER
#

SERVER=server

SERVER_SRC=src/server/main.c \
			src/server/serv.c \
			src/server/server_create.c \
			src/server/accept_connection.c \
			src/server/do_select.c \
			src/server/client_read.c \
			src/server/client_write.c

# Utils
SERVER_SRC+=src/utils/safe.c

# fd
SERVER_SRC+=src/server/init_fd.c \
			src/server/check_fd.c \
			src/server/clear_fd.c

SERVER_OBJ=$(SERVER_SRC:.c=.o)
