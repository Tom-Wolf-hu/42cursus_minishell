/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_realloc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 15:20:15 by alex              #+#    #+#             */
/*   Updated: 2025/04/17 10:51:36 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_realloc(void *oldptr, size_t oldsize, size_t newsize)
{
	void			*newptr;
	unsigned char	*bytenew;
	unsigned char	*byteold;
	size_t			i;

	i = 0;
	if (newsize == 0)
	{
		free(oldptr);
		return (NULL);
	}
	newptr = (void *)malloc(newsize);
	if (!newptr)
		return (NULL);
	if (!oldptr)
		return (newptr);
	byteold = (unsigned char *)oldptr;
	bytenew = (unsigned char *)newptr;
	while (i < newsize && i < oldsize)
	{
		bytenew[i] = byteold[i];
		i++;
	}
	free(oldptr);
	return (newptr);
}
