/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_read_map_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 10:40:17 by samusanc          #+#    #+#             */
/*   Updated: 2023/07/01 10:50:05 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <fdf.h>

void	ft_print_coord(t_coord_val *list)
{
	t_coord_val	*tmp;

	tmp = list;
	ft_printf("COORDS:\n");
	while (tmp)
	{
		ft_printf("z:%d, color:%d\n", tmp->z, tmp->color);
		tmp = tmp->next;
	}
}

void	ft_free_coord(t_coord_val **list)
{
	t_coord_val	*tmp1;
	t_coord_val	*tmp2;

	ft_printf("hola, estoy liberando\n");
	tmp1 = NULL;
	tmp2 = NULL;
	if (!list)
		return ;
	if (!*list)
		return ;
	tmp1 = *list;
	if (tmp1->next)
		tmp2 = tmp1->next;
	while (tmp2)
	{
		free(tmp1);
		tmp1 = NULL;
		tmp1 = tmp2;
		tmp2 = tmp1->next;
	}
	free(tmp1);
}

int	ft_push(t_coord_val **coords_stack, t_coord_val *new)
{
	if (coords_stack)
	{
		if (new)
			new->next = *coords_stack;
		*coords_stack = new;
	}
	return (1);
}

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
