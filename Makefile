CPP_FLAGS = -ggdb -Wall -Werror -lncurses

CPP_OBJS = \
core.o \
monster.o \
player.o \
dijkstra.o \
heap.o \
io_main.o \
game.o \
monster_desc.o \
dice.o \
object_description.o \
object.o \

NAME = rungame

all: $(NAME)

$(NAME): $(CPP_OBJS)
	g++ $(CPP_OBJS) $(CPP_FLAGS) -o $(NAME)

%.o: %.cpp
	g++ $(CPP_FLAGS) -c $<

clean:
	rm -f $(NAME) *.o *~ dungeon*




