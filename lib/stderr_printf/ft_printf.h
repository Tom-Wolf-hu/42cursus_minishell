/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfarkas <tfarkas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 11:08:10 by tfarkas           #+#    #+#             */
/*   Updated: 2025/05/05 10:43:48 by tfarkas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <stdarg.h>
# include <stdlib.h>
# include <unistd.h>
# include <limits.h>

int		stderr_printf(char const *formatter, ...);
int		ft_putchar_w(int fd, char ch);
int		ft_putstr_w(int fd, char *str);
int		ft_choose_formatter_case(char formatter, va_list args);
char	*ft_create_str_in_diff_base(unsigned long long unum, char c);

#endif