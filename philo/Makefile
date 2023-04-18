# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: FelipeBelfort <FelipeBelfort@student.42    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/04/12 22:58:20 by FelipeBelfo       #+#    #+#              #
#    Updated: 2023/04/13 14:13:56 by FelipeBelfo      ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME			=	philo

GCC				=	cc

# /usr/lib/itw-bash -p
# CFLAGS			=	-Wall -Wextra -Werror -O3 -pthread
CFLAGS			=	-Wall -Wextra -Werror -O3 -pthread -g3 -fsanitize=thread -D TSAN_OPTIONS=second_deadlock_stack=1

SRC				=	main.c \
					t_philo_management.c \
					utils.c \
					routine.c

OBJS_DIR		=	objs/

OBJ				=	$(addprefix $(OBJS_DIR), $(SRC:.c=.o))

RM				=	rm -fr

HEADER			=	$(NAME).h

$(OBJS_DIR)%.o	: %.c $(HEADER)
				$(GCC) $(CFLAGS) -c $< -o $@

all				:	$(NAME)

$(NAME)			: $(OBJS_DIR) $(OBJ) Makefile
			$(GCC) $(CFLAGS) -o $(NAME) $(OBJ)

$(OBJS_DIR)		:
			mkdir -p $(OBJS_DIR)

clean			:
			$(RM) $(OBJS_DIR)

fclean			:	clean
			$(RM) $(NAME)

re				:	fclean all

.PHONY			:	all clean fclean re
