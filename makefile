NAME = pomodoro_timer

SRC = src/pomodoro.c \
	  src/timer.c \
	  src/preferences.c

OBJ = $(SRC:.c=.o) 

INCLUDE = -I include 
LIBRARIES = -lncurses -lpthread

CFLAGS = -W -Wall -Wextra $(INCLUDE)

all: $(NAME)

$(NAME):	$(OBJ)
			gcc -o $(NAME) $(INCLUDE) $(OBJ) $(LIBRARIES)

clean: 
	rm $(NAME) $(OBJ)

re: clean all