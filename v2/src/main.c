/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/26 16:17:36 by samusanc          #+#    #+#             */
/*   Updated: 2023/07/07 19:32:28 by samusanc         ###   ########.fr       */
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
	int		i;

	i = 0;

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
	//*x = (tmp_x - tmp_y) * cos(0.523599);
	//*x = -z + (tmp_x + tmp_y) * sin(0.523599);
	z = 0;
}

t_point	au(t_point point, t_fdf *fdf)
{
	int zoom = 100;
	int x;
	int y;

	point.x *= zoom;
	point.y *= zoom;
	point.x -= (fdf->map->width * zoom) / 2;
	point.x -= (fdf->map->height * zoom) / 2;
	x = point.x;
	y = point.y;
	//point.x = (x - y) * cos(0.523599);
	//point.y = (x + y) * sin(0.523599);
	point.x += WIDTH / 2;
	point.y += HEIGHT + fdf->map->height / 2;
	fdf = NULL;
	return (point);
}

t_point	ft_point(int x, int y, t_map *map)
{
	t_point	point;

	point.x = x;
	point.y = y;
	point.color = 0x00000000;
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
		ft_free_line_map(tmp);
		free(tmp);
		tmp = tmp2;
		tmp2 = tmp->next;
	}
	free(tmp);
}

int	main(int argc, char **argv)
{
	t_height	*map;
	if (argc == 2)
	{
		map = ft_make_map(argv[1]);
		ft_print_mapi(map);
		ft_free_map(map);
	}
	atexit(leaks);
	return (0);	
	/*
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
		printf("Alles gut IN OPEN\n");
		map = ft_init_map();
		printf("Alles gut in INIT MAP\n");
		ft_read_map(fd, &coords_stack, map);
		printf("Alles gut in READ MAP\n");
		fdf = fdf_init(map, argv[1]);
		printf("Alles gut in FDF INIT\n");
		ft_stack_to_arrays(&coords_stack, map);
		printf("Alles gut in STACK to ARRAYS\n");
		fdf->camera = ft_init_camera(fdf);
		printf("Alles gut in INIT CAMERA\n");
		ft_draw(fdf->map, fdf);
		printf("Alles gut in DRAW\n");
		ft_print_map(map);
		printf("Alles gut int PRINT MAP\n");
		mlx_loop(fdf->mlx);
	}
	ft_printf("usage: ./fdf 'map.fdf'\n");
	exit(0);
	*/
}
