NAME = minishell

CC = cc
CFLAGS = 
# -Wall -Wextra -Werror

# Указываем все исходники вручную, включая файлы из подкаталогов
SRCS =	srcs/builtins/echo.c \
		srcs/builtins/get_pwd.c \
		srcs/builtins/handle_cd.c \
		srcs/builtins/handle_exit.c \
		srcs/builtins/handle_export_unset.c \
		srcs/builtins/handle_export_unset_utils.c \
		srcs/builtins/print_env.c \
		srcs/minishell_main.c \
		srcs/exec_cmd_single.c \
		srcs/exec_pipe_cmd.c \
		srcs/exec_pipe_cmd_utils.c \
		srcs/check_redir.c \
		srcs/check_redir_utils.c \
		srcs/handle_redirect.c \
		srcs/remove_redirects.c \
		srcs/minishell.c \
		srcs/minishell_utils.c \
		srcs/minishell_utils2.c \
		srcs/check_line.c \
		srcs/msh_cmd.c \
		srcs/msh_quotes.c \
		srcs/msh_quotes_utils.c \
		srcs/check_var.c \
		srcs/bridge_var.c \
		srcs/msh_utils.c \
		srcs/check_quastion_sign.c \
		lib/get_next_line/get_next_line.c \
		lib/get_next_line/get_next_line_utils.c \
		# srcs/msh_redirect.c \
		# srcs/msh_operations.c \
		# srcs/msh_parse.c \
		# srcs/msh_env.c \
		# srcs/msh_utils.c \
		# srcs/msh_line_analizer.c \
		# srcs/msh_redir_cmd_call.c \
		# srcs/msh_pipe.c \
		# srcs/msh_redir_cmd_utils.c \
		# srcs/test_funcs.c \

LIBFT_DIR = lib/libft
LIBFT = libft.a

OBJS_DIR = objs
OBJS = $(addprefix $(OBJS_DIR)/, $(notdir $(SRCS:.c=.o)))

all: $(LIBFT) $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS) -L$(LIBFT_DIR) -lft -lreadline

# Правило для компиляции исходных файлов из подкаталогов srcs
$(OBJS_DIR)/%.o: srcs/builtins/%.c
	mkdir -p $(OBJS_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Правило для других исходных файлов, например, из srcs
$(OBJS_DIR)/%.o: srcs/%.c
	mkdir -p $(OBJS_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Правило для файлов в lib/get_next_line
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
