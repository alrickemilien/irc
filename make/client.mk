#
# CLIENT
#

CLIENT=client

CLIENT_SRC=src/client/main.c \
			src/client/ipv4.c \
			src/client/ipv6.c 

# options
CLIENT_SRC+=src/client/options/utils.c \
			src/client/options/read_options.c \
			src/client/options/read_port_option.c \
			src/client/options/read_host_option.c

# utils
CLIENT_SRC+=src/utils/safe.c \
			src/utils/ato32.c \
			src/utils/i64toa.c \
			src/utils/time2iso.c

CLIENT_OBJ=$(CLIENT_SRC:.c=.o)
