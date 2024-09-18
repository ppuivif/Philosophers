#CC = cc
#CC = gcc -fsanitize=thread
#CC = gcc -fsanitize=address
CC = gcc -pg

CFLAGS = -Wall -Werror -Wextra -g -Iincludes

LDFLAGS = -lpthread

NAME  = philo

SOURCES = 	sources/philo.c \
			sources/parsing.c \
			sources/mutex_init.c \
			sources/structs_init.c \
			sources/get_time.c \
			sources/print.c \
			sources/error_handling.c \
			sources/structs_free.c \
			sources/mutex_destroy.c \
			sources/utils1.c \

SOURCES += tests/tests.c

OBJECTS = $(SOURCES:.c=.o)

all : $(NAME)

$(NAME) : $(OBJECTS)
		$(CC) $(CFLAGS) $(OBJECTS) -o $@ $(LDFLAGS)
		@clear

%.o : %.c
		$(CC) $(CFLAGS) -c $< -o $@

clean :
		rm -rf $(OBJECTS)

fclean : clean
		rm -rf $(NAME)

re : fclean all

.PHONY: all clean fclean re