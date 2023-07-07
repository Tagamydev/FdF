/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/26 16:14:03 by samusanc          #+#    #+#             */
/*   Updated: 2023/07/04 17:23:50 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef FDF_H
# define FDF_H
# include <mlx.h>
# include <math.h>
# include <libft.h>
# define BASE_UP "0123456789ABCDEF"
# define BASE_LOW "0123456789abcdef"
# define ERROR_INVALID_MAP "INVALID_MAP\nMake sure the .fdf file has permissions"
# define WIDTH 1920
# define HEIGHT 1080

typedef enum e_projection{
	ISO,
	PARALLEL
}		t_projection;

typedef struct s_point{
	int	x;
	int	y;
	int	z;
	int	color;
}				t_point;

typedef struct s_map{
	int		width;
	int		height;
	int		*coords_arr;
	int		*colors_arr;
	int		z_min;
	int		z_max;
	int		z_range;
}				t_map;

typedef struct s_coord_val{
	int					z;
	int					color;
	struct s_coord_val	*next;
}				t_coord_val;

typedef struct s_img{
	void	*img;
	char	*data_addr;
	int		bits_per_pixel;
	int		line_size;
	int		endian;
	double	transparency;
	int		width;
	int		height;
}				t_img;

typedef struct s_camera{
	t_projection	projection;
	int				zoom;
	double			alpha;
	double			beta;
	double			gamma;
	float			z_divisor;
	int				x_offset;
	int				y_offset;
}				t_camera;

typedef struct s_mouse{
	char	midle_is_pressed;
	char	click_is_pressed;
	int		x;
	int		y;
	int		previous_x;
	int		previous_y;
}				t_mouse;

typedef struct s_fdf{
	void		*mlx;
	void		*win;
	t_map		*map;
	t_img		map_display;
	t_img		mesh;
	t_img		background;
	t_img		ui;
	t_mouse		*mouse;
	t_camera	*camera;
}				t_fdf;

//	Memory, leaks and errors
int			ft_free_split(char **split);
int			ft_free_string(char **split);
void		ft_error_log(char *str);
//	MLX stuff
void		ft_put_pixel(t_img *img, int x, int y, int color);
void		ft_fill_img(t_img *img, int color);
t_img		*ft_init_img(t_fdf *fdf, t_img *img, int width, int height);
int			ft_get_color(int color, char *str);
int			ft_lineal_mix(int c1, int c2, double mix);
int			ft_mix_color(int color1, int color2, double tr);
void		ft_put_display(t_fdf *fdf);
t_img		*ft_open_img(t_fdf *fdf, t_img *img, char *path);
//	Parse map funtions
void		ft_print_coord(t_coord_val *list);
void		ft_free_coord(t_coord_val **list);
int			ft_push(t_coord_val **coords_stack, t_coord_val *new);
int			ft_is_digit_base(char c, int base, char *digits);
int			ft_atoi_base_hex(char *number, int base);
int			ft_is_number_or_color(char **numb);
t_coord_val	*ft_make_coord(char *number);
void		ft_parse_line(t_coord_val **coords_stack, \
char **coords_line, t_map *map);
int			ft_read_map(int fd, t_coord_val **coords_stack, t_map *map);
void		ft_stack_to_arrays(t_coord_val **coords_stack, t_map *map);
//	Inits
t_map		*ft_init_map(void);
t_camera	*ft_init_camera(t_fdf *fdf);
void		ft_render_init(t_fdf *fdf);
t_fdf		*fdf_init(t_map *map, char *title);
//	Main
int			main(int argc, char **argv);

#endif