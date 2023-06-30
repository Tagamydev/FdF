/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/26 16:14:03 by samusanc          #+#    #+#             */
/*   Updated: 2023/06/30 19:25:29 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef FDF_H
# define FDF_H
# include <mlx.h>
# include <math.h>
# include <libft.h>
# define BASE_UP "0123456789ABCDEF"
# define BASE_LOW "0123456789abcdef"

typedef	struct	s_map{
	int		width;
	int		height;
	int		*coords_arr;
	int		*colors_arr;
	int		z_min;
	int		z_max;
	int		z_range;
}				t_map;

typedef	struct	s_coord_val{
	int					z;
	int					color;
	struct s_coord_val	*next;
}				t_coord_val;

typedef	struct	s_img{
	void	*img;
	char	*data_addr;
	int		bits_per_pixel;
	int		line_size;
	int		endian;
	double	transparency;
	int		width;
	int		height;
}				t_img;

typedef	struct	s_fdf{
	void	*mlx;
	void	*win;
	t_img	map;
	t_img	mesh;
	t_img	background;
	t_img	ui;
}				t_fdf;

#endif
