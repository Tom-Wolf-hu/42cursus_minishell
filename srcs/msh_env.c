/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfarkas <tfarkas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 15:04:09 by tfarkas           #+#    #+#             */
/*   Updated: 2025/02/20 15:46:48 by tfarkas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*env_variable_val(char *variable_name)
{
	char	*env_var_val;

	if (!variable_name)
		return (NULL);
	env_var_val = getenv(variable_name);
	if (!env_var_val)
		printf("The '%s' variable is not found.\n", variable_name);
	return (env_var_val);
}

