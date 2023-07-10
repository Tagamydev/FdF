# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/06/24 19:28:25 by samusanc          #+#    #+#              #
#    Updated: 2023/07/10 15:53:20 by samusanc         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	= fdf
MEMORY	= ./utils/Memory/
ERR		= ./utils/Error/
PARSE	= ./utils/Parse/
MATH	= ./utils/Math/
GP		= ./utils/Get_point/
INITS	= ./utils/Inits/
RENDER	= ./utils/Render/
KEYS	= ./utils/Keys/
FT_MLX	= ./utils/Ft_mlx/
CFLAGS	= -Wall -Wextra -Werror -I ./includes -I ./libft/ -I /usr/local/include
CC		= gcc $(CFLAGS)
MLX		= -L /usr/local/lib -lmlx -lm -framework OpenGL -framework AppKit
LIBFT	= -L./libft/ -lft
MAIN	= src/main.c
SRCS	= $(MAIN) \
		  \
		  $(MEMORY)ft_free_utils.c \
		  $(ERR)ft_error_log.c \
		  \
		  $(PARSE)ft_number_parse.c \
		  $(PARSE)ft_win_title.c \
		  $(PARSE)ft_map_parse.c \
		  $(PARSE)ft_map_utils.c \
		  \
		  $(MATH)ft_maths.c \
		  $(MATH)ft_rotate.c \
		  $(MATH)ft_maths_4_render.c \
		  \
		  $(FT_MLX)ft_mlx_funtions.c \
		  \
		  $(GP)ft_get_point.c \
		  \
		  $(INITS)ft_inits.c \
		  \
		  $(RENDER)ft_render.c \
		  \
		  $(KEYS)ft_keys.c \
		  \
		  utils/Gnl_copy/get_all_doc.c

O_DIR	= ./objects/
OBJS	= $(addprefix $(O_DIR)/, $(SRCS:.c=.o))

$(O_DIR)/%.o: %.c
	@mkdir -p $(@D)
	@$(CC) -c $< -o $(O_DIR)/$(<:.c=.o)

all: $(NAME) includes/fdf.h

$(NAME): $(OBJS)
	@make -sC ./libft/
	@$(CC) $(OBJS) $(MLX) $(LIBFT) -o $(NAME)

re: fclean all

fclean: clean
	@make -sC ./libft/ fclean
	@rm -f $(NAME)

clean:
	@make -sC ./libft/ clean
	@rm -f $(OBJS)

.PHONY: all
