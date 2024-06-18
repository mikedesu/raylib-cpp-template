#include "Game.h"
#include "raylib.h"

int main(void) {
  const int screenWidth = 1280;
  const int screenHeight = 720;
  const char *title = "test";

  InitWindow(screenWidth, screenHeight, title);

  Game game;
  SetExitKey(KEY_Q);
  SetTargetFPS(60); // Set our game to run at 60 frames-per-second
  game.run();

  CloseWindow();
  return 0;
}
