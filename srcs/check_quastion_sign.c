/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_quastion_sign.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfarkas <tfarkas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 13:02:47 by omalovic          #+#    #+#             */
/*   Updated: 2025/05/02 12:35:40 by tfarkas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	change_to_exit_status(int i, char **line, char *status)
{
	char	*result;
	int		index;
	int		j;
	int		rest;

	result = malloc(strlen(*line) - 2 + strlen(status) + 1);
	if (!result)
		return ;
	index = 0;
	j = 0;
	while (index < i)
	{
		result[index] = (*line)[index];
		index++;
	}
	rest = index;
	while (status[j])
		result[index++] = status[j++];
	rest += 2;
	while ((*line)[rest])
		result[index++] = (*line)[rest++];
	result[index] = '\0';
	free(*line);
	*line = result;
}

int	check_quastion_sign(char **line, int status)
{
	int		i;
	int		flag_single;
	int		flag_double;
	char	*ch_status;

	i = 0;
	flag_single = 0;
	flag_double = 0;
	ch_status = ft_itoa(status);
	if (!ch_status)
		return (1);
	while ((*line)[i])
	{
		if ((*line)[i] == '\'' && flag_double == 0)
			flag_single = !flag_single;
		else if ((*line)[i] == '\"' && flag_single == 0)
			flag_double = !flag_double;
		if ((*line)[i] == '$' && flag_single == 0)
		{
			if ((*line)[i + 1] == '?')
				change_to_exit_status(i, line, ch_status);
		}
		i++;
	}
	return (free(ch_status), 0);
}
