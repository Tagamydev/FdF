/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/26 16:17:36 by samusanc          #+#    #+#             */
/*   Updated: 2023/07/08 20:31:55 by samusanc         ###   ########.fr       */
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

	if (x >= 0 && y >= 0 && x < img->width && y < img->height && y < HEIGHT && x < WIDTH)
	{
		dst = img->data_addr + ((y * img->line_size) + \
		((x * (img->bits_per_pixel / 8))));
		*(unsigned int*)dst = color;
	}
}

t_img	*ft_init_img(t_fdfc *fdf,t_img *img, int width, int height)
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

t_img	*ft_open_img(t_fdfc *fdf, t_img *img, char *path)
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
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//		pasos a hacer
//		parseo
//			pasar de un documento a una doble lista enlazada
//		dibujo
//			a partir de un punto x y y obtener dos puntos para trazar una linea
//

char	*get_all_doc(int fd)
{
	static struct s_data_base	file[OPEN_MAX];
	int							read_number;
	char						*result;

	read_number = 1;
	if (read(fd, 0, 0) < 0 || fd < 0 \
	|| BUFFER_SIZE < 1 || fd > OPEN_MAX)
		return (ft_error(&file[fd]));
	while (read_number)
		if (!ft_read_line(&file[fd], &read_number, fd))
			return (0);
	result = ft_mem_cpy(file[fd].saved_line, file[fd].saved_line_len);
	ft_error(&file[fd]);
	return (result);
}

t_width		*ft_new_column_node(char *str)
{
	char	**numbers;
	t_width	*col;

	numbers = ft_split(str, ',');
	if (!numbers)
		ft_error_log("SPLIT_NUMBERS");
	col = malloc(sizeof(t_width));
	if (!col)
		ft_error_log("MALLOC_MAP");
	col->z = ft_atoi_base(numbers[0], 10);
	if (numbers[1])
		col->color = ft_atoi_base(numbers[1], 16);
	else
		col->color = 0x00000000;
	col->next = NULL;
	ft_free_split(numbers);
	return (col);
}

void	ft_atach_column(t_width **list, t_width *node)
{
	t_width	*tmp;
	
	tmp = *list;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = node;
}

t_height	*ft_new_height_node(char *str)
{
	char		**columns;
	t_height	*map;
	int	i;

	i = 1;
	columns = ft_split(str, ' ');
	if (!columns)
		ft_error_log("SPLIT_COLUMNS");
	map = malloc(sizeof(t_height));
	if (!map)
		ft_error_log("MALLOC_MAP");
	map->next = NULL;
	map->line = ft_new_column_node(columns[0]);
	while (columns[i])
		ft_atach_column(&map->line, ft_new_column_node(columns[i++]));
	ft_free_split(columns);
	return (map);
}

void	ft_atach_line(t_height **list, t_height *node)
{
	t_height	*tmp;
	
	tmp = *list;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = node;
}


t_height	*ft_init_height(char *doc)
{
	char **lines;
	t_height	*map;
	int	i;

	i = 1;
	map = NULL;
	lines = ft_split(doc, '\n');
	if (!lines)
		ft_error_log("SPLIT_LINES");
	map = ft_new_height_node(lines[0]);
	while (lines[i])
		ft_atach_line(&map, ft_new_height_node(lines[i++]));
	ft_free_split(lines);
	return (map);
}

void	ft_prove_map(t_height *map)
{
	t_height *tmp1;
	t_width	*tmp2;
	int	i;
	int	j;

	i = 0;
	tmp1 = map;
	if (!tmp1 )
		return ;
	tmp2 = tmp1->line;
	while (tmp2)
	{
		tmp2 = tmp2->next;
		i++;
	}
	while (tmp1)
	{
		tmp2 = tmp1->line;
		j = 0;
		while (tmp2)
		{
			tmp2 = tmp2->next;
			++j;
		}
		if (j != i)
			ft_error_log("INVALID_MAP");
		tmp1 = tmp1->next;
	}
}

t_height	*ft_make_map(char *str)
{
	int fd;
	char *doc;
	t_height	*map;

	map = NULL;
	fd = open(str, O_RDONLY);
	if (fd == -1)
		ft_error_log("FD_OPEN");
	doc = get_all_doc(fd);
	if (!doc)
		ft_error_log("GNL");
	map = ft_init_height(doc);
	fd = close(fd);
	if (fd == -1)
		ft_error_log("FD_CLOSE");
	ft_free_string(&doc);
	ft_prove_map(map);
	return (map);
}

