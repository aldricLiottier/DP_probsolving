SRC 	= 	main.cpp \
			Algorithm/Algorithm.cpp

OBJ		=	$(SRC:.cpp=.o)

NAME 	=	ai

RM 		=	rm -f

RM_TRASH=	*.o *~ *.gcno *.gcda libmy.a *.gch *.out *.data

# CFLAGS	= 	-lsfml-graphics -lsfml-audio -lsfml-window -lsfml-system -Wall -Wextra -W -g -I ./include
# -lsfml-graphics -lsfml-audio -lsfml-window -lsfml-system
# -O3 -pthread
CFLAGS	=  -g 

all:	$(NAME)

$(NAME):	$(OBJ)
	g++ $(OBJ) -o $(NAME) $(CFLAGS)

clean:
	$(RM) $(RM_TRASH) $(OBJ)

fclean: clean
	$(RM) $(NAME)

re: fclean all

clear: clean fclean