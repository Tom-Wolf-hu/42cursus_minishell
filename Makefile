NAME = minishell

CC = cc
CFLAGS = -fsanitize=address -g -Wall -Wextra -Werror

SRCS =	srcs/minishell.c \
		srcs/msh_redirect.c \
		srcs/msh_operations.c
# GNL_SRCS = lib/get_next_line/get_next_line.c lib/get_next_line/get_next_line_utils.c
LIBFT_DIR = lib/libft
LIBFT = libft.a

OBJS_DIR = objs
OBJS = $(addprefix $(OBJS_DIR)/, $(notdir $(SRCS:.c=.o)))
GNL_OBJS = $(addprefix $(OBJS_DIR)/, $(notdir $(GNL_SRCS:.c=.o)))
#notdir - оставляет только имена файлоа
#addprefix добавляет к каждому имени файла objs/

all: $(LIBFT) $(NAME)

$(NAME): $(OBJS) $(GNL_OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(GNL_OBJS) -L$(LIBFT_DIR) -lft -lreadline

$(OBJS_DIR)/%.o: srcs/%.c
	mkdir -p $(OBJS_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJS_DIR)/%.o: lib/get_next_line/%.c
	mkdir -p $(OBJS_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(LIBFT):
	@$(MAKE) -C $(LIBFT_DIR)

re: fclean all

clean:
	rm -rf $(OBJS_DIR)
	@$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	rm -rf $(NAME)
	@$(MAKE) -C $(LIBFT_DIR) fclean

.PHONY: all re clean fclean