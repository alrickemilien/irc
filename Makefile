CFLAGS= -Wall -Wextra -Werror

BUILD_DIR=build/

LIBFT=libft
DIR_LIBFT=libft
#LINK_LIBFT=-lft -L $(DIR_LIBFT)

# Set default C compiler if not set
ifeq ($(CC),)
CC=gcc
endif

include make/client.mk
include make/server.mk

INCLUDE=-I include -I libft

.PHONY: all clean fclean

all: $(SERVER) $(CLIENT) $(CLIENT_UI_COPY_FILES) $(ASSETS_COPY_FILES)

$(SERVER): $(SERVER_OBJ)
	@mkdir -p $(BUILD_DIR)
	@gcc $(DEBUG) $^ -o $(BUILD_DIR)$@ $(LINK_LIBFT) $(CFLAGS)

$(CLIENT): $(CLIENT_OBJ)
	@mkdir -p $(BUILD_DIR)
	@gcc $(DEBUG) $^ -o $(BUILD_DIR)$@ $(LINK_LIBFT) $(GTK_DPKG) $(CFLAGS)

clean:
	@rm -rf $(SERVER_OBJ) $(CLIENT_OBJ) $(CLIENT_UI_COPY_FILES) $(ASSETS_COPY_FILES)

fclean: clean
	@rm -rf $(SERVER) $(CLIENT)

re: fclean all
