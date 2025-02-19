/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_export_unset.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omalovic <omalovic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 12:49:13 by omalovic          #+#    #+#             */
/*   Updated: 2025/02/19 12:49:29 by omalovic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	handle_export(char *line)
{
	char	*arg;
	char	*equals_pos;
	char	*value;

	if (ft_strlen(line) == 6 || check_line(line, 7))
		print_env();
	else
	{
		arg = line + 7;
		equals_pos = ft_strchr(arg, '=');
		if (equals_pos != NULL)
		{
			*equals_pos = '\0';
			value = equals_pos + 1;
			if (setenv(arg, value, 1) == -1)
				perror("setenv");
		}
		else
			printf("export: invalid syntax\n");
	}
}

void	handle_unset(char *line)
{
	char	*arg;

	arg = line + 6;
	if (unsetenv(arg) == -1)
		perror("unsetenv");
}
