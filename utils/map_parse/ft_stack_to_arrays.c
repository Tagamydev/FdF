/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_stack_to_arrays.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 15:10:01 by samusanc          #+#    #+#             */
/*   Updated: 2023/07/01 15:12:38 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <fdf.h>

static t_coord_val	*ft_push_top(t_coord_val **coords_stack)
{
	t_coord_val	*tmp;

	tmp = NULL;
	if (coords_stack && *coords_stack)
	{
		tmp = *coords_stack;
		*coords_stack = tmp->next;
	}
	return (tmp);
}

void	ft_stack_to_arrays(t_coord_val **coords_stack, t_map *map)
{
	t_coord_val	*coord;
	ssize_t		i;
	size_t		arr_size;

	arr_size = map->width * map->height * sizeof(int);
	map->coords_arr = calloc((arr_size), 1);
	if (!map->coords_arr)
		ft_error_log("STACK_2_ARRAY_ALLOC");
	map->colors_arr = calloc((arr_size), 1);
	if (!map->colors_arr)
		ft_error_log("STACK_2_COLORS_ALLOC");
	i = map->width * map->height -1;
	coord = ft_push_top(coords_stack);
	while (coord && i >= 0)
	{
		map->coords_arr[i] = coord->z;
		map->colors_arr[i--] = coord->color;
		if (coord->z > map->z_max)
			map->z_max = coord->z;
		if (coord->z < map->z_min)
			map->z_min = coord->z;
		free(coord);
		coord = ft_push_top(coords_stack);
	}
	map->z_range = map->z_max - map->z_min;
}
