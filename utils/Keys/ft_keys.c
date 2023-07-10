/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_keys.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/10 15:41:54 by samusanc          #+#    #+#             */
/*   Updated: 2023/07/10 15:59:53 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <fdf.h>

void	ft_close(void *param)
{
	(void)param;
	exit(0);
}

void	ft_change_proyection(t_fdfc *fdf)
{
	if (fdf->angle.projection == CONIC)
		fdf->angle.projection = ISO;
	else
		fdf->angle.projection++;
	if (fdf->angle.projection == PARALLEL)
	{
		fdf->camera->alpha = 0;
		fdf->camera->beta = 0;
		fdf->camera->gamma = 1.55;
	}
	ft_ft_draw(fdf);
}

int	ft_rotate(int key, t_fdfc *fdf)
{
	if (key == 91)	
		fdf->camera->alpha -= 0.1;
	else if (key == 84)	
		fdf->camera->alpha += 0.1;
	else if (key == 86)
		fdf->camera->gamma += 0.05;
	else if (key == 88)
		fdf->camera->gamma -= 0.05;
	else if (key == 15)
	{
		fdf->camera->alpha = (int)ft_random(fdf->random) / 4;
		fdf->camera->beta = (int)ft_random(fdf->random)  / 4;
		fdf->camera->gamma = (int)ft_random(fdf->random) / 4;
		fdf->camera->x_offset = (int)ft_random(fdf->random) / 4;
		fdf->camera->y_offset = (int)ft_random(fdf->random) / 4;
		fdf->camera->y_offset -= (int)ft_random(fdf->random) / 2;
		fdf->camera->y_offset -= 100;
		fdf->camera->zoom = (int)(ft_random(fdf->random));
		fdf->camera->zoom -= (int)((ft_random(fdf->random) - ft_random(fdf->random)));
	}
	else if (key == 29)
	{
		fdf->camera->zoom = ft_min((WIDTH / ft_get_lenght("W", fdf->map->map) / 2)\
		, (HEIGHT / ft_get_lenght("H", fdf->map->map) / 2));
		fdf->camera->x_offset = 0;
		fdf->camera->y_offset = 0;
		fdf->camera->alpha = 0;
		fdf->camera->beta = 0;
		fdf->camera->gamma = -1.5;
	}
	else if (key == 30)
	{
		if (fdf->angle.projection == CUSTOM)
			fdf->angle.angle += 0.05;
		else
			fdf->translation += 0.25;
	}
	else if (key == 44)
	{
		if (fdf->angle.projection == CUSTOM)
			fdf->angle.angle -= 0.05;
		else
			fdf->translation -= 0.25;
	}
	else if (key == 69)
		fdf->camera->zoom++;
	else if (key == 78)
		fdf->camera->zoom--;
	else if (key >= 123 && key <= 126)
	{
		if (key == 125)
		fdf->camera->y_offset += 10;
		if (key == 126)
		fdf->camera->y_offset -= 10;
		if (key == 124)
		fdf->camera->x_offset += 20;
		if (key == 123)
		fdf->camera->x_offset -= 20;
	}
	else if (key == 49)
	{
		if (fdf->play)
			fdf->play = 0;
		else 
			fdf->play = 1;
	}
	ft_ft_draw(fdf);
	return (0);
}

int	ft_key_press(int key, void *param)
{
	t_fdfc	*fdf;

	fdf = (t_fdfc *)param;
	if (key == 53)
		ft_close(param);
	if (key == 91 || key == 86 \
		|| key == 84 || key == 88 \
		|| key == 15 || key == 29 \
		|| key == 30 || key == 44 \
		|| key == 69 || key == 78 \
		|| key == 49 || key == 98 \
		|| key == 123 || key == 124 \
		|| key == 126 || key == 125)
		ft_rotate(key, fdf);
	if (key == 87)
		ft_change_proyection(fdf);
	ft_printf("key:%d\n", key);
	return (0);
}

void	ft_counter(void *param)
{
	t_fdfc	*fdf;

	fdf = (t_fdfc *)param;
	fdf->random += 1;
	if (fdf->play)
	{
		fdf->camera->gamma += 0.01;
		ft_ft_draw(fdf);
	}
}

