# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: sopinha <sopinha@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/02/07 18:00:00 by sopinha           #+#    #+#              #
#    Updated: 2026/03/03 19:55:08 by sopinha          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		=	philo

CC			=	cc
CFLAGS		=	-Wall -Wextra -Werror -pthread -I include
RM			=	rm -rf

GREEN		=	\033[0;32m
RED			=	\033[0;31m
YELLOW		=	\033[0;33m
RESET		=	\033[0m

SRC_DIR		=	src
OBJ_DIR		=	obj
FUNC_DIR	=	$(SRC_DIR)/func_aux

HEADER		=	include/philo.h

SRCS		=	main.c \
				$(SRC_DIR)/input.c \
				$(SRC_DIR)/validate.c \
				$(SRC_DIR)/init.c \
				$(SRC_DIR)/cleanup.c \
				$(SRC_DIR)/error.c \
				$(SRC_DIR)/simulation.c \
				$(SRC_DIR)/action.c \
				$(SRC_DIR)/one_philo.c \
				$(SRC_DIR)/routine.c \
				$(SRC_DIR)/monitor.c \
				$(SRC_DIR)/log.c \
				$(SRC_DIR)/utils.c \
				$(SRC_DIR)/utils_time.c \
				$(FUNC_DIR)/ft_isdigit.c \
				$(FUNC_DIR)/ft_isspace.c \
				$(FUNC_DIR)/ft_putstr_fd.c \
				$(FUNC_DIR)/ft_strlen.c \
				$(FUNC_DIR)/ft_unsigned_atol.c

OBJS		=	$(SRCS:%.c=$(OBJ_DIR)/%.o)

all: $(NAME)

$(OBJ_DIR)/%.o: %.c $(HEADER)
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME)
	@printf "$(GREEN)━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━$(RESET)\n"
	@printf "$(GREEN)✓ $(NAME) compiled successfully!$(RESET)\n"
	@printf "$(GREEN)━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━$(RESET)\n"

clean:
	@$(RM) $(OBJ_DIR)
	@printf "$(RED)✗ Object files removed$(RESET)\n"

fclean: clean
	@$(RM) $(NAME)
	@printf "$(RED)✗ $(NAME) removed$(RESET)\n"

re: fclean all

drd: $(NAME)
	@printf "$(YELLOW)Running DRD (thread error detector)...$(RESET)\n"
	valgrind --tool=drd --log-file=drd.log ./$(NAME) 5 800 200 200 4
	@printf "$(GREEN)✓ Check drd.log for results$(RESET)\n"

helgrind: $(NAME)
	@printf "$(YELLOW)Running Helgrind (thread error detector)...$(RESET)\n"
	valgrind --tool=helgrind --log-file=helgrind.log ./$(NAME) 5 800 200 200 4
	@printf "$(GREEN)✓ Check helgrind.log for results$(RESET)\n"

valgrind: $(NAME)
	@printf "$(YELLOW)Running Valgrind (memory leak detector)...$(RESET)\n"
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes \
		--log-file=valgrind.log ./$(NAME) 5 800 200 200 7
	@printf "$(GREEN)✓ Check valgrind.log for results$(RESET)\n"

.PHONY: all clean fclean re drd helgrind valgrind