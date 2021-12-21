NAME = ft_containers

SRC = main.cpp

OBJ = main.o

FLAGS = --std=c++98 -Wall -Wextra -Werror

all: $(NAME)

$(NAME): $(SRC)
	clang++ ${FLAGS} $(SRC)  -o $(NAME)

clean:
	@rm -rf ${OBJ}

fclean: clean
	@rm -f $(NAME)

re: fclean all