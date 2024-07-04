#include "Game.h"

int main(void) {
  Game game;

  game.set_window_title("Game");
  game.set_camera_default_values();
  game.set_debug_panel(true);

  /*
  game.add_collision_func([&](entity_id id) {
    entity_id player_id = game.get_player_id();
    if (id == player_id) {
      return;
    }
    shared_ptr<Sprite> s = game.get_sprite(id);
    if (s == nullptr) {
      return;
    }
    // check if player collides with other sprites
    if (CheckCollisionRecs(game.get_sprite(player_id)->get_dest(),
                           s->get_dest())) {
      // if the sprite is the player, we don't want to delete it
      s->mark_for_deletion();
      return;
    }
  });
  game.add_collision_func([&](entity_id id) {
    shared_ptr<Sprite> s = game.get_sprite(id);
    if (s == nullptr) {
      return;
    }
    sprite_type type = s->get_type();
    if (type != SPRITETYPE_KNIFE) {
      return;
    }
    s->move_pos_x(s->get_vx());
    s->move_pos_y(s->get_vy());

    for (auto id2 : game.get_entity_ids()) {
      shared_ptr<Sprite> s2 = game.get_sprite(id2);
      sprite_type type2 = s2->get_type();
      if (type2 != SPRITETYPE_ENEMY) {
        continue;
      }
      if (CheckCollisionRecs(s->get_dest(), s2->get_dest())) {
        s->mark_for_deletion();
        s2->mark_for_deletion();
        continue;
      }
    }
    // check if offscreen using collision rects
    Rectangle right_side = {(float)GetScreenWidth() + s->get_width(), 0, 1,
                            (float)GetScreenHeight()};
    Rectangle left_side = {(float)-s->get_width(), 0, 1,
                           (float)GetScreenHeight()};
    Rectangle top_side = {0, (float)-s->get_height(), (float)GetScreenWidth(),
                          1};
    Rectangle bottom_side = {0, (float)GetScreenHeight() + s->get_height(),
                             (float)GetScreenWidth(), 1};
    if (CheckCollisionRecs(s->get_dest(), right_side) ||
        CheckCollisionRecs(s->get_dest(), left_side) ||
        CheckCollisionRecs(s->get_dest(), top_side) ||
        CheckCollisionRecs(s->get_dest(), bottom_side)) {
      s->mark_for_deletion();
      return;
    }
  });
  */

  game.init();
  game.run();

  return 0;
}
