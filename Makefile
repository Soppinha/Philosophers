# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: sopinha <sopinha@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/02/07 18:00:00 by sopinha           #+#    #+#              #
#    Updated: 2026/02/28 18:41:31 by sopinha          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		=	philo

CC			=	cc
CFLAGS		=	-Wall -Wextra -Werror -pthread
DFLAGS		=	-g -fsanitize=thread
RM			=	rm -rf

GREEN		=	\033[0;32m
RED			=	\033[0;31m
YELLOW		=	\033[0;33m
RESET		=	\033[0m

SRC_DIR		=	.
OBJ_DIR		=	obj
FUNC_DIR	=	func_aux

SRCS		=	main.c \
				input.c \
				validate.c \
				init.c \
				cleanup.c \
				error.c \
				simulation.c \
				action.c \
				one_philo.c \
				routine.c \
				monitor.c \
				log.c \
				utils.c \
				utils_time.c

FUNC_SRCS	=	$(FUNC_DIR)/ft_isdigit.c \
				$(FUNC_DIR)/ft_isspace.c \
				$(FUNC_DIR)/ft_putstr_fd.c \
				$(FUNC_DIR)/ft_strlen.c \
				$(FUNC_DIR)/ft_unsigned_atol.c

ALL_SRCS	=	$(SRCS) $(FUNC_SRCS)

OBJS		=	$(SRCS:%.c=$(OBJ_DIR)/%.o)
FUNC_OBJS	=	$(FUNC_SRCS:$(FUNC_DIR)/%.c=$(OBJ_DIR)/%.o)
ALL_OBJS	=	$(OBJS) $(FUNC_OBJS)

HEADER		=	philo.h

all: $(NAME)

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)
	@echo "$(YELLOW)Creating object directory...$(RESET)"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(HEADER) | $(OBJ_DIR)
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "$(GREEN)✓$(RESET) Compiled: $<"

$(OBJ_DIR)/%.o: $(FUNC_DIR)/%.c $(HEADER) | $(OBJ_DIR)
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "$(GREEN)✓$(RESET) Compiled: $<"

$(NAME): $(ALL_OBJS)
	@$(CC) $(CFLAGS) $(ALL_OBJS) -o $(NAME)
	@echo "$(GREEN)━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━$(RESET)"
	@echo "$(GREEN)✓ $(NAME) compiled successfully!$(RESET)"
	@echo "$(GREEN)━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━$(RESET)"

clean:
	@$(RM) $(OBJ_DIR)
	@echo "$(RED)✗ Object files removed$(RESET)"

fclean: clean
	@$(RM) $(NAME)
	@echo "$(RED)✗ $(NAME) removed$(RESET)"

re: fclean all

debug: CFLAGS += $(DFLAGS)
debug: fclean $(NAME)
	@echo "$(YELLOW)⚠ Debug build with thread sanitizer enabled$(RESET)"

helgrind: $(NAME)
	@echo "$(YELLOW)Running Helgrind (thread error detector)...$(RESET)"
	valgrind --tool=helgrind --log-file=helgrind.log ./$(NAME) 5 800 200 200
	@echo "$(GREEN)✓ Check helgrind.log for results$(RESET)"

valgrind: $(NAME)
	@echo "$(YELLOW)Running Valgrind (memory leak detector)...$(RESET)"
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes \
		--log-file=valgrind.log ./$(NAME) 5 800 200 200 7
	@echo "$(GREEN)✓ Check valgrind.log for results$(RESET)"

.PHONY: all clean fclean re debug helgrind valgrind