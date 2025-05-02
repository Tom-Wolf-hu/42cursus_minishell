/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   remove_redirects.utils.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfarkas <tfarkas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 17:19:57 by tfarkas           #+#    #+#             */
/*   Updated: 2025/05/02 17:21:02 by tfarkas          ###   ########.fr       */
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
	*s1 = temp;
}
