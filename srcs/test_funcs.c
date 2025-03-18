/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_funcs.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfarkas <tfarkas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 16:35:10 by tfarkas           #+#    #+#             */
/*   Updated: 2025/03/18 13:26:32 by tfarkas          ###   ########.fr       */
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

void check_tty()
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

void	wr_openfd(t_store *st, char *name, FILE *fd_monit)
{
	fprintf(fd_monit, "\033[1;32mThe %s's fd[%d] is opened\
		in the %d process.\033[0m\n", name, st->fd_exin, 1);
}

void	monitor_fds(t_store *st)
{
	char	*monitor_file;
	char	*name;
	FILE	*fd_monit;

	name = "Execution input";
	monitor_file = ".fds_monitoring.txt";
	(void)st;
	fd_monit = fopen(monitor_file, "a");
	if (!fd_monit)
	{
		perror("Failed to open temporaryfile for fds monitoring");
		exit(EXIT_FAILURE);
	}
	fprintf(fd_monit, "\033[1;32mProba %d\033[0m\n", 12);
	(void)name;
	fclose(fd_monit);
	// delete_file(monitor_file);
}
