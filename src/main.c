/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/26 16:17:36 by samusanc          #+#    #+#             */
/*   Updated: 2023/07/01 21:55:24 by samusanc         ###   ########.fr       */
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

void	ft_bresen_init(t_bresen *z, t_point a, t_point b)
{
	z->dx = ft_abs(b.x - a.x);
	z->dy = -ft_abs(b.y - a.y);
	if (a.x < b.x)
		z->sx = 1;
	else
		z->sx = -1;
	if (a.y < b.y)
		z->sy = 1;
	else
		z->sy = 1;
	z->error = z->dx + z->dy;
}

void	ft_draw_line(t_point a, t_point b, t_fdf *fdf)
{
	t_bresen *z;

	z = malloc(sizeof(t_bresen));
	if (!z)
		ft_error_log("KOMENASAE_ONICHAN");
	ft_bresen_init(z, a, b);
	while (1)
	{
		ft_put_pixel(&fdf->map_display, a.x, a.y, 0x00FF0000);
		if (a.x == b.x && a.y == b.y)
			break ;
		z->e2 = z->error * 2;
		if (z->e2 >= z->dy)
		{
			if (a.x == b.x)
				break ;
			z->error += z->dy;
			a.x += z->sx;
		}
		if (z->e2 <= z->dx)
		{
			if (a.y == b.y)
				break ;
			z->error += z->dx;
			a.y += z->sy;
		}
	}
}

void	ft_draw(t_map *map, t_fdf *fdf)
{
	/*
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
				ft_put_line();
			if (y != (map->height - 1))
				ft_put_line();
		}
	}
	*/
	t_point	a;
	t_point	b;

	a.x = 200;
	a.y = 200;
	b.x = 600;
	b.y = 300;
	ft_draw_line(a, b, fdf);
	ft_put_display(fdf);
	map = NULL;
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
		printf("Alles gut\n");
		mlx_loop(fdf->mlx);
	}
	ft_printf("usage: ./fdf 'map.fdf'\n");
	exit(0);
}
