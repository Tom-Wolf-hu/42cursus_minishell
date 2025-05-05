/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_choose_formatter_case.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfarkas <tfarkas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 12:37:17 by tfarkas           #+#    #+#             */
/*   Updated: 2024/11/07 14:29:29 by tfarkas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static int	ft_str_check(int fd, va_list args);
static int	ft_create_ptr_address(void *ptr);
static int	ft_write_number(int fd, char formatter, va_list args);
static int	ft_ull_conv(int fd, va_list args);

int	ft_choose_formatter_case(char formatter, va_list args)
{
	int	count;

	if (formatter == 'c')
		count = ft_putchar_w(1, va_arg(args, int));
	else if (formatter == 's')
		count = ft_str_check(1, args);
	else if (formatter == 'p')
		count = ft_create_ptr_address(va_arg(args, void *));
	else if (formatter == 'd' || formatter == 'i' || formatter == 'u'
		|| formatter == 'x' || formatter == 'X')
		count = ft_write_number(1, formatter, args);
	else if (formatter == '%')
		count = ft_putchar_w(1, '%');
	else
		count = -1;
	return (count);
}

static int	ft_str_check(int fd, va_list args)
{
	char	*str;
	int		count;

	str = va_arg(args, char *);
	if (str == NULL)
		count = ft_putstr_w(fd, "(null)");
	else if (*str == '\0')
		return (0);
	else
		count = ft_putstr_w(fd, str);
	return (count);
}

static int	ft_create_ptr_address(void *ptr)
{
	int					count1;
	int					count2;
	char				*word;
	unsigned long long	ulnum;

	count1 = ft_putstr_w(1, "0x");
	if (count1 == -1)
		return (-1);
	ulnum = (unsigned long long)ptr;
	word = ft_create_str_in_diff_base(ulnum, 'x');
	if (word == NULL)
		return (-1);
	count2 = ft_putstr_w(1, word);
	free(word);
	if (count2 == -1)
		return (-1);
	return (count1 + count2);
}

static int	ft_write_number(int fd, char formatter, va_list args)
{
	int		count;
	char	*word;

	word = (char *)0;
	if (formatter == 'd' || formatter == 'i')
	{
		count = ft_ull_conv(fd, args);
		if (count == -1)
			return (-1);
		return (count);
	}
	else if (formatter == 'u' || formatter == 'x' || formatter == 'X')
		word = ft_create_str_in_diff_base(va_arg(args, unsigned int),
				formatter);
	if (word == NULL)
		return (-1);
	count = ft_putstr_w(fd, word);
	free(word);
	return (count);
}

static int	ft_ull_conv(int fd, va_list args)
{
	int					i_num;
	int					check;
	int					count;
	char				*word;
	unsigned long long	ul_num;

	i_num = va_arg(args, int);
	check = 0;
	if (i_num < 0)
	{
		ul_num = ULLONG_MAX - ((unsigned long long)i_num - 1);
		word = ft_create_str_in_diff_base(ul_num, 'u');
		check = ft_putchar_w(fd, '-');
		if (check == -1)
			return (free(word), -1);
	}
	else
		word = ft_create_str_in_diff_base(i_num, 'u');
	if (word == NULL)
		return (-1);
	count = ft_putstr_w(fd, word);
	if (count == -1)
		return (free(word), -1);
	free(word);
	return (check + count);
}
