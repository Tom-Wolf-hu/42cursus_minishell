/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_line_analizer.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfarkas <tfarkas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 13:06:36 by tfarkas           #+#    #+#             */
/*   Updated: 2025/03/09 17:57:50 by tfarkas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	save_word(char *line, char *word, int *j)
{
	int		start;
	int		end;
	int		i;
	char	op;

	start = *j;
	i = 0;
	while (line[start] && ft_isspace(line[start]))
		start++;
	end = start;
	while (line[end] && !ft_isoperator(line[end]))
	{
		end++;
	}
	op = line[end];
	while (start < end && !ft_isspace(line[start]))
		word[i++] = line[start++];
	*j = end;
	return (op);
}

void	choose_action(char *beforeop, char *afterop, char op)
{
	if ((op == '<' || op == '>') && afterop)
		printf("Choose from redirection, with: %s\n", afterop);
	else if (op == '|' && beforeop && afterop)
		printf("Choose pipe with: %s, %s\n", beforeop, afterop);
	else if (op == '$' && afterop)
		printf("Choose expansion with: %s\n", afterop);
	else if ((op == '\'' || op == '\"') && afterop)
		printf("Choose from quotes with: %s\n", afterop);
	else if (op == '\0' && beforeop)
		printf("Choose command execution with: %s\n", beforeop);
	else
		printf("Something wnt wrong.\n");
}

void	analyze_line(char *line, int *j)
{
	char	*beforeop;
	char	*afterop;
	char	op1;
	char	op2;
	int		temp;

	(void)op2;
	beforeop = calloc(strlen(line), sizeof(char));
	afterop = calloc(strlen(line), sizeof(char));
	if (!beforeop || !afterop)
	{
		perror("Failed to allocate memory.");
		return ;
	}
	op1 = save_word(line, beforeop, j);
	if (op1 != '\0')
		*j += 1;
	temp = *j;
	op2 = save_word(line, afterop, &temp);
	choose_action(beforeop, afterop, op1);
	free(beforeop);
	free(afterop);
}

void	loop_analyzel(char *line)
{
	int	j;

	j = 0;
	if (!line)
		return ;
	while (line[j])
	{
		analyze_line(line, &j);
		while (line[j] && (ft_isspace(line[j]) || ft_isoperator(line[j])))
			j++;
		printf("line[j] in loop_analyzel: \t'%c'\n", line[j]);
	}
}
