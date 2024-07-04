CC=g++
OBJ_FILES=main.o Game.o Sprite.o Scene.o

all: game


game: $(OBJ_FILES)
	$(CC) $^ -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -o $@

Game.o: Game.cpp
	$(CC) $^ -c -o $@

Scene.o: Scene.cpp
	$(CC) $^ -c -o $@

Sprite.o: Sprite.cpp
	$(CC) $^ -c -o $@

main.o: main.cpp
	$(CC) $^ -c -o $@


clean:
	rm -f *.o game 
