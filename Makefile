# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rschramm <rschramm@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2016/11/30 12:55:46 by rschramm          #+#    #+#              #
#    Updated: 2017/11/14 14:27:22 by rschramm         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = TERM
SRC = ./src/main.c

LIBFT = ./libft/libftprintf.a
LIBFTHEAD = -I ./libft/includes/

GCC = gcc
HEADERS = -I ./includes/ $(LIBFTHEAD)
FLAGS += -Wall -Wextra -Werror

COMPILE = $(GCC) $(FLAGS) $(SRC) $(HEADERS) $(LIBFT) -o $(NAME) -g

OUTPUTBUILD = echo "\033[32mBuilding TERM\033[0m"
OUTPUTCLEAN = echo "\033[31mRemoving Objects\033[0m"
OUTPUTFCLEAN = echo "\033[31mRemoving Executable\033[0m"
OUTPUTGO = echo "\033[92mSystem Ready :D\033[0m"
OUTPUTLIB = echo "\033[36;21mRebuilding Lib\033[0m"
OUTPUTRE = echo "\033[96;21mRebuilding Malloc\033[0m"

all: $(NAME)

$(NAME):
	@cd libft && make re
	@$(COMPILE)
	@$(OUTPUTGO)

clean:
	@$(OUTPUTCLEAN)
	@rm -f *.o

fclean: clean
	@$(OUTPUTFCLEAN)
	@rm -f $(NAME)

re:
	@$(OUTPUTRE)
	@make fclean
	@make

.PHONY: all clean fclean re
