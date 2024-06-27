#include "Game.h"

int main(void) {
  Game game;

  game.set_screen_width(1280);
  game.set_screen_height(760);
  game.set_window_title("Game");
  game.set_camera_default_values();
  game.set_debug_panel(true);
  game.set_global_scale(4.0f);

  game.add_texture_to_load("skull", "../ecs/img/skull-sheet.png", 2, 1);
  game.add_texture_to_load("knife", "../ecs/img/knife1.png", 1, 0);
  game.add_texture_to_load("bat", "../ecs/img/bat-sheet.png", 2, 0);

  game.init();

  game.run();

  return 0;
}
