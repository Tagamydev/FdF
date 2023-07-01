# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/06/24 19:28:25 by samusanc          #+#    #+#              #
#    Updated: 2023/07/01 15:13:21 by samusanc         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	= aoeu
CFLAGS	= -Wall -Wextra -Werror -I ./includes -I ./libft/ -I /usr/local/include
CC		= gcc $(CFLAGS)
MLX		= -L /usr/local/lib -lmlx -lm -framework OpenGL -framework AppKit
LIBFT	= -L./libft/ -lft
MAIN	= src/main.c
SRCS	= $(MAIN) \
		  utils/map_parse/ft_read_map.c \
		  utils/map_parse/ft_read_map_utils.c \
		  utils/map_parse/ft_stack_to_arrays.c \
		  utils/inits/ft_inits.c

O_DIR	= ./objects/
OBJS	= $(addprefix $(O_DIR)/, $(SRCS:.c=.o))

$(O_DIR)/%.o: %.c
	@mkdir -p $(@D)
	@$(CC) -c $< -o $(O_DIR)/$(<:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	@make -sC ./libft/
	@$(CC) $(OBJS) $(MLX) $(LIBFT) -o $(NAME)

fclean: clean
	@make -sC ./libft/ fclean
	@rm -f $(NAME)

clean:
	@make -sC ./libft/ clean
	@rm -f $(OBJS)

.PHONY: all
