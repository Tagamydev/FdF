/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/26 16:17:36 by samusanc          #+#    #+#             */
/*   Updated: 2023/06/27 14:30:14 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <fdf.h>

void	leaks()
{
	system("leaks aoeu");
}

void	ft_put_pixel(t_img *img, int x, int y, int color)
{
	char	*dst;

	if (x >= 0 && y >= 0 && x < img->width && y < img->height)
		dst = img->data_addr + ((y * img->line_size) + \
		((x * (img->bits_per_pixel / 8))));
	*(unsigned int*)dst = color;
}

t_img	*ft_init_img(t_img *img, void *mlx, int width, int height)
{	
	img->img = mlx_new_image(mlx, width, height);
	img->data_addr = mlx_get_data_addr(img->img, &(img->bits_per_pixel), \
	&(img->line_size), &(img->endian));
	img->transparency = 1;
	img->width = width;
	img->height = height;
	return (img);
}

void	ft_put_square(t_img *img, int x, int y, int perimeter, int color)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (i != perimeter)
	{
		j = 0;
		while (j != perimeter)
		{
			ft_put_pixel(img, i + x, j + y, color);
			++j;
		}
		++i;
	}
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

void	ft_make_display(t_img *ui, t_img *bg, t_img *dp)
{
	ui = NULL;
	bg = NULL;
	dp = NULL;
	return ;
}

t_img	*ft_open_img(t_fdf *fdf, t_img *img, char *path)
{
	img->img = mlx_xpm_file_to_image(&fdf->mlx, path, &img->width, &img->height);
	if (!img->img)
	{
		write(2, "IMG_ERROR\n", 10);
		exit(0);
	}
	return (img);
}

int	main()
{
	t_fdf	*fdf;
	t_img	*bg;
	t_img	*ui;
	int		y;

	y = 0;
	fdf = NULL;
	atexit(leaks);
	fdf = malloc(sizeof(t_fdf));
	fdf->mlx = mlx_init();
	fdf->win = mlx_new_window(fdf->mlx, 1920, 1080, "FDF");
	bg = ft_init_img(&fdf->background, fdf->mlx, 1920, 1080);
	ui = ft_open_img(fdf, &fdf->ui, "./src/img/test.xpm");
	ft_fill_img(bg, 0x00FFFFFF);
	//mlx_put_image_to_window(fdf->mlx, fdf->win, fdf->display->img, 0, 0);
	mlx_put_image_to_window(fdf->mlx, fdf->win, fdf->background.img, 0, 0);
	mlx_put_image_to_window(fdf->mlx, fdf->win, fdf->ui.img, 50, 0);
	mlx_put_image_to_window(fdf->mlx, fdf->win, fdf->ui.img, 0, 0);
	mlx_loop(fdf->mlx);
}
