/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_redir_cmd_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omalovic <omalovic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 13:53:33 by tfarkas           #+#    #+#             */
/*   Updated: 2025/03/03 14:45:34 by omalovic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_store	*init_store(void)
{
	t_store	*st;

	st->save_stdin = dup(STDIN_FILENO);
	if (st->save_stdin < 0)
	{
		perror("Failed to duplicate standard input");
		return (NULL);
	}
	st->save_stdout = dup(STDOUT_FILENO);
	if (st->save_stdout < 0)
	{
		perror("Failed to duplicate standard output");
		return (NULL);
	}
	st->childs = NULL;
	return (st);
}

