/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_quotes.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfarkas <tfarkas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 18:25:56 by tfarkas           #+#    #+#             */
/*   Updated: 2025/04/03 12:28:05 by tfarkas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	write_stderr(char *str)
{
	int	strlen;

	if (!str)
		str = "Error occured.\n";
	strlen = ft_strlen(str);
	write(2, str, strlen);
}

int	check_quotes(char *line)
{
	int	i;
	int	j;
	int	end_q;

	i = 0;
	end_q = 1;
	while (line[i])
	{
		if (line[i] == '\'' || line[i] == '\"')
		{
			j = i;
			i++;
			end_q = 0;
			while (line[i])
			{
				if (line[i] == line[j])
				{
					end_q = 1;
					break;
				}
				i++;
			}
		}
		i++;
	}
	if (end_q == 0)
	{
		write_stderr("Please close the quotes!");
		write(STDOUT_FILENO, "\n", 1);
		return (1);
	}
	return (0);
}

char	**split_cmdarg(char *line)
{
	
}
