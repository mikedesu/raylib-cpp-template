CC=g++

all: game


game: main.o
	$(CC) $^ -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -o $@


main.o: main.cpp
	$(CC) $^ -c -o $@


clean:
	rm -f *.o game
