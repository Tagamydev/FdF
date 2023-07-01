/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_read_map.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 10:32:49 by samusanc          #+#    #+#             */
/*   Updated: 2023/07/01 10:48:30 by samusanc         ###   ########.fr       */
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

t_coord_val	*ft_make_coord(char *number)
{
	t_coord_val	*result;
	char		**parts;

	result = malloc(sizeof(t_coord_val));
	if (!result)
		ft_error_log("READ_MAP");
	parts = ft_split(number, ',');
	if (!parts)
		ft_error_log("READ_MAP");
	if (!ft_is_number_or_color(parts))
		ft_error_log("INVALID_MAP_MAKE_COORD");
	result->z = ft_atoi_base(parts[0], 10);
	if (parts[1])
		result->color = ft_atoi_base(parts[1], 16);
	else
		result->color = -1;
	result->next = NULL;
	ft_free_split(parts);
	return (result);
}

void	ft_parse_line(t_coord_val **coords_stack, \
char **coords_line, t_map *map)
{
	int	i;
	int	width;

	width = 0;
	i = 0;
	while (coords_line[i])
		width += ft_push(coords_stack, ft_make_coord(coords_line[i++]));
	if (!(map->height))
		map->width = width;
	else if (map->width != width)
		ft_error_log("INVALID_MAP_PARSE_LINE");
}

int	ft_read_map(int fd, t_coord_val **coords_stack, t_map *map)
{
	char	*line;
	char	**coords_line;

	line = get_next_line(fd);
	if (!line)
		ft_error_log("GET_NETX_LINE");
	while (line)
	{
		coords_line = ft_split(line, ' ');
		if (!coords_line)
			ft_error_log("SPLIT");
		ft_parse_line(coords_stack, coords_line, map);
		ft_free_split(coords_line);
		ft_free_string(&line);
		map->height += 1;
		line = get_next_line(fd);
	}
	if (!(*coords_stack))
		ft_error_log("ERR_MAP");
	return (0);
}
