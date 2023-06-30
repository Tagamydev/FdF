# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/06/24 19:28:25 by samusanc          #+#    #+#              #
#    Updated: 2023/06/30 20:10:32 by samusanc         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CFLAGS	= -Wall -Wextra -Werror -I ./includes -I ./libft/ -I /usr/local/include -L./libft/ -lft 
MLX		= -L /usr/local/lib -lmlx -lm -framework OpenGL -framework AppKit
MAIN	= src/main.c
OUT		= aoeu

all: $(MAIN)
	make -sC ./libft/
	gcc $(CFLAGS) $(MAIN) $(MLX) -o $(OUT)
.PHONY: all
