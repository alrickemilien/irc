#
# CLIENT
#

CLIENT=client

CLIENT_SRC=src/client/main.c \
			src/client/client_ipv4.c \
			src/client/client_ipv6.c

# ui
CLIENT_SRC+=src/client/ui/login.c


CLIENT_OBJ=$(CLIENT_SRC:.c=.o)

GTK_DPKG=$(shell pkg-config --cflags --libs gtk+-2.0)

%.o: %.c
	gcc $(DEBUG) -o $@ -c $< $(INCLUDE) $(GTK_DPKG) $(CFLAGS)

# glade files
CLIENT_SRC_GLADE_PREFIX=src/client/ui/
CLIENT_BUILD_GLADE_PREFIX=build/ui/
CLIENT_SRC_GLADE=login.xml
CLIENT_UI_COPY_FILES=$(addprefix $(CLIENT_BUILD_GLADE_PREFIX), $(CLIENT_SRC_GLADE))

$(CLIENT_BUILD_GLADE_PREFIX)%: $(CLIENT_SRC_GLADE_PREFIX)%
	mkdir -p $(CLIENT_BUILD_GLADE_PREFIX)
	cp -f $< $@
