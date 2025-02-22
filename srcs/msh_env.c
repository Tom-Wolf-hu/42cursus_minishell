/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfarkas <tfarkas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 15:04:09 by tfarkas           #+#    #+#             */
/*   Updated: 2025/02/21 21:49:10 by tfarkas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*cmd_acces(char *path, char *cmd)
{
	char	*temp1;
	char	*temp2;

	if (!path || !cmd)
		return (NULL);
	temp1 = ft_strdup(path);
	temp2 = ft_strjoin(temp1, "/");
	free(temp1);
	temp1 = ft_strjoin(temp2, cmd);
	free(temp2);
	if (!temp1)
		return (NULL);
	if (access(temp1, X_OK) == 0)
		return (temp1);
	free(temp1);
	return (NULL);
}

/*
We need to freeze the result, after the usage.
*/
char	**dev_strarr(char *str, char delimeter)
{
	char	**result;

	result = NULL;
	if (!str || delimeter == '\0')
		return (NULL);
	result = ft_split(str, delimeter);
	if (!result)
		return (NULL);
	return (result);
}

/*
We need to freeze the env_var_val, after the usage.
*/
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

