/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/26 16:17:36 by samusanc          #+#    #+#             */
/*   Updated: 2023/07/01 10:18:48 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <fdf.h>

void	leaks()
{
	system("leaks aoeu");
}

void	ft_error_log(char *str)
{
	write(2, "ERROR_", 6);
	write(2, str, ft_strlen(str));
	write(2, "\n", 1);
	exit(-1);
}

void	ft_put_pixel(t_img *img, int x, int y, int color)
{
	char	*dst;

	if (x >= 0 && y >= 0 && x < img->width && y < img->height)
	{
		dst = img->data_addr + ((y * img->line_size) + \
		((x * (img->bits_per_pixel / 8))));
		*(unsigned int*)dst = color;
	}
}

void	ft_print_coord(t_coord_val *list)
{
	t_coord_val *tmp;

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
	ft_printf("hola, estoy liberando, pero realmente no porque soy imbeeeeeeeeeeeecil\n");
	t_coord_val *tmp1;
	t_coord_val *tmp2;

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

t_img	*ft_init_img(t_fdf *fdf,t_img *img, int width, int height)
{	
	void	*mlx;

	mlx = fdf->mlx;
	img->img = mlx_new_image(mlx, width, height);
	img->data_addr = mlx_get_data_addr(img->img, &(img->bits_per_pixel), \
	&(img->line_size), &(img->endian));
	img->transparency = 1;
	img->width = width;
	img->height = height;
	return (img);
}

void 	ft_fill_img(t_img *img, int color)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (i != img->height)
	{
		j = 0;
		while (j != img->width)
		{
			ft_put_pixel(img, j, i, color);
			++j;
		}
		i++;
	}
}

int	ft_get_color(int color, char *str)
{
	if (*str == 'T')
		return ((color >> 24) & 0xFF);
	if (*str == 'R')
		return ((color >> 16) & 0xFF);
	if (*str == 'G')
		return ((color >> 8) & 0xFF);
	if (*str == 'B')
		return ((color) & 0xFF);
	return (0);
}

int	ft_lineal_mix(int c1, int c2, double mix)
{
	return (c1 * (1 - mix) + c2 * mix);
}

int	ft_make_transparency(int color1, int color2, double tr)
{
	int	t;
	int	r;
	int	g;
	int	b;

	t = 0;
	t = ft_lineal_mix(ft_get_color(color1, "T"), \
	ft_get_color(color2, "T"), tr);
	r = ft_lineal_mix(ft_get_color(color1, "R"), \
	ft_get_color(color2, "R"), tr);
	g = ft_lineal_mix(ft_get_color(color1, "G"), \
	ft_get_color(color2, "G"), tr);
	b = ft_lineal_mix(ft_get_color(color1, "B"), \
	ft_get_color(color2, "B"), tr);
	return (t << 24 | r << 16| g << 8 | b);
}

void	ft_put_display(t_fdf *fdf)
{
	mlx_put_image_to_window(fdf->mlx, fdf->win, fdf->background.img, 0, 0);
	mlx_put_image_to_window(fdf->mlx, fdf->win, fdf->ui.img, 4, 5);
	return ;
}

t_img	*ft_open_img(t_fdf *fdf, t_img *img, char *path)
{
	img->img = mlx_xpm_file_to_image(&fdf->mlx, path, &img->width, &img->height);
	if (!img->img)
		ft_error_log("IMG_OPEN");
	return (img);
}


int	ft_push(t_coord_val **coords_stack, t_coord_val *new)
{
	if (coords_stack)
	{
		if (new)
			new->next= *coords_stack;
		*coords_stack = new;
	}
	return (1);
}

int	ft_free_string(char **split)
{
	if (!*split)
		return (0);
	if (*split)
		free(*split);
	*split = NULL;
	return (0);
}

int	ft_free_split(char **split)
{
	int	i;

	i = 0;
	if (split && *split)
	{
		while (split[i])
			ft_free_string(&split[i++]);
		free(split);
		split = NULL;
	}
	return (0);
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

void	ft_parse_line(t_coord_val **coords_stack, char **coords_line, t_map *map)
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

int	ft_read_map(int	fd, t_coord_val **coords_stack,t_map *map)
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

t_map	*ft_init_map()
{
	t_map	*map;

	map = ft_calloc(sizeof(t_map), 1);
	if (!map)
		ft_error_log("MAP_INIT");
	map->width = 0;
	map->height = 0;
	map->coords_arr = NULL;
	map->colors_arr = NULL;
	map->z_min = INT_MAX;
	map->z_max = INT_MIN;
	map->z_range = 0;
	return (map);
}

//				hay que hacer una funcion para poner el titulo del mapa en la ventana
//				y comprobar que tenga el formato .fdf

int	main(int argc, char **argv)
{
	t_fdf	*fdf;
	t_coord_val	*coords_stack;
	t_map	*map;
	int		fd;

	fdf = NULL;
	coords_stack = NULL;
	atexit(leaks);
	if (argc == 2)
	{
		fd = open(argv[1], O_RDONLY);
		if (fd == -1)
			ft_error_log("FD_OPEN");
		map = ft_init_map();
		ft_read_map(fd, &coords_stack, map);
		ft_print_coord(coords_stack);
		ft_free_coord(&coords_stack);
		/*
		ft_put_display(fdf);
		mlx_loop(fdf->mlx);
		*/
	}
	ft_printf("usage: ./fdf 'map.fdf'\n");
	exit(0);
}
