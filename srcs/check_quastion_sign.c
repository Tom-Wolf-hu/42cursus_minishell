/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_quastion_sign.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 13:02:47 by omalovic          #+#    #+#             */
/*   Updated: 2025/04/11 13:09:28 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void change_to_exit_status(int i, char **line, char *status)
{
	char *result;
	int	index;
	int	j;
	int	rest;

	result = malloc(strlen(*line) - 2 + strlen(status) + 1);
	if (!result)
		return ;
	index = 0;
	j = 0;
	while (index < i) // overwriting TILL status
	{
		result[index] = (*line)[index];
		index++;
	}
	rest = index;
	while (status[j]) // overwriting the status
	{
		result[index] = status[j];
		j++;
		index++;
	}
	rest += 2; // pass the $?
	while ((*line)[rest]) // overwriting after the status
	{
		result[index] = (*line)[rest];
		index++;
		rest++;
	}
	result[index] = '\0';
	free(*line);
	*line = result;
}

int check_quastion_sign(char **line, int wstatus)
{
    int i = 0;
    int flag_single = 0;
    int flag_double = 0;
	char	*status;

	status = ft_itoa(wstatus);
	if (!status)
		exit(1);
    while ((*line)[i])
    {
        if ((*line)[i] == '\'' && flag_double == 0)
            flag_single = !flag_single;
        else if ((*line)[i] == '\"' && flag_single == 0)
            flag_double = !flag_double;
        if ((*line)[i] == '$' && flag_single == 0)
        {
            if ((*line)[i + 1] == '?')
                change_to_exit_status(i, line, status);
        }
        i++;
    }
    free(status);
    return (0);
}

// int	check_quastion_sign(char **line, char *status)
// {
// 	int	i;
// 	int	flag;

// 	flag = 0;
// 	i = 0;
// 	while ((*line)[i])
// 	{
// 		if ((*line)[i] == '\'' && flag == 0)
// 			flag = 1;
// 		else if ((*line)[i] == '\'' && flag == 1)
// 			flag = 0;
// 		if ((*line)[i] == '$' && flag == 0)
// 		{
// 			if ((*line)[i + 1] == '?')
// 			{
// 				change_to_exit_status(i, line, status);
// 				i = -1;
// 			}
// 		}
// 		i++;
// 	}
// 	free(status);
// 	return (0);
// }

// int main()
// {
// 	char *str1 = strdup("Hello, world! \'$?\'");
// 	char *num = strdup("255");
// 	check_quastion_sign(&str1, num);
// 	printf("%s\n", str1);
// }