/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_read_map.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 10:32:49 by samusanc          #+#    #+#             */
/*   Updated: 2023/07/09 22:28:05 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <fdf.h>

int	ft_atoi_base(char *number, int base)
{
	int	result;
	int	sing;

	result = 0;
	sing = 1;
	if (base > 10)
		return (ft_atoi_base_hex(number, base));
	while (*number == ' ' || *number == '\t')
		number++;
	if (*number == '-')
		sing = -sing;
	if (*number == '-' || *number == '+')
		number++;
	while (*number >= '0' && *number <= '9')
		result = result * base + ft_is_digit_base(*(number++), 10, BASE_UP);
	if (*number && *number != '\n')
		ft_error_log("INVALID_MAP");
	return (sing * result);
}

int	ft_is_number_or_color(char **numb)
{
	if (!*numb)
		return (0);
	ft_atoi_base(*numb, 10);
	if (*(numb + 1))
		ft_atoi_base(*(numb + 1), 16);
	return (1);
}
