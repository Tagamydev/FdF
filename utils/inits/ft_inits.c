/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_inits.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/10 15:32:29 by samusanc          #+#    #+#             */
/*   Updated: 2023/07/10 15:50:16 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <fdf.h>

t_camera	*ft_init_cam(void)
{
	t_camera *camera;

	camera = malloc(sizeof(t_camera));
	if (!camera)
		ft_error_log("MALLOC_CAMERA");
	camera->zoom = 1;
	camera->alpha = 0;
	camera->beta = 0;
	camera->gamma = -1.55;
	camera->z_divisor = 1;
	camera->x_offset = 0;
	camera->y_offset = 0;
	return (camera);
}

void	ft_init_fdf(t_fdfc **fdfp, char *title)
{
	t_fdfc	*fdf;


	fdf = malloc(sizeof(t_fdfc));
	if (!fdf)
		ft_error_log("MALLOC_FDF");
	fdf->mlx = mlx_init();
	if (!fdf->mlx)
		ft_error_log("MLX_INIT");
	title = ft_map_format(title);
	fdf->win = mlx_new_window(fdf->mlx, WIDTH, HEIGHT, title);
	free(title);
	if (!fdf->win)
		ft_error_log("MLX_WIN");
	ft_init_img(fdf, &fdf->map_display, WIDTH, HEIGHT);
	if (!fdf->map_display.img)
		ft_error_log("INIT_MAP_DISPLAY");
	ft_fill_img(&fdf->map_display, 0xFF000000);
	ft_init_img(fdf, &fdf->background, WIDTH, HEIGHT);
	ft_fill_img(&fdf->background, 0x00000000);
	fdf->angle.projection = ISO_GAMES;
	fdf->angle.angle = ft_get_angle(fdf);
	fdf->translation = 1;
	fdf->play = 0;
	fdf->camera = ft_init_cam();
	*fdfp = fdf;
	return ;
}
