/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_write_tools.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfarkas <tfarkas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 12:14:40 by tfarkas           #+#    #+#             */
/*   Updated: 2024/11/07 10:40:27 by tfarkas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_putchar_w(int fd, char ch)
{
	int	check;

	check = write(fd, &ch, 1);
	if (check == -1)
		return (-1);
	return (1);
}

int	ft_putstr_w(int fd, char *str)
{
	int	strleng;

	strleng = 0;
	if (*str == '\0')
		return (-1);
	while (str[strleng] != '\0')
	{
		if (ft_putchar_w(fd, str[strleng]) == -1)
			return (-1);
		strleng++;
	}
	return (strleng);
}
