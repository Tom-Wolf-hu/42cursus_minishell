/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_operations.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfarkas <tfarkas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 13:15:08 by tfarkas           #+#    #+#             */
/*   Updated: 2025/03/02 18:00:42 by tfarkas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	choose_redirection(t_tokentype e_red, char *name_d)
{
	if (!name_d)
	{
		ft_putendl_fd("Filename or delimiter does \
			not exist in choose_op.", STDERR_FILENO);
		return ;
	}
	if (e_red == REDINPUT)
		red_in(name_d);
	else if (e_red == REDOUTPUT)
		red_out(name_d);
	else if (e_red == APPENDREDOUTPUT)
		red_out_append(name_d);
	else if (e_red == REDDELIMETER)
		red_del(name_d);
}

// int	count_deilemeter(char *line, char delimeter)
// {
// 	int	count;

// 	count = 0;
// 	while (line[count] && (line[count] == delimeter || ft_isspace(line[count])))
// 		count++;
// 	if (count > 2)
// 	{
// 		ft_putendl_fd("Syntax error", STDERR_FILENO);
// 		exit(EXIT_FAILURE);
// 	}
// 	return (count);
// }

// char	*allocate_word(char *line, int len)
// {
// 	char	*word;
// 	int		i;

// 	i = 0;
// 	if (len < 1)
// 		return (NULL);
// 	word = (char *)malloc((len + 1) * sizeof(char));
// 	if (!word)
// 	{
// 		perror("Failed to allocate memory for word");
// 		return (NULL);
// 	}
// 	while (i < len)
// 	{
// 		word[i] = line[i];
// 		i++;
// 	}
// 	word[i] = '\0';
// 	return (word);
// }

// char	*save_w(char *line, char delimeter)
// {
// 	int		start;
// 	int		end;
// 	char	*word;

// 	start = 0;
// 	while (line[start] && line[start] != delimeter)
// 		start++;
// 	start = start + count_deilemeter(line + start, delimeter);
// 	end = start;
// 	while (line[end] && (!ft_isoperator(line[end]) || !ft_isspace(line[end])))
// 		end++;
// 	if (start == end)
// 		return (NULL);
// 	word = allocate_word(line + start, end - start);
// 	while (line[end] && !ft_isoperator(line[end]))
// 		end++;
// 	return (word);
// }

// void	check_redirect(char *line)
// {
// 	int 	i;
// 	char	*filename;	

// 	i = 0;
// 	while (line[i])
// 	{
// 		filename = save_w(line, '>', &i);
// 		if (filename)
			
// 	}
// }