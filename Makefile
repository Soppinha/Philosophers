# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: svaladar <svaladar@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/03/04 16:50:45 by sofia             #+#    #+#              #
#    Updated: 2026/03/16 18:10:15 by svaladar         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		=	philo

CC			=	cc
CFLAGS		=	-g -Wall -Wextra -Werror
LDFLAGS		=	-lpthread
SRC_DIR		=	src
OBJ_DIR		=	obj
HEADER		=	include/philo.h

SRCS		= $(SRC_DIR)/main.c			\
			  $(SRC_DIR)/error.c		\
			  $(SRC_DIR)/parser.c		\
			  $(SRC_DIR)/init.c			\
			  $(SRC_DIR)/list.c			\
			  $(SRC_DIR)/routine.c		\
			  $(SRC_DIR)/forks.c		\
			  $(SRC_DIR)/monitor.c		\
			  $(SRC_DIR)/log.c			\
			  $(SRC_DIR)/one_philo.c	\
			  $(SRC_DIR)/thread.c		\
			  $(SRC_DIR)/action.c		\
			  $(SRC_DIR)/time.c			\
			  $(SRC_DIR)/memory.c		\
			  $(SRC_DIR)/utils.c

OBJS		= $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))

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

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(HEADER)
	$(SILENT)mkdir -p $(dir $@)
	$(SILENT)$(CC) $(CFLAGS) -Iinclude -c $< -o $@

# ***************************** CLEANING ***************************** #

clean:
	$(SILENT)echo "$(RED)[CLEAN]$(RESET) Removidos: objetos do projeto"
	$(SILENT)rm -rf $(OBJ_DIR)

fclean: clean
	$(SILENT)echo "$(RED)[FCLEAN]$(RESET) Removidos: objetos e binário"
	$(SILENT)rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re