/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_env.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfarkas <tfarkas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 16:10:44 by alex              #+#    #+#             */
/*   Updated: 2025/05/02 12:24:17 by tfarkas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	print_env(char ***myenvp)
{
	int	i;

	i = 0;
	if (!*myenvp)
		return (1);
	while ((*myenvp)[i])
	{
		if (ft_strnstr((*myenvp)[i], "=", ft_strlen((*myenvp)[i])) != NULL)
		{
			write(1, (*myenvp)[i], ft_strlen((*myenvp)[i]));
			write(1, "\n", 1);
		}
		i++;
	}
	return (0);
}

int	print_export(char ***myenvp)
{
	int	i;

	i = 0;
	if (!*myenvp)
		return (1);
	while ((*myenvp)[i])
	{
		write(1, "declare -x ", ft_strlen("declare -x "));
		write(1, (*myenvp)[i], ft_strlen((*myenvp)[i]));
		write(1, "\n", 1);
		i++;
	}
	return (0);
}

char	*my_getenv(char **myenvp, const char *name)
{
	int	len;
	int	i;

	if (!name)
		return (NULL);
	len = ft_strlen((char *)name);
	i = 0;
	while (myenvp[i])
	{
		if (ft_strncmp(myenvp[i], name, len) == 0 && myenvp[i][len] == '=')
			return (myenvp[i] + len + 1);
		i++;
	}
	return (NULL);
}
