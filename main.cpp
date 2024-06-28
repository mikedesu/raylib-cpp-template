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

  game.set_player_texture_key("skull");

  game.set_collision_func([&](entity_id id) {
    entity_id player_id = game.get_player_id();
    if (id == player_id) {
      return;
    }
    shared_ptr<Sprite> s = game.get_sprite(id);
    // check if player collides with other sprites
    if (CheckCollisionRecs(game.get_sprite(player_id)->get_dest(),
                           s->get_dest())) {
      // if the sprite is the player, we don't want to delete it
      s->mark_for_deletion();
    }
  });

  game.init();
  game.run();

  return 0;
}
