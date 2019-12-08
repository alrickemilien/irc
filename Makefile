.PHONY: all clean fclean po

CFLAGS= -Wall -Wextra -Werror

BUILD_DIR=build/

LIBFT=libft
DIR_LIBFT=libft

# Set default C compiler if not set
ifeq ($(CC),)
CC=gcc
endif

OS=$(shell uname)

#
# Open SSL
#

ifeq ($(OS),Darwin)
IS_OPENSSL_DEV_INSTALLED=$(shell brew info openssl 2>/dev/null 1>&2 ; echo $$?)
ifneq ($(IS_OPENSSL_DEV_INSTALLED), 0)
$(error openssl dev library required for build)
endif
OPENSSL_CFLAGS=$(shell pkg-config --cflags openssl)
OPENSSL_LIBS=$(shell pkg-config --libs openssl)
else
IS_OPENSSL_DEV_INSTALLED=$(shell dpkg --status openssl 2>/dev/null 1>&2 ; echo $$?)
ifneq ($(IS_OPENSSL_DEV_INSTALLED), 0)
$(error openssl dev library required for build)
endif
OPENSSL_CFLAGS=$(shell pkg-config --cflags openssl)
OPENSSL_LIBS=$(shell pkg-config --libs openssl)
endif

#
# Debug
#

ifeq ($(DEBUG),false)
DEBUG=-D DEBUG=0
endif

#
# Fribidi
#

ifeq ($(OS),Darwin)
FRIBIDI_DPKG_LIBS=$(shell pkg-config --libs fribidi)
endif

include make/server.mk
include make/client.mk
include make/po.mk
include make/assets.mk

INCLUDE=-I include -I libft

all: $(SERVER) $(CLIENT)

$(SERVER): $(BUILD_DIR)$(SERVER)
$(BUILD_DIR)$(SERVER): $(SERVER_OBJ)
	@mkdir -p $(BUILD_DIR)
	@gcc $(DEBUG) $^ -o $@ $(LINK_LIBFT) $(OPENSSL_LIBS) $(CFLAGS)

$(CLIENT): $(BUILD_DIR)$(CLIENT) $(GLADE) $(ASSETS) $(MO)
$(BUILD_DIR)$(CLIENT): $(CLIENT_OBJ)
	@mkdir -p $(BUILD_DIR)
	@gcc $(DEBUG) $^ -o $@ $(LINK_LIBFT) $(GTK_DPKG_LIBS) $(OPENSSL_LIBS) $(CFLAGS) $(FRIBIDI_DPKG_LIBS)

clean:
	@rm -rf $(SERVER_OBJ) $(CLIENT_OBJ)

fclean: clean
	@rm -rf $(BUILD_DIR)

re: fclean all
