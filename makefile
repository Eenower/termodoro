
NAME = termodoro
SRC = src/pomodoro.c \
	  src/timer.c \
	  src/preferences.c

OUT = out/
OBJ = $(addprefix $(OUT), $(notdir $(SRC:.c=.o)))

INCLUDE = -I include 
LIBRARIES = -lncurses -lpthread

CFLAGS = -W -Wall -Wextra $(INCLUDE)

all: $(NAME)

$(NAME): $(OBJ)
	gcc -o $(NAME) $(OBJ) $(LIBRARIES)

$(OUT)%.o: src/%.c
	gcc $(CFLAGS) -c $< -o $@

clean:
	rm -f $(NAME) $(OBJ)

re: clean all
