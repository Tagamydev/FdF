/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/26 16:14:03 by samusanc          #+#    #+#             */
/*   Updated: 2023/07/09 22:38:18 by samusanc         ###   ########.fr       */
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
# define ZOOM fdf->camera->zoom
# define MAP_WIDTH fdf->map->width
# define MAP_HEIGHT fdf->map->height
# define ALPHA fdf->camera->alpha
# define BETA fdf->camera->beta
# define GAMMA fdf->camera->gamma
# define OFFSET_X fdf->camera->x_offset
# define OFFSET_Y fdf->camera->y_offset
# define Z_DIV fdf->camera->z_divisor
# define PROJ fdf->angle.projection
//------------------------------------------------------------------------//

//--------------------------------------------------------------------------//
typedef enum e_projection{
	ISO,
	ISO_GAMES,
	PARALLEL,
	CUSTOM,
	CONIC,
	TOTAL
}		t_projection;

typedef struct s_angle{
	t_projection	projection;
	double			angle;
}				t_angle;

typedef struct s_point{
	int	x;
	int	y;
	int	z;
	int	color;
}				t_point;

typedef struct s_points{
	t_point	start;
	t_point end;
}				t_points;

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
//------------------------------------------------------NUEVA ERA DESDE AQUI-----------------------------

typedef struct s_width{
	int				z;
	int				color;
	struct s_width	*next;
}				t_width;

typedef struct s_height{
	t_width			*line;
	struct s_height	*next;
}				t_height;

typedef struct s_mapi{
	t_height	*map;
	int			height;
	int			width;
}				t_mapi;

typedef struct s_fdf{
	void		*mlx;
	void		*win;
	t_mapi		*map;
	t_img		map_display;
	t_img		background;
	t_camera	*camera;
	t_height	*grid;
	t_angle		angle;
	double			translation;
	unsigned char	random;
	int				play;
}				t_fdfc;
//---------------------------------------------------------------------------------------------------------

//	Memory, leaks and errors
//	Parse map funtions
int			ft_is_digit_base(char c, int base, char *digits);
int			ft_atoi_base_hex(char *number, int base);
int			ft_atoi_base(char *number, int base);
int			ft_is_number_or_color(char **numb);
/////////////////////
char		*ft_map_format(char *str);
void		ft_error_log(char *str);
void		ft_put_pixel(t_img *img, int x, int y, int color);
t_img		*ft_init_img(t_fdfc *fdf,t_img *img, int width, int height);
void 		ft_fill_img(t_img *img, int color);
int			ft_get_color(int color, char *str);
int			ft_lineal_mix(int c1, int c2, double mix);
int			ft_mix_color(int color1, int color2, double tr);
t_img		*ft_open_img(t_fdfc *fdf, t_img *img, char *path);
int			ft_free_string(char **split);
int			ft_free_string(char **split);
int			ft_free_split(char **split);
int			ft_abs(int x);
int			ft_make_translucid(int color1);
double		ft_distance_2_points(t_point a, t_point b);
int			ft_color_degradade(t_point init, t_point end, t_point current);
double		ft_get_angle(t_fdfc *fdf);
char		*get_all_doc(int fd);
t_width		*ft_new_column_node(char *str);
void		ft_atach_column(t_width **list, t_width *node);
t_height	*ft_new_height_node(char *str);
void		ft_atach_line(t_height **list, t_height *node);
t_height	*ft_init_height(char *doc);
void		ft_prove_map(t_height *map);
t_height	*ft_make_map(char *str);
void		ft_print_line_map(t_width *line, int y);
void		ft_print_mapi(t_height *map);
void		ft_free_line_map(t_width *map);
void		ft_free_map(t_height *map);
int			ft_get_col(t_height *map, int x, int y);
int			ft_get_z(t_height *map, int x, int y);
t_point		*ft_get_point(t_height *map, int x, int y);
t_camera	*ft_init_cam(void);
void		ft_init_fdf(t_fdfc **fdfp, char *title);
int			ft_get_lenght(char *str, t_height *map);
void		ft_rotate_x(int *y, int *z, double alpha);
void		ft_rotate_y(int *x, int *z, double beta);
void		ft_rotate_z(int *x, int *y, double gamma);
void		ft_iso(int *x, int *y, int z, t_fdfc *fdf);
void		ft_make_maths(t_point *point, t_fdfc *fdf);
t_point		ft_proyect_minimap(t_point *point, t_fdfc *fdf);
t_point		ft_proyect(t_point *point, t_fdfc *fdf);
void		ft_clone(t_point *point, t_points line, int x, int y);
void		ft_draw_pixel(t_point point, t_img *img);
void		ft_put_line(t_point f, t_point s, t_img *map_display);
void		ft_ft_draw(t_fdfc *fdf);
int			ft_min(int x, int y);
t_fdfc		*ft_set_up(char *str);
void		ft_close(void *param);
void		ft_change_proyection(t_fdfc *fdf);
int			ft_random(unsigned char seed);
int			ft_rotate(int key, t_fdfc *fdf);
int			ft_key_press(int key, void *param);
void		ft_counter(void *param);
void		ft_controls(t_fdfc *fdf);
int			main(int argc, char **argv);

#endif
