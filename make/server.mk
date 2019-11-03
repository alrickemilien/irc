#
# SERVER
#

SERVER=server

SERVER_SRC=src/server/main.c \
			src/server/server_create.c \
			src/server/accept_connection.c \
			src/server/clearfd.c \
			src/server/client_read.c \
			src/server/client_write.c

SERVER_SRC+=src/utils/safe.c

SERVER_OBJ=$(SERVER_SRC:.c=.o)
