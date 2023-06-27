# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/06/24 19:28:25 by samusanc          #+#    #+#              #
#    Updated: 2023/06/27 13:12:37 by samusanc         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CFLAGS	= -Wall -Wextra -Werror -I ./includes -I /usr/local/include
MLX		= -L /usr/local/lib -lmlx -lm -framework OpenGL -framework AppKit
MAIN	= src/main.c
OUT		= aoeu

all:
	gcc $(CFLAGS) $(MAIN) $(MLX) -o $(OUT)
.PHONY: all
