#Set compiler
CC = g++

#All objects
OBJ = main.o Condition.o Container.o Creature.o Item.o Room.o Trigger.o

%.o:%.c
	$(CC) $< -o $@

all:game

game: $(OBJ)
	$(CC) -o game $^

# Clean all objects and game
clean:
	rm *.o game

