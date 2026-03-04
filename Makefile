# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: wedos-sa <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/12/02 15:50:50 by wedos-sa          #+#    #+#              #
#    Updated: 2025/12/19 11:46:56 by wedos-sa         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		= philo

CC			= cc
CFLAGS		= -g -Wall -Wextra -Werror
LDFLAGS		= -lpthread

SRCS		= main.c		\
			  parser.c		\
			  init.c		\
			  list.c		\
			  routine.c		\
			  forks.c		\
			  monitor.c		\
			  utils.c

OBJS		= $(SRCS:.c=.o)

# ************************* COLORS & SILENCE ************************* #

RESET		= \033[0m
GREEN		= \033[1;32m
YELLOW		= \033[1;33m
RED			= \033[1;31m

SILENT		= @

# ****************************** RULES ******************************* #

all: $(NAME)

$(NAME): $(OBJS)
	$(SILENT)echo "$(GREEN)[PHILO]$(RESET) Compilado"
	$(SILENT)$(CC) $(CFLAGS) $(OBJS) -o $(NAME) $(LDFLAGS)

%.o: %.c philosophers.h
	$(SILENT)$(CC) $(CFLAGS) -c $< -o $@

# ***************************** CLEANING ***************************** #

clean:
	$(SILENT)echo "$(RED)[CLEAN]$(RESET) Removidos: objetos do projeto"
	$(SILENT)rm -f $(OBJS)

fclean: clean
	$(SILENT)echo "$(RED)[FCLEAN]$(RESET) Removidos: objetos e binário"
	$(SILENT)rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re