/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_operations.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfarkas <tfarkas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 13:15:08 by tfarkas           #+#    #+#             */
/*   Updated: 2025/02/18 14:22:46 by tfarkas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	choose_op(t_redir e_red, char *name_d)
{
	if (!name_d)
	{
		ft_putendl_fd("File name or delimiter does \
			ot exist in choose_op.", STDERR_FILENO);
		return ;
	}
	if (e_red == REDINPUT)
		red_in(name_d);
	else if (e_red == REDOUTPUT)
		red_del(name_d);
	else if (e_red == APPENDREDOUTPUT)
		red_out_append(name_d);
	else if (e_red == REDDELIMETER)
		red_del(name_d);
}
