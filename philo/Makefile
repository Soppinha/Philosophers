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

# ********************************* OUTPUT ********************************** #

TAG			=	philo
RESET		=	\033[0m
C_TAG		=	\033[1;34m
C_BUILD		=	\033[0;32m
C_DONE		=	\033[1;32m
C_CLEAN		=	\033[0;33m
C_RM		=	\033[0;31m
LOG			=	printf "$(C_TAG)%-12s$(RESET) %b%-10s$(RESET) %s\n" "[$(TAG)]"

# ********************************** BUILD ********************************** #

CC			=	cc
CFLAGS		=	-Wall -Wextra -Werror
LDFLAGS		=	-lpthread
RM			=	rm -rf

SRC_DIR		=	src
OBJ_DIR		=	obj
INC_DIR		=	include

INCLUDES	=	-I$(INC_DIR)
HEADERS		=	$(INC_DIR)/philo.h

PARSING		=	parser.c error.c

SETUP		=	init.c list.c thread.c

SIMULATION	=	routine.c forks.c action.c monitor.c one_philo.c

UTILS		=	log.c time.c memory.c utils.c

SRC_FILES	=	main.c \
				$(addprefix parsing/, $(PARSING)) \
				$(addprefix setup/, $(SETUP)) \
				$(addprefix simulation/, $(SIMULATION)) \
				$(addprefix utils/, $(UTILS))

OBJS		=	$(addprefix $(OBJ_DIR)/, $(SRC_FILES:.c=.o))

all:		$(NAME)

$(NAME):	$(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME) $(LDFLAGS)
	@$(LOG) "$(C_DONE)" linked "$(NAME)"

$(OBJ_DIR)/%.o:	$(SRC_DIR)/%.c $(HEADERS)
	@mkdir -p $(dir $@)
	@$(LOG) "$(C_BUILD)" compiling "$<"
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	@$(RM) $(OBJ_DIR)
	@$(LOG) "$(C_CLEAN)" cleaned objects

fclean:		clean
	@$(RM) $(NAME)
	@$(LOG) "$(C_RM)" removed "$(NAME)"

re:			fclean all

.PHONY:		all clean fclean re
