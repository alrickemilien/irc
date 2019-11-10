#
# CLIENT
#

CLIENT=client

CLIENT_SRC=src/client/main.c \
			src/client/client_ipv4.c \
			src/client/client_ipv6.c

# ui
CLIENT_SRC+=src/client/ui/login.c

# glade files
CLIENT_SRC_GLADE=src/client/ui/login.xml

CLIENT_OBJ=$(CLIENT_SRC:.c=.o)

GTK_DPKG=$(shell pkg-config --cflags --libs gtk+-2.0)

%.o: %.c
	@gcc $(DEBUG) -o $@ -c $< $(INCLUDE) $(GTK_DPKG) $(CFLAGS)

%.xml:
	mkdir -p $(BUILD_DIR)/ui
	cp $@ $(BUILD_DIR)/ui