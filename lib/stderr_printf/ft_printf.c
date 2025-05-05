/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfarkas <tfarkas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 10:50:36 by tfarkas           #+#    #+#             */
/*   Updated: 2025/05/05 10:43:35 by tfarkas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static int	ft_write_format(const char *format, va_list args);

void	redir_stderr(int *saved_stdout)
{
	*saved_stdout = dup(1);
	if (*saved_stdout == -1)
	{
		perror("Error saved stdin in stderr_printf");
		exit(EXIT_FAILURE);
	}
	if (dup2(2, 1) == -1)
	{
		perror("Error redirect stderr filedescripor in stderr_printf");
		exit(EXIT_FAILURE);
	}
}

int	stderr_printf(const char *format, ...)
{
	int	saved_stdout;

	va_list	ap;
	int		result_int;

	va_start(ap, format);
	result_int = 0;
	if (*format == '\0')
		return (va_end(ap), 0);
	result_int = ft_write_format(format, ap);
	if (result_int != -1)
		return (va_end(ap), result_int);
	return (va_end(ap), -1);
}

static int	ft_write_format(const char *format, va_list args)
{
	int	result_int;
	int	i;
	int	check;

	result_int = 0;
	i = 0;
	while (format[i] != '\0')
	{
		if (format[i] != '%')
		{
			if (ft_putchar_w(1, format[i]) == -1)
				return (-1);
			result_int++;
		}
		else
		{
			check = ft_choose_formatter_case(format[i + 1], args);
			if (check == -1)
				return (-1);
			result_int = result_int + check;
			i++;
		}
		i++;
	}
	return (result_int);
}
