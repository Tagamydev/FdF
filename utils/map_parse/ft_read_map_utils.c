/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_read_map_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 10:40:17 by samusanc          #+#    #+#             */
/*   Updated: 2023/07/09 22:28:23 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <fdf.h>

int	ft_is_digit_base(char c, int base, char *digits)
{
	int	i;

	i = 0;
	while (i < base)
	{
		if (digits[i] == ft_toupper(c))
			return (i);
		i++;
	}
	return (-1);
}

int	ft_atoi_base_hex(char *number, int base)
{
	int	result;

	result = 0;
	if (*number == '0' && *(number + 1) == 'x')
		number += 2;
	else
		ft_error_log("COLOR_FORMAT_ERROR_0x");
	while (ft_is_digit_base(*number, base, BASE_UP) != -1)
		result = result * base + ft_is_digit_base(*(number++), base, BASE_UP);
	if (*number && *number != '\n')
		ft_error_log("COLOR_FORMAT_ERROR_NULL");
	return (result);
}
