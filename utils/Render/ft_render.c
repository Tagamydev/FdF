/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_render.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/10 15:37:05 by samusanc          #+#    #+#             */
/*   Updated: 2023/07/10 16:49:07 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <fdf.h>

static void	ft_proyect_minimap_util(t_fdfc *fdf, int *max, double *i)
{
	if (fdf->map->width > fdf->map->height)
		*max = fdf->map->width;
	else
		*max = fdf->map->height;
	if (*max > 200)
	{
		*i = 1;
		while (*max)
		{
			*i = *i / 1.2;
			*max = *max / 10;
		}
	}
	else
		*i = 200 / *max;
}

t_point	ft_proyect_minimap(t_point *point, t_fdfc *fdf)
{
	t_point	point_clone;
	int		max;
	double	i;

	max = 0;
	i = 0;
	ft_proyect_minimap_util(fdf, &max, &i);
	point_clone.x = point->x * i;
	point_clone.y = point->y * i;
	point_clone.z = point->z * i;
	point_clone.color = point->color;
	free(point);
	fdf = NULL;
	return (point_clone);
}

t_point	ft_proyect(t_point *point, t_fdfc *fdf)
{
	t_point	point_clone;

	ft_make_maths(point, fdf);
	point_clone.x = point->x;
	point_clone.y = point->y;
	point_clone.z = point->z;
	point_clone.color = point->color;
	free(point);
	return (point_clone);
}

void	ft_clone(t_point *point, t_points line, int x, int y)
{
	point->x = x;
	point->y = y;
	point->color = ft_color_degradade(line.start, line.end, *point);
}

void	ft_draw_pixel(t_point point, t_img *img)
{
	ft_put_pixel(img, point.x, point.y, point.color);
}

static void	bb_init(t_bb *bb, t_point f, t_point s)
{
	bb->x0 = f.x;
	bb->y0 = f.y;
	bb->x1 = s.x;
	bb->y1 = s.y;
	bb->dx = ft_abs(bb->x1 - bb->x0);
	bb->dy = ft_abs(bb->y1 - bb->y0);
	if (bb->x0 < bb->x1)
		bb->sx = 1;
	else
		bb->sx = -1;
	if (bb->y0 < bb->y1)
		bb->sy = 1;
	else
		bb->sy = -1;
	bb->err = bb->dx - bb->dy;
}

void	ft_put_line(t_point f, t_point s, t_img *map_display)
{
	t_bb		bb;
	t_point		tmp;
	t_points	line;
	int			e2;

	bb_init(&bb, f, s);
	line.start = f;
	line.end = s;
	while (1) {
		ft_clone(&tmp, line, bb.x0, bb.y0);
		ft_draw_pixel(tmp, map_display);
		if (bb.x0 == bb.x1 && bb.y0 == bb.y1)
			break;
		e2 = 2 * bb.err;
		if (e2 > -bb.dy) {
			bb.err -= bb.dy;
			bb.x0 += bb.sx;
		}
		if (e2 < bb.dx) {
			bb.err += bb.dx;
			bb.y0 += bb.sy;
		}
	}
}

void	ft_ft_draw(t_fdfc *fdf)
{
	int	x;
	int	y;

	x = 0;
	y = 0;

	ft_fill_img(&fdf->map_display, 0xFF000000);
	while (y < fdf->map->height)
	{
		x = 0;
		while (x < fdf->map->width)
		{
			if (x != (fdf->map->width - 1))
				ft_put_line(ft_proyect(ft_get_point(fdf->map->map, x, y), fdf),\
				ft_proyect(ft_get_point(fdf->map->map, x + 1, y), fdf), &fdf->map_display);
			if (y != (fdf->map->height - 1))
				ft_put_line(ft_proyect(ft_get_point(fdf->map->map, x, y), fdf), \
				ft_proyect(ft_get_point(fdf->map->map, x, y + 1), fdf), &fdf->map_display);
			x++;
		}
		y++;
	}
	x = 0;
	y = 0;
	while (y < fdf->map->height)
	{
		x = 0;
		while (x < fdf->map->width)
		{
			if (x != (fdf->map->width - 1))
				ft_put_line(ft_proyect_minimap(ft_get_point(fdf->map->map, x, y), fdf),\
				ft_proyect_minimap(ft_get_point(fdf->map->map, x + 1, y), fdf), &fdf->map_display);
			if (y != (fdf->map->height - 1))
				ft_put_line(ft_proyect_minimap(ft_get_point(fdf->map->map, x, y), fdf), \
				ft_proyect_minimap(ft_get_point(fdf->map->map, x, y + 1), fdf), &fdf->map_display);
			x++;
		}
		y++;
	}

	mlx_put_image_to_window(fdf->mlx, fdf->win, fdf->background.img, 0, 0);
	mlx_put_image_to_window(fdf->mlx, fdf->win, fdf->map_display.img, 0, 0);
}
