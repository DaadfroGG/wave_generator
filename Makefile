NAME 			= wave

CC 				= gcc -L ./lib/ -lraylib -lm -fopenmp -g3 #-O3 # -fsanitize=address

LIBS 			= -L. -L./lib  -lm -fopenmp

OBJ				= $(SRCS:.c=.o)

DEP				= $(SRCS:.c=.d)

SRCS 			= main.c 

SRCH_INCLDS 	= -Iinclude

RM				= rm -rf

all:        $(NAME)

%.o: %.c
			$(CC) $(SRCH_INCLDS) -c $< -o $@

$(NAME):	$(OBJ)
			$(CC) $(OBJ) -lraylib -lm -fopenmp -o $(NAME)

clean:
			$(RM) $(OBJ)
			$(RM) $(DEP)

web:		clean
			emcc -o game.html $(SRCS) -Os -Wall ./lib/libraylib_web.a -I. -I./include $(LIBS) -s USE_GLFW=3  -DPLATFORM_WEB

fclean:     clean
			$(RM) $(NAME)

re:         fclean $(NAME)