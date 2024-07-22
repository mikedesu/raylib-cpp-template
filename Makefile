CC=g++
OBJ_FILES=main.o Game.o Sprite.o Scene.o TitleScene.o GameplayScene.o GameoverScene.o PopupManager.o

FLAGS=-Wall

LIBS=-lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -lSDL2 -lSDL2_mixer

all: game


game: $(OBJ_FILES)
	$(CC) $^ $(FLAGS) $(LIBS) -o $@

Game.o: Game.cpp
	$(CC) $^ $(FLAGS) -c -o $@

TitleScene.o: TitleScene.cpp
	$(CC) $^ $(FLAGS) -c -o $@

GameoverScene.o: GameoverScene.cpp
	$(CC) $^ $(FLAGS) -c -o $@

GameplayScene.o: GameplayScene.cpp
	$(CC) $^ $(FLAGS) -c -o $@

Scene.o: Scene.cpp
	$(CC) $^ $(FLAGS) -c -o $@

Sprite.o: Sprite.cpp
	$(CC) $^ $(FLAGS) -c -o $@

PopupManager.o: PopupManager.cpp
	$(CC) $^ $(FLAGS) -c -o $@

main.o: main.cpp
	$(CC) $^ $(FLAGS) -c -o $@


clean:
	rm -f *.o game 
