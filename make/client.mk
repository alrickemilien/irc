#
# CLIENT
#

CLIENT=client

CLIENT_SRC=src/client/main.c \
			src/client/client_ipv4.c \
			src/client/client_ipv6.c 


CLIENT_OBJ=$(CLIENT_SRC:.c=.o)
