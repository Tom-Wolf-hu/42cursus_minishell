/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_realloc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 15:20:15 by alex              #+#    #+#             */
/*   Updated: 2025/04/15 15:21:26 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_realloc(void *ptr, size_t size)
{
	void	*new_ptr;
	size_t	i;
	char	*src;
	char	*dst;

	if (!ptr)
		return (malloc(size));
	if (size == 0)
	{
		free(ptr);
		return (NULL);
	}
	new_ptr = malloc(size);
	if (!new_ptr)
		return (NULL);
	src = (char *)ptr;
	dst = (char *)new_ptr;
	i = 0;
	while (i < size)
	{
		dst[i] = src[i];
		i++;
	}
	free(ptr);
	return (new_ptr);
}
