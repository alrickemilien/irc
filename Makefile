.PHONY: all clean fclean po

CFLAGS= -Wall -Wextra -Werror

BUILD_DIR=build/

LIBFT=libft
DIR_LIBFT=libft

# Set default C compiler if not set
ifeq ($(CC),)
CC=gcc
endif

IS_OPENSSL_DEV_INSTALLED=$(shell dpkg --status openssl 2>/dev/null 1>&2 ; echo $$?)
ifneq ($(IS_OPENSSL_DEV_INSTALLED), 0)
$(error openssl dev ibrary required for build)
endif
OPENSSL_CFLAGS=$(shell pkg-config --cflags openssl)
OPENSSL_LIBS=$(shell pkg-config --libs openssl)

include make/server.mk
include make/client.mk
include make/po.mk

INCLUDE=-I include -I libft

all: $(SERVER) $(CLIENT)

$(SERVER): $(BUILD_DIR)$(SERVER)
$(BUILD_DIR)$(SERVER): $(SERVER_OBJ)
	@mkdir -p $(BUILD_DIR)
	@gcc $(DEBUG) $^ -o $(BUILD_DIR)$@ $(LINK_LIBFT) $(OPENSSL_LIBS) $(CFLAGS)

$(CLIENT): $(BUILD_DIR)$(CLIENT) $(CLIENT_UI_COPY_FILES) $(ASSETS_COPY_FILES) $(PO)
$(BUILD_DIR)$(CLIENT): $(CLIENT_OBJ)
	@mkdir -p $(BUILD_DIR)
	@gcc $(DEBUG) $^ -o $@ $(LINK_LIBFT) $(GTK_DPKG_LIBS) $(OPENSSL_LIBS) $(CFLAGS)

clean:
	@rm -rf $(SERVER_OBJ) $(CLIENT_OBJ)

fclean: clean
	@rm -rf $(BUILD_DIR)

re: fclean all
