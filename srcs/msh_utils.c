/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfarkas <tfarkas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 13:22:01 by tfarkas           #+#    #+#             */
/*   Updated: 2025/02/22 13:48:52 by tfarkas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	print_arr(char **strarr)
{
	int	i;

	i = 0;
	if (!strarr)
	{
		printf("The stringarray point to NULL.\n");
		return ;
	}
	while (strarr[i])
	{
		printf("[%i] -> %s\n", i, strarr[i]);
		i++;
	}
}
