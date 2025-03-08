NAME = minishell

CC = cc
CFLAGS = -Wall -Wextra -Werror
# -Wall -Wextra -Werror

SRCS =	srcs/minishell.c \
		srcs/check_line.c \
		srcs/echo.c \
		srcs/handle_export_unset.c \
		srcs/msh_redirect.c \
		srcs/msh_operations.c \
		srcs/msh_parse.c \
		srcs/msh_cmd.c \
		srcs/msh_env.c \
		srcs/check_var.c \
		srcs/msh_utils.c \
		srcs/msh_line_analizer.c \
		srcs/check_quastion_sign.c \
		srcs/msh_redir_cmd_call.c \
		srcs/msh_pipe.c \
		srcs/msh_redir_cmd_utils.c
# GNL_SRCS = lib/get_next_line/get_next_line.c lib/get_next_line/get_next_line_utils.c
LIBFT_DIR = lib/libft
LIBFT = libft.a
GNL_DIR = lib/get_next_line
GNL = get_next_line.a

OBJS_DIR = objs
OBJS = $(addprefix $(OBJS_DIR)/, $(notdir $(SRCS:.c=.o)))
# GNL_OBJS = $(addprefix $(OBJS_DIR)/, $(notdir $(GNL_SRCS:.c=.o)))
#notdir - оставляет только имена файлоа
#addprefix добавляет к каждому имени файла objs/

all: $(LIBFT) $(GNL) $(NAME)

# $(NAME): $(OBJS) $(GNL_OBJS)
# 	$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(GNL_OBJS) -L$(LIBFT_DIR) -lft -lreadline

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS) -L$(LIBFT_DIR) -lft -L$(GNL_DIR) -lget_next_line -lreadline

$(OBJS_DIR)/%.o: srcs/%.c
	mkdir -p $(OBJS_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# $(OBJS_DIR)/%.o: lib/get_next_line/%.c
# 	mkdir -p $(OBJS_DIR)
# 	$(CC) $(CFLAGS) -c $< -o $@

$(LIBFT):
	@$(MAKE) -C $(LIBFT_DIR)

$(GNL):
	@$(MAKE) -C $(GNL_DIR)
	mv $(GNL_DIR)/get_next_line.a $(GNL_DIR)/libget_next_line.a

re: fclean all

clean:
	rm -rf $(OBJS_DIR)
	@$(MAKE) -C $(LIBFT_DIR) clean
	@$(MAKE) -C $(GNL_DIR) clean

fclean: clean
	rm -rf $(NAME)
	@$(MAKE) -C $(LIBFT_DIR) fclean
	@$(MAKE) -C $(GNL_DIR) fclean

.PHONY: all re clean fclean