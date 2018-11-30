NAME= mainJeuxTest
#NAME= mainEvalTemps

SRC=	entree_sortie.c \
	structure.c \
	fonction.c \
	fonctionFile.c \
	structure_AVL.c \
	graphe.c \
	resolution.c \
	mainJeuxTest.c \
#	mainEvalTemps.c \


OBJ=	$(SRC:.c=.o)

RM=	rm -vf

CFLAGS +=	-Wall -Wextra -Werror -g

all:	$(NAME)

$(NAME): $(OBJ)
	gcc -g -o $(NAME) $(OBJ)

clear:
	$(RM) $(OBJ)

clearall: clear
	$(RM) $(NAME)

recompile: clearall all
