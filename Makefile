# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: thuynguy <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/11/22 15:30:29 by thuynguy          #+#    #+#              #
#    Updated: 2022/11/22 15:30:31 by thuynguy         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = pipex

CFLAGS = -Wall -Wextra -Werror -g3

FSAN = -fsanitize=address -static-libsan -g

SRC = pipex.c \
	pipex_exit.c \
	pipex_utils.c 

SRC_B = pipex_bonus.c \
	pipex_bonus_exit.c \
	pipex_bonus_heredoc.c \
	pipex_bonus_utils.c 

OBJSFD 	= objs

OBJSFD_B 	= objs_b

OBJS 	= $(addprefix $(OBJSFD)/,$(SRC:.c=.o))

OBJS_B 	= $(addprefix $(OBJSFD_B)/,$(SRC_B:.c=.o))

HDR 		= -I./includes

FT_PRINTF_HDR 	= -I./includes/ft_printf

FT_PRINTF_BINARY	= -L./includes/ft_printf -lftprintf

FT_PRINTF		= ./includes/ft_printf/libftprintf.a

BONUS_DONE_FILE = $(OBJSFD_B)/.done

all: $(FT_PRINTF) $(NAME)

$(FT_PRINTF):
			make -C ./includes/ft_printf

$(OBJSFD):
	mkdir $@

$(OBJSFD)/%.o: %.c | $(OBJSFD)
	gcc -g $(CFLAGS) $(HDR) $(FT_PRINTF_HDR) -c $< -o $@

$(NAME): $(OBJS) $(FT_PRINTF) ./includes/pipex.h
	gcc -g $(CFLAGS) $(OBJS) $(FT_PRINTF_BINARY) -o $@

bonus: $(BONUS_DONE_FILE)

$(BONUS_DONE_FILE): $(FT_PRINTF) $(OBJS_B) ./includes/pipex_bonus.h
	gcc -g $(OBJS_B) $(FT_PRINTF_BINARY) -o $(NAME)
	touch $(BONUS_DONE_FILE)

$(OBJSFD_B):
	mkdir $@

$(OBJSFD_B)/%.o: %.c | $(OBJSFD_B)
	gcc -g $(CFLAGS) $(HDR) $(FT_PRINTF_HDR) -c $< -o $@

clean:
	rm -f $(OBJS) $(OBJS_B) $(BONUS_DONE_FILE)
	rm -rf $(OBJSFD) $(OBJSFD_B)
	make -C ./includes/ft_printf clean

fclean: clean
	rm -f $(NAME)
	make -C ./includes/ft_printf fclean

re: fclean all

.PHONY: all clean fclean re bonus