void	ft_print_line_map(t_width *line, int y)
{
	t_width	*tmp;
	int		x;

	x = 0;
	tmp = line;
	while (tmp)
	{
		ft_printf("(%d, %d) = number:%d, color:%p\n", x++, y, tmp->z, tmp->color);
		tmp = tmp->next;
	}
}

void	ft_print_mapi(t_height *map)
{
	t_height *tmp;
	int	y;

	y = 0;
	tmp = map;
	while (map)
	{
		ft_print_line_map(map->line, y++);
		map = map->next;
	}
}

void	ft_free_line_map(t_width *map)
{
	t_width	*tmp;
	t_width	*tmp2;

	if (!map)
		return ;
	tmp = map;
	tmp2 = tmp->next;
	while (tmp2)
	{
		free(tmp);
		tmp = tmp2;
		tmp2 = tmp->next;
	}
	free(tmp);
}

void	ft_free_map(t_height *map)
{
	t_height	*tmp;
	t_height	*tmp2;

	if (!map)
		return ;
	tmp = map;
	tmp2 = tmp->next;
	while (tmp2)
	{
		ft_free_line_map(tmp->line);
		free(tmp);
		tmp = tmp2;
		tmp2 = tmp->next;
	}
	ft_free_line_map(tmp->line);
	free(tmp);
}

int	ft_get_col(t_height *map, int x, int y)
{
	t_height	*tmp1;
	t_width		*tmp2;
	int	i;
	int	j;

	i = 0;
	tmp1 = map;
	tmp2 = tmp1->line;
	while (tmp1)
	{
		if (i++ == y)
		{
			j = 0;
			tmp2 = tmp1->line;
			while (tmp2)
			{
				if (j++ == x)
					break ;
				tmp2 = tmp2->next;
			}
			break ;
		}
		tmp1 = tmp1->next;
	}
	return (tmp2->color);
}

int	ft_get_z(t_height *map, int x, int y)
{
	t_height	*tmp1;
	t_width		*tmp2;
	int	i;
	int	j;

	i = 0;
	tmp1 = map;
	tmp2 = tmp1->line;
	while (tmp1)
	{
		if (i++ == y)
		{
			j = 0;
			tmp2 = tmp1->line;
			while (tmp2)
			{
				if (j++ == x)
					break ;
				tmp2 = tmp2->next;
			}
			break ;
		}
		tmp1 = tmp1->next;
	}
	return (tmp2->z);
}


t_point	*ft_get_point(t_height *map, int x, int y)
{
	t_point	*point;

	point = malloc(sizeof(t_point));
	if (!point)
		ft_error_log("MALLOC_POINT");
	point->z = ft_get_z(map, x, y);
	point->color = ft_get_col(map, x, y);
	point->x = x;
	point->y = y;
	return (point);
}

t_camera	*ft_init_cam(void)
{
	t_camera *camera;

	camera = malloc(sizeof(t_camera));
	if (!camera)
		ft_error_log("MALLOC_CAMERA");
	camera->zoom = 1;
	camera->alpha = 0;
	camera->beta = 0;
	camera->gamma = 0;
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
	ft_fill_img(&fdf->background, 0x00FFFFFF);
	//ft_open_img(fdf, &fdf->background, "./src/img/testui.xpm");
	fdf->camera = ft_init_cam();
	*fdfp = fdf;
	return ;
}

int	ft_get_lenght(char *str, t_height *map)
{
	t_height	*tmp1;
	t_width		*tmp2;
	int	i;

	i = 0;
	tmp1 = map;
	tmp2 = map->line;
	if (*str == 'H')
	{
		while (tmp1)
		{
			tmp1 = tmp1->next;
			i++;
		}
	}
	else
	{
		while (tmp2)
		{
			tmp2 = tmp2->next;
			i++;
		}
	}
	return (i);
}

void	ft_rotate_x(int *y, int *z, double alpha)
{
	int previous_y;

	previous_y = *y;
	*y = previous_y * cos(alpha) + *z * sin(alpha);
	*z = -previous_y * sin(alpha) + *z * cos(alpha);
}

void	ft_rotate_y(int *x, int *z, double beta)
{
	int previous_x;

	previous_x = *x;
	*x = previous_x * cos(beta) + *z * sin(beta);
	*z = -previous_x * sin(beta) + *z * cos(beta);
}

void	ft_rotate_z(int *x, int *y, double gamma)
{
	int previous_x;
	int previous_y;

	previous_x = *x;
	previous_y = *y;
	*x = previous_x * cos(gamma) - previous_y * sin(gamma);
	*y = previous_x * sin(gamma) + previous_y * cos(gamma);
}

