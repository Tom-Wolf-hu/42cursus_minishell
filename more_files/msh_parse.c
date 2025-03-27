/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_parse.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfarkas <tfarkas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 16:40:54 by tfarkas           #+#    #+#             */
/*   Updated: 2025/02/18 19:16:41 by tfarkas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	add_node(t_pnode **node)
{
	t_pnode	*new_node;

	new_node = (t_pnode *)malloc(sizeof(t_pnode));
	if (!new_node)
	{
		perror("Failed to allocate memory for node.\n");
		return ;
	}
	new_node->toktype = -1;
	new_node->next = NULL;
	new_node->prev = NULL;
	new_node->strpart = NULL;
	if (!node || !(*node))
		return ;
	new_node->prev = *node;
	(*node)->next = new_node;
}

void	nl_clear(t_pnode **node)
{
	t_pnode	*current;

	if (!node)
		return ;
	while (*node)
	{
		current = *node;
		*node = (*node)->next;
		free(current->strpart);
		free(current);
	}
	*node = NULL;
}

void	back_to_firstnode(t_pnode **node)
{
	if (!node || !(*node))
		return ;
	while ((*node)->prev != NULL)
		*node = (*node)->prev;
}

void	store_str(t_pnode *node, char *str, int numchar, int *i)
{
	int	j;

	j = 0;
	if (!node || !str || numchar < 1)
		return ;
	node->strpart = (char *)malloc((numchar + 1) * sizeof(char));
	if (!node->strpart)
	{
		perror("Failed to allocate memory for strpart.\n");
		return ;
	}
	while (j < numchar)
	{
		node->strpart[j] = str[*i];
		j++;
		(*i)++;
	}
	node->strpart[j] = '\0';
}

int		search_op(t_pnode *node, char *line, int *i)
{
	if (line[*i] == '|')
		node->toktype = PIPET;
	else if (line[*i] == '\'')
		node->toktype = SINGLEQUOTE;
	else if (line[*i] == '\"')
		node->toktype = DOUBLEQUOTE;
	else if (line[*i] == '$')
		node->toktype = EXPAND;
	else if (line[*i] == '<' && line[*i + 1] != '<')
		node->toktype = REDINPUT;
	else if (line[*i] == '<' && line[*i + 1] == '<')
		node->toktype = REDDELIMETER;
	else if (line[*i] == '>' && line[*i + 1] != '>')
		node->toktype = REDOUTPUT;
	else if (line[*i] == '>' && line[*i + 1] == '>')
		node->toktype = APPENDREDOUTPUT;
	else
		return (-1);
	if (node->toktype == REDDELIMETER || node->toktype == APPENDREDOUTPUT)
		store_str(node, line, 2, i);
	else
		store_str(node, line, 1, i);
	return (node->toktype);
}
