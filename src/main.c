/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/26 16:17:36 by samusanc          #+#    #+#             */
/*   Updated: 2023/07/04 17:52:33 by samusanc         ###   ########.fr       */
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
	ft_fill_img(img, 0xFF000000);
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

int	ft_mix_color(int color1, int color2, double tr)
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
	mlx_put_image_to_window(fdf->mlx, fdf->win, fdf->map_display.img, 0, 0);
	mlx_put_image_to_window(fdf->mlx, fdf->win, fdf->ui.img, 4, 5);
	return ;
}

t_img	*ft_open_img(t_fdf *fdf, t_img *img, char *path)
{
	img->img = mlx_xpm_file_to_image(&fdf->mlx, path, &img->width, &img->height);
	if (!img->img)
		ft_error_log("IMG_OPEN_ME_CAGO_EN_TUS_MUERTOS");
	return (img);
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

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int	ft_abs(int x)
{
	if (x < 0)
		return (-x);
	return (x);
}

void	ft_init_bresen(t_point *delta, t_point \
*sign, t_point f, t_point s)
{
	delta->x = ft_abs(s.x - f.x);
	delta->y = ft_abs(s.y - f.y);
	if (f.x < s.x)
		sign->x = 1;
	else
		sign->x = -1;
	if (f.y < s.y)
		sign->y = 1;
	else
		sign->y = -1;
}

int	ft_make_translucid(int color1)
{
	int	t;
	int	r;
	int	g;
	int	b;

	t = 255;
	r = ft_get_color(color1, "R");
	g = ft_get_color(color1, "G");
	b = ft_get_color(color1, "B");
	return (t << 24 | r << 16| g << 8 | b);
}

double	ft_distance_2_points(t_point a, t_point b)
{
	double	x;
	double	y;

	x = pow((b.x - a.x), 2);
	y = pow((b.y - a.y), 2);
	return (sqrt(x + y));
}

int	ft_color_degradade(t_point init, t_point end, t_point current)
{
	double	total_distance;
	double	actual_distance;
	double	result;
	
	total_distance = ft_distance_2_points(init, end);
	actual_distance = ft_distance_2_points(init, current);
	result = actual_distance / total_distance;
	return (ft_mix_color(init.color, end.color, result));
}

void	ft_draw_line(t_point f, t_point s, t_fdf *fdf)
{
	t_point	delta;
	t_point	sign;
	t_point	cur;
	int		error;

	ft_init_bresen(&delta, &sign, f, s);
	error = delta.x - delta.y;
	cur = f;
	while (cur.x != s.x || cur.y != s.y)
	{
		ft_put_pixel(&fdf->map_display, cur.x, cur.y, ft_color_degradade(f, s, cur));
		if ((error * 2) > -delta.y)
		{
			error -= delta.y;
			cur.x += sign.x;
		}
		if ((error * 2) < delta.x)
		{
			error += delta.x;
			cur.y += sign.y;
		}
	}
}

void	rotate_x(int *y, int *z, double alpha)
{
	int tmp_y;
	int tmp_z;
	
	tmp_y = *y;
	tmp_z = *z;
	*y = tmp_y * cos(alpha) + tmp_z * sin(alpha);
	*z = tmp_y * sin(alpha) + tmp_z * cos(alpha);
}

void	iso(int *x, int *y, int z)
{
	int	tmp_x;
	int	tmp_y;

	tmp_x = *x;
	tmp_y = *y;
	*x = (tmp_x - tmp_y) * cos(0.523599);
	*x = -z + (tmp_x + tmp_y) * sin(0.523599);
}

t_point	au(t_point point, t_fdf *fdf)
{
	point.x *= fdf->camera->zoom;
	point.y *= fdf->camera->zoom;
	point.x -= (fdf->camera->zoom * fdf->map->width) / 2;
	point.y -= (fdf->camera->zoom * fdf->map->height) / 2;
	//rotate_x(&point.y, &point.z, fdf->camera->alpha);
	//iso(&point.x, &point.y, point.z);
	point.x += 1000;
	point.y += 500;
	return (point);
}

t_point	ft_point(int x, int y, t_map *map)
{
	t_point	point;

	point.x = x;
	point.y = y;
	point.color = 0x00FF0000;
	point.z = map->coords_arr[x * y * map->width];
	if (map->colors_arr[y * map->width + x] != -1)
			point.color = map->colors_arr[x * y * map->width];
	return (point);
}

void	ft_draw(t_map *map, t_fdf *fdf)
{
	int	x;
	int	y;

	x = 0;
	y = 0;
	while (y < map->height)
	{
		x = 0;
		while (x < map->width)
		{
			if (x != (map->width - 1))
				ft_draw_line(au(ft_point(x, y, map), fdf), au(ft_point(x + 1, y, map), fdf), fdf);
			if (y != (map->height - 1))
				ft_draw_line(au(ft_point(x, y, map), fdf), au(ft_point(x, y + 1, map), fdf), fdf);
			x++;
		}
		y++;
	}
	ft_put_display(fdf);
}


void	ft_print_map(t_map *map)
{
	int i;
	int j;

	i = map->width * map->height;
	j = 0;
	while (j++ < i)
	{
		ft_printf("%d, %d	", map->coords_arr[j], map->colors_arr[j]);
		if (j % 19 == 0)
			ft_printf("\n");
	}
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

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
		fdf = fdf_init(map, argv[1]);
		ft_stack_to_arrays(&coords_stack, map);
		fdf->camera = ft_init_camera(fdf);
		ft_draw(fdf->map, fdf);
		ft_print_map(map);
		printf("Alles gut\n");
		mlx_loop(fdf->mlx);
	}
	ft_printf("usage: ./fdf 'map.fdf'\n");
	exit(0);
}
