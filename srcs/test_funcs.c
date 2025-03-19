/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_funcs.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfarkas <tfarkas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 16:35:10 by tfarkas           #+#    #+#             */
/*   Updated: 2025/03/18 19:56:24 by tfarkas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	fds_state(void)
{
	int	fd_i;

	fd_i = 0;
	while (fd_i < 25)
	{
		if (fcntl(fd_i, F_GETFD) != -1)
			printf("\033[1;32mThe [%d] fd is open.\033[0m\n", fd_i);
		else
			printf("\033[1;31mThe [%d] fd is closed.\033[0m\n", fd_i);
		fd_i++;
	}
}

void check_tty(void)
{
	if (isatty(STDIN_FILENO))
		printf("\033[1;33mSTDIN is a terminal.\033[0m\n");
	else
		printf("\033[1;31mSTDIN NOT a terminal.\033[0m\n");
	if (isatty(STDOUT_FILENO))
		printf("\033[1;32mSTDOUT is a terminal.\033[0m\n");
	else
		printf("\033[1;31mSTDOUT NOT a terminal.\033[0m\n");
}

void	fds_state_f(FILE *fd_monit)
{
	int	fd_i;

	fd_i = 0;
	while (fd_i < 12)
	{
		if (fcntl(fd_i, F_GETFD) != -1)
			fprintf(fd_monit, "\033[1;32mThe [%d] fd is open.\033[0m\n", fd_i);
		else
			fprintf(fd_monit, "\033[1;31mThe [%d] fd is closed.\033[0m\n", fd_i);
		fd_i++;
	}
}

void	init_fds_struct(int fd_struct[9], t_store *st)
{
	fd_struct[0] = st->save_stdin;
	fd_struct[1] = st->save_stdout;
	fd_struct[2] = st->fd_readl;
	fd_struct[3] = st->fd_exin;
	fd_struct[4] = st->fd_exout;
	fd_struct[5] = st->pipefd[0][0];
	fd_struct[6] = st->pipefd[0][1];
	fd_struct[7] = st->pipefd[1][0];
	fd_struct[8] = st->pipefd[1][1];
}

void	struct_fds_state(FILE *fd_monit, t_store *st)
{
	int	fd_struct[9];
	int	fd_i;

	fd_i = 0;
	init_fds_struct(fd_struct, st);
	while (fd_i < 9)
	{
		if (fcntl(fd_struct[fd_i], F_GETFD) != -1)
			fprintf(fd_monit, "\033[1;32mThe [%d] fd is open.\033[0m\n", fd_struct[fd_i]);
		else
			fprintf(fd_monit, "\033[1;31mThe [%d] fd is closed.\033[0m\n", fd_struct[fd_i]);
		fd_i++;
	}
}

void check_tty_f(FILE *fd_monit)
{
	if (isatty(STDIN_FILENO))
		fprintf(fd_monit, "\033[1;33mSTDIN is a terminal.\033[0m\n");
	else
		fprintf(fd_monit, "\033[1;31mSTDIN NOT a terminal.\033[0m\n");
	if (isatty(STDOUT_FILENO))
		fprintf(fd_monit, "\033[1;32mSTDOUT is a terminal.\033[0m\n");
	else
		fprintf(fd_monit, "\033[1;31mSTDOUT NOT a terminal.\033[0m\n");
}

void	delete_file(char *monitor_file)
{
	if (remove(monitor_file) == 0)
		printf("\033[1;32mThe file %s succesfully deleted.\033[0m\n", monitor_file);
	else
	{
		perror("\033[1;31mFailed to delete the file\033[0m");
		exit(EXIT_FAILURE);
	}
}

void	wr_openfd(int fd, char *name, FILE *fd_monit, t_wopt wopt)
{
	pid_t	pid;

	pid = getpid();
	if (wopt == OPEN_FD)
	{
		fprintf(fd_monit, "\033[1;32mThe %s's fd[%d] is OPENED"
			" in the %d process.\033[0m\n", name, fd, pid);
	}
	else if (wopt == CLOSE_FD)
	{
		fprintf(fd_monit, "\033[1;31mThe %s's fd[%d] is CLOSED"
			" in the %d process.\033[0m\n", name, fd, pid);
	}
	else if (wopt == DUPLICATE_FD)
	{
		fprintf(fd_monit, "\033[1;33mThe %s's fd[%d] is duplicated"
			" in the %d process.\033[0m\n", name, fd, pid);
	}
}

void	fds_type(t_fds fds, t_store *st, int *fd, char **name)
{
	if (fds == SAVE_STDIN)
	{
		*fd = st->save_stdin;
		*name = "Save Standard Input";
	}
	else if (fds == SAVE_STDOUT)
	{
		*fd = st->save_stdout;
		*name = "Save Standard Output";
	}
	else if (fds == FD_READL)
	{
		*fd = st->fd_readl;
		*name = "Temporary readline file";
	}
	else if (fds == FD_EXIN)
	{
		*fd = st->fd_exin;
		*name = "Execution Input";
	}
	else if (fds == FD_EXOUT)
	{
		*fd = st->fd_exout;
		*name = "Execution Output";
	}
	else if (fds == PIPEFD_R)
	{
		*fd = st->pipefd[st->cmd_num % 2][0];
		*name = "Pipe read";
	}
	else if (fds == PIPEFD_W)
	{
		*fd = st->pipefd[st->cmd_num % 2][1];
		*name = "Pipe write";
	}
}

void	fd_start_end(FILE *fd_monit, t_wopt wopt)
{
	static int	count = 0;

	if (wopt == START_FD)
	{
		fprintf(fd_monit, "%d. START fds state:\n", count);
		fds_state_f(fd_monit);
	}
	else if (wopt == END_FD)
	{
		fprintf(fd_monit, "%d. END fds state:\n", count);
		fds_state_f(fd_monit);
	}
	count++;
}

void	monitor_fds(t_store *st, t_fds fds, t_wopt wopt)
{
	char	*monitor_file;
	char	*name;
	FILE	*fd_monit;
	int		fd;

	fd = -1;
	name = NULL;
	monitor_file = ".fds_monitoring.txt";
	fds_type(fds, st, &fd, &name);
	fd_monit = fopen(monitor_file, "a");
	if (!fd_monit)
	{
		perror("Failed to open temporaryfile for fds monitoring");
		exit(EXIT_FAILURE);
	}
	// fprintf(fd_monit, "\033[1;32mProba %d\033[0m\n", 12);
	if (wopt == START_FD || wopt == END_FD)
		fd_start_end(fd_monit, wopt);
	else
	{
		wr_openfd(fd, name, fd_monit, wopt);
		fprintf(fd_monit, "\n");
		struct_fds_state(fd_monit, st);
	}
	// fds_state_f(fd_monit);
	fprintf(fd_monit, "\n--------------------------------------------\n\n");
	// check_tty_f(fd_monit);
	fclose(fd_monit);
	// delete_file(monitor_file);
}
