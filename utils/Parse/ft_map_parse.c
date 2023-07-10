/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_map_parse.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/10 14:16:30 by samusanc          #+#    #+#             */
/*   Updated: 2023/07/10 15:36:13 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <fdf.h>

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
		col->color = 0x00FFFFFF;
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

