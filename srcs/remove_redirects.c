/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   remove_redirects.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omalovic <omalovic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 17:46:55 by tfarkas           #+#    #+#             */
/*   Updated: 2025/04/16 15:54:00 by omalovic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	jp_temp_s1(char *s1, char **temp, int *lens1, int lens2)
{
	int	i;

	i = 0;
	*lens1 = 0;
	if (s1 != NULL)
		*lens1 = ft_strlen(s1);
	*temp = (char *)malloc((*lens1 + lens2 + 1) * sizeof(char));
	if (!(*temp))
	{
		perror("Failed to allocate memory for temp in join part");
		exit(EXIT_FAILURE);
	}
	while (s1 && i < *lens1)
	{
		(*temp)[i] = s1[i];
		i++;
	}
}

void	join_part(char **s1, char *s2)
{
	int		lens1;
	int		lens2;
	char	*temp;
	int		i;

	i = 0;
	if (!s2)
		return ;
	lens2 = ft_strlen(s2);
	jp_temp_s1(*s1, &temp, &lens1, lens2);
	while (s2 && i < lens2)
	{
		temp[lens1 + i] = s2[i];
		i++;
	}
	temp[lens1 + i] = '\0';
	free(*s1);
	free(s2);
	s2 = NULL;
	*s1 = temp;
}

void	redir_part(char *cmd, int *i)
{
	while (cmd[*i] && (cmd[*i] == '<' || cmd[*i] == '>' || ft_isspace(cmd[*i])))
		(*i)++;
	while (cmd[*i] && !ft_isspace(cmd[*i]))
		(*i)++;
}

char	*before_red(char *cmd, int *i)
{
	int		start;
	int		j;
	char	*bef_red;

	start = *i;
	j = 0;
	while (cmd[*i] && cmd[*i] != '>' && cmd[*i] != '<')
		(*i)++;
	if (start == *i)
		return (NULL);
	bef_red = malloc((*i - start + 1));
	if (!bef_red)
	{
		perror("Failed to allocate memory for bef_red");
		exit(EXIT_FAILURE);
	}
	while (start < *i)
	{
		bef_red[j] = cmd[start];
		start++;
		j++;
	}
	bef_red[j] = '\0';
	return (bef_red);
}

char	*remove_redirects(char *cmd)
{
	char	*clean_cmd;
	char	*temp;
	int		i;

	i = 0;
	clean_cmd = NULL;
	while (cmd[i])
	{
		temp = before_red(cmd, &i);
		join_part(&clean_cmd, temp);
		redir_part(cmd, &i);
	}
	return (clean_cmd);
}

// void	join_part(char **s1, char *s2)
// {
// 	int		lens1;
// 	int		lens2;
// 	char	*temp;
// 	int		i;

// 	lens1 = 0;
// 	i = 0;
// 	if (*s1 != NULL)
// 		lens1 = ft_strlen(*s1);
// 	if (s2 == NULL)
// 		return ;
// 	lens2 = ft_strlen(s2);
// 	temp = (char *)malloc((lens1 + lens2 + 1) * sizeof(char));
// 	if (!temp)
// 	{
// 		perror("Failed to allocate memory for temp in join part");
// 		exit(EXIT_FAILURE);
// 	}
// 	while (*s1 && i < lens1)
// 	{
// 		temp[i] = (*s1)[i];
// 		i++;
// 	}
// 	i = 0;
// 	while (s2 && i < lens2)
// 	{
// 		temp[lens1 + i] = s2[i];
// 		i++;
// 	}
// 	temp[lens1 + i] = '\0';
// 	free(*s1);
// 	free(s2);
// 	s2 = NULL;
// 	*s1 = temp;
// }