void	ft_iso(int *x, int *y, int z)
{
	int	tmp_x;
	int	tmp_y;

	tmp_x = *x;
	tmp_y = *y;
	*x = (tmp_x - tmp_y) * cos(0.523599);
	*y = -z + (tmp_x + tmp_y) * sin(0.523599);
	z = 0;
}

void	ft_make_maths(t_point *point, t_fdfc *fdf)
{
	point->x *= ZOOM;
	point->y *= ZOOM;
	point->z *= ZOOM / fdf->camera->z_divisor;
	point->x -= (MAP_WIDTH * ZOOM) / 2;
	point->y -= (MAP_HEIGHT * ZOOM) / 2;
	ft_rotate_x(&point->y, &point->z, ALPHA);
	ft_rotate_y(&point->x, &point->z, BETA);
	ft_rotate_z(&point->x, &point->y, GAMMA);
	ft_iso(&point->x, &point->y, point->z);
	point->x += (WIDTH) / 2 + OFFSET_X;
	point->y += (HEIGHT + MAP_HEIGHT * ZOOM) / 2 + OFFSET_Y;
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
		//ft_put_pixel(img, point.x, point.y, ft_mix_color(ft_make_translucid(point.color), point.color, alpha));
		ft_put_pixel(img, point.x, point.y, point.color);
}

void	ft_put_line(t_point f, t_point s, t_img *map_display)
{
    int x0 = f.x;
    int y0 = f.y;
    int x1 = s.x;
    int y1 = s.y;
	t_point	tmp;
	t_points	line;


	line.start = f;
	line.end = s;
    int dx = ft_abs(x1 - x0);
    int dy = ft_abs(y1 - y0);
    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;
    int err = dx - dy;

    while (1) {
		ft_clone(&tmp, line, x0, y0);
		ft_draw_pixel(tmp, map_display);
        if (x0 == x1 && y0 == y1)
            break;

        int e2 = 2 * err;

        if (e2 > -dy) {
            err -= dy;
            x0 += sx;
        }

        if (e2 < dx) {
            err += dx;
            y0 += sy;
        }
    }
}

void	ft_ft_draw(t_fdfc *fdf)
{
	int	x;
	int	y;

	x = 0;
	y = 0;
	while (y < MAP_HEIGHT)
	{
		x = 0;
		while (x < MAP_WIDTH)
		{
			if (x != (MAP_WIDTH - 1))
				ft_put_line(ft_proyect(ft_get_point(fdf->map->map, x, y), fdf),\
				ft_proyect(ft_get_point(fdf->map->map, x + 1, y), fdf), &fdf->map_display);
			if (y != (MAP_HEIGHT - 1))
				ft_put_line(ft_proyect(ft_get_point(fdf->map->map, x, y), fdf), \
				ft_proyect(ft_get_point(fdf->map->map, x, y + 1), fdf), &fdf->map_display);
			x++;
		}
		y++;
	}
	mlx_put_image_to_window(fdf->mlx, fdf->win, fdf->background.img, 0, 0);
	mlx_put_image_to_window(fdf->mlx, fdf->win, fdf->map_display.img, 0, 0);
}

//					Tareas para tomorrow, iniciar la cam, empezar a dibujar,
//					IMPLEMENTA LA VERIFICACION DE MAPA!!!!!!!!!!!!!!!!!!
//					lograr rotar los puntos, y si se puede implementar los controles

int	ft_min(int x, int y)
{
	if (x < y)
		return (x);
	return (y);
}

t_fdfc	*ft_set_up(char *str)
{
	t_fdfc		*fdf;
	t_height	*map;
	int	width;
	int	height;

	map = ft_make_map(str);
	ft_init_fdf(&fdf, str);
	fdf->map = malloc(sizeof(t_mapi));
	if (!fdf->map)
		ft_error_log("MALLOC_MAPI");
	fdf->map->map = map;
	width = ft_get_lenght("W", fdf->map->map);
	height = ft_get_lenght("H", fdf->map->map);
	MAP_HEIGHT = height;
	MAP_WIDTH = width;
	ZOOM = ft_min((WIDTH / width / 2), (HEIGHT / height / 2));
	return (fdf);
}

int	main(int argc, char **argv)
{
	t_fdfc		*fdf;

	atexit(leaks);
	fdf = NULL;
	if (argc == 2)
	{
		fdf = ft_set_up(argv[1]);
		ft_ft_draw(fdf);
		//ft_controls(fdf);
		mlx_loop(fdf->mlx);
	}
	ft_printf("usage: ./fdf 'map.fdf'\n");
	exit(0);
	return (0);	
}
