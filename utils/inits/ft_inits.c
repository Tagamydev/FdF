/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_inits.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 13:46:09 by samusanc          #+#    #+#             */
/*   Updated: 2023/07/04 15:45:36 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <fdf.h>

static char	*ft_map_format(char *str)
{
	char	**sp_dir;
	char	**sp_format;
	char	*result;
	int		i;

	i = 0;
	sp_dir = ft_split(str, '/');
	while (sp_dir[i])
		++i;
	if (i)
		--i;
	sp_format = ft_split(sp_dir[i], '.');
	result = NULL;
	if (sp_format[1])
	{
		if (ft_strncmp(sp_format[1], "fdf", ft_strlen(sp_format[1])) == 0)
			result = ft_strdup(sp_format[0]);
		else
			ft_error_log("INVALID_MAP_NOT_FDF_FILE");
	}
	ft_free_split(sp_dir);
	ft_free_split(sp_format);
	return (result);
}

t_camera	*ft_init_camera(t_fdf *fdf)
{
	t_camera	*camera;

	camera = calloc(sizeof(t_camera), 1);
	if (!camera)
		ft_error_log("CAMERA_INIT");
	if ((WIDTH / fdf->map->width / 2) < (HEIGHT / fdf->map->height / 2))
		camera->zoom = (WIDTH / fdf->map->width / 2);
	else
		camera->zoom = (HEIGHT / fdf->map->height / 2);
	camera->alpha = 0;
	camera->beta = 0;
	camera->gamma = 0;
	camera->z_divisor = 1;
	camera->x_offset = 0;
	camera->y_offset = 0;
	return (camera);
}

t_map	*ft_init_map(void)
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

void	ft_render_init(t_fdf *fdf)
{	
	t_img	*map;
	t_img	*mesh;
	t_img	*bg;

	map = ft_init_img(fdf, &fdf->map_display, WIDTH, HEIGHT);
	if (!fdf->map_display.img)
		ft_error_log("MAP_DISPLAY");
	mesh = ft_init_img(fdf, &fdf->mesh, WIDTH, HEIGHT);
	if (!fdf->mesh.img)
		ft_error_log("MESH_DISPLAY");
	bg = ft_init_img(fdf, &fdf->background, WIDTH, HEIGHT);
	if (!fdf->background.img)
		ft_error_log("BACKGROUND_DISPLAY");
	ft_fill_img(bg, 0x00FFFFFF);
	ft_open_img(fdf, &fdf->ui, "./src/img/ui_a.xpm");
}

t_fdf	*fdf_init(t_map *map, char *title)
{
	t_fdf	*fdf;

	fdf = ft_calloc(sizeof(t_fdf), 1);
	if (!fdf)
		ft_error_log("FDF_MEM_ALLOC");
	fdf->mlx = mlx_init();
	if (!fdf->mlx)
		ft_error_log("MLX_INIT_ERROR");
	title = ft_map_format(title);
	fdf->win = mlx_new_window(fdf->mlx, WIDTH, HEIGHT, title);
	free(title);
	if (!fdf->win)
		ft_error_log("WIN_INIT_ERROR");
	ft_render_init(fdf);
	fdf->mouse = ft_calloc(sizeof(t_mouse), 1);
	if (!fdf->mouse)
		ft_error_log("MOUSE_MEM_ALLOC");
	fdf->map = map;
	return (fdf);
}
