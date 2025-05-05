/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_create_str_in_diff_base.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfarkas <tfarkas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 11:18:49 by tfarkas           #+#    #+#             */
/*   Updated: 2024/11/07 14:16:04 by tfarkas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static int	intlength(unsigned long long unum, char *chosenbase);
static char	*choose_base(char c);
static char	*create_string(unsigned long long unum, int intlen,
				char *chosenbase);
static int	ft_strlen(const char *s);

char	*ft_create_str_in_diff_base(unsigned long long unum, char c)
{
	int		intlen;
	char	*chosenbase;
	char	*created_str;

	chosenbase = choose_base(c);
	intlen = intlength(unum, chosenbase);
	if (chosenbase == (char *)0)
		return (NULL);
	created_str = create_string(unum, intlen, chosenbase);
	if (created_str == NULL)
		return (NULL);
	return (created_str);
}

static int	intlength(unsigned long long unum, char *chosenbase)
{
	int	count;
	int	basecount;

	count = 0;
	basecount = ft_strlen(chosenbase);
	if (unum == 0)
		count++;
	while (unum != 0)
	{
		unum = unum / basecount;
		count++;
	}
	return (count);
}

static char	*choose_base(char c)
{
	char	*decimalbase;
	char	*smallhexadecimalbase;
	char	*largehexadecimalbase;
	char	*chosen_base;

	decimalbase = "0123456789";
	smallhexadecimalbase = "0123456789abcdef";
	largehexadecimalbase = "0123456789ABCDEF";
	if (c == 'u')
		chosen_base = decimalbase;
	else if (c == 'x')
		chosen_base = smallhexadecimalbase;
	else if (c == 'X')
		chosen_base = largehexadecimalbase;
	else
		chosen_base = (char *)0;
	return (chosen_base);
}

static char	*create_string(unsigned long long unum, int intlen,
		char *chosenbase)
{
	char	*createstr;
	int		basecount;
	int		i;

	basecount = ft_strlen(chosenbase);
	i = intlen - 1;
	if (unum == 0)
	{
		createstr = (char *)malloc(2 * sizeof(char));
		if (createstr == NULL)
			return (NULL);
		createstr[0] = '0';
		createstr[1] = '\0';
		return (createstr);
	}
	createstr = (char *)malloc((intlen + 1) * sizeof(char));
	if (createstr == NULL)
		return (NULL);
	while (unum != 0)
	{
		createstr[i--] = chosenbase[unum % basecount];
		unum = unum / basecount;
	}
	createstr[intlen] = '\0';
	return (createstr);
}

static int	ft_strlen(const char *s)
{
	int	n;

	n = 0;
	while (*s != '\0')
	{
		n++;
		s++;
	}
	return (n);
}
