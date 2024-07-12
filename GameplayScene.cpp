#include "GameplayScene.h"
#include "Sprite.h"
#include "mPrint.h"

static entity_id next_entity_id = 0;

GameplayScene::GameplayScene() {
  mPrint("GameplayScene constructor");
  set_control_mode(CONTROL_MODE_PLAYER);
  set_texture_filepath("game_textures.txt");
  set_global_scale(4.0f);
  set_scene_transition(SCENE_TRANSITION_IN);
  set_scene_type(SCENE_TYPE_GAMEPLAY);
}

GameplayScene::~GameplayScene() { mPrint("GameplayScene destructor"); }

void GameplayScene::update_player_movement() {
  shared_ptr<Sprite> player = get_sprites()[get_player_id()];
  player->incr_ay(0.0032f);
  player->update();
  player->set_y(player->get_y() + player->get_vy());
  const int height = player->get_height();
  const int width = player->get_width();
  const int bottom_of_sprite = player->get_y() + height;
  // lock the player to a relative "ground" location
  if (bottom_of_sprite >= GetScreenHeight()) {
    player->set_y(GetScreenHeight() - height);
  }
  // prevent player from moving off-screen
  if (player->get_x() < 0) {
    player->set_x(0);
  }
  if (player->get_x() > GetScreenWidth() - width) {
    player->set_x(GetScreenWidth() - width);
  }
}

void GameplayScene::update_enemy_movement() {
  for (auto &s : get_sprites()) {
    switch (s.second->get_type()) {
    case SPRITETYPE_ENEMY:
      s.second->update();
      s.second->set_x(s.second->get_x() + s.second->get_vx());
      break;
    }
  }
}

void GameplayScene::update_knife_movement() {
  for (auto &s : get_sprites()) {
    switch (s.second->get_type()) {
    case SPRITETYPE_KNIFE:
      s.second->update();
      s.second->set_x(s.second->get_x() + s.second->get_vx());
      break;
    }
  }
}

void GameplayScene::handle_offscreen() {
  for (auto &s : get_sprites()) {
    const int width = s.second->get_width() * 2;
    // if sprite moves beyond screen, mark for deletion
    // we use 2*width to help create a buffer zone
    // so we can spawn new entities within the buffer
    // while allowing old entities to move outside of it
    // and get marked for deletion
    if (s.second->get_x() > GetScreenWidth() + width ||
        s.second->get_x() < -width) {
      s.second->mark_for_deletion();
    }
  }
}

void GameplayScene::handle_player_collision() {
  shared_ptr<Sprite> player = get_sprites()[get_player_id()];
  for (auto &s : get_sprites()) {
    sprite_type t = s.second->get_type();
    switch (t) {
    case SPRITETYPE_ENEMY:
    case SPRITETYPE_KNIFE:
      // if (CheckCollisionRecs(player->get_dest(), s.second->get_dest())) {
      // if (CheckCollisionRecs(player->get_hitbox(), s.second->get_hitbox())) {
      if (CheckCollisionRecs(player->get_dest(), s.second->get_dest())) {
        s.second->mark_for_deletion();

        if (t == SPRITETYPE_KNIFE) {
          set_knife_catches(get_knife_catches() + 1);
        } else {
        }
      }
      break;
    default:
      break;
    }
  }
}

void GameplayScene::handle_knife_collisions() {
  for (auto &knife : get_sprites()) {
    sprite_type t = knife.second->get_type();
    switch (t) {
    case SPRITETYPE_KNIFE: {
      for (auto &s : get_sprites()) {
        sprite_type t2 = s.second->get_type();
        switch (t2) {
        case SPRITETYPE_ENEMY: {
          // if (CheckCollisionRecs(knife.second->get_dest(),
          // s.second->get_dest())) {
          if (CheckCollisionRecs(knife.second->get_dest(),
                                 s.second->get_dest())) {

            s.second->mark_for_deletion();
            knife.second->mark_for_deletion();
          }
          break;
        }
        }
      }
      break;
    }
    }
  }
}

void GameplayScene::update() {
  const float star_move_speed = -1.0f;

  if (!get_paused()) {

    update_stars_vx(star_move_speed);

    update_player_movement();
    update_enemy_movement();
    update_knife_movement();

    handle_offscreen();
    handle_player_collision();
    handle_knife_collisions();

    // we want the camera to follow the player in such a way that
    // the player is always in the center of the screen
    // except for the game beginning, when they begin flapping at the bottom
    // once they hit the center, the camera should follow them
    Camera2D &camera2d = get_camera2d();
    camera2d.target.y = get_sprites()[get_player_id()]->get_y();
    // camera2d.offset.y = GetScreenHeight() / 2.0f;
  }
}

void GameplayScene::handle_input() {
  shared_ptr<Sprite> player = get_sprites()[get_player_id()];
  // this value affects how high skull 'flaps' or jumps
  const float vy = -6.75f;
  const float ay = 0.0f;
  const float move_speed = 2.0f;

  if (IsKeyPressed(KEY_D)) {
    flip_debug_panel();
  }

  if (!get_paused()) {

    if (IsKeyPressed(KEY_C)) {
      set_control_mode(CONTROL_MODE_CAMERA);
    }

    if (IsKeyPressed(KEY_B)) {
      const int bat_width = get_textures()["bat"].texture.width;
      const int bat_height = get_textures()["bat"].texture.height;
      const float bat_x = -bat_width;
      const float bat_y =
          (float)GetScreenHeight() / 2 - (float)bat_height + 300;
      spawn_bat(bat_x, bat_y);
    }

    if (IsKeyPressed(KEY_Z)) {
      // fire a knife
      spawn_knife();
    }

    if (IsKeyPressed(KEY_SPACE)) {
      player->set_ay(ay);
      player->set_vy(vy);
      player->update();
    }

    bool shift = IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT);
    int new_move_speed = move_speed;
    if (shift) {
      new_move_speed = move_speed * 2;
    }

    if (IsKeyDown(KEY_LEFT)) {
      player->set_x(player->get_x() - new_move_speed);
      if (!player->get_is_flipped()) {
        player->flip();
      }
    } else if (IsKeyDown(KEY_RIGHT)) {

      player->set_x(player->get_x() + new_move_speed);
      if (player->get_is_flipped()) {
        player->flip();
      }
    }

    if (IsKeyPressed(KEY_P)) {
      pause();
    }

  } else {
    if (IsKeyPressed(KEY_P)) {
      unpause();
    }
  }
}

bool GameplayScene::init() {
  if (!get_has_been_initialized()) {
    mPrint("Initializing scene...");
    mPrint("Initializing camera...");
    set_camera_default_values();
    mPrint("Loading assets...");
    load_fonts();
    bool result = load_textures();
    if (!result) {
      mPrint("Error loading textures. Exiting...");
      return false;
    }

    const int sprite_width = get_textures()["skull"].texture.width;
    const int sprite_height = get_textures()["skull"].texture.height;
    const float x = (float)GetScreenWidth() / 2 - (float)sprite_width;
    const float y = (float)GetScreenHeight() / 2 - (float)sprite_height;
    spawn_player(x, y);

    for (int i = 0; i < 1000; i++) {
      add_star();
    }

    const int bat_width = get_textures()["bat"].texture.width;
    const int bat_height = get_textures()["bat"].texture.height;
    const float bat_x = -bat_width;
    const float bat_y = (float)GetScreenHeight() / 2 - (float)bat_height + 300;
    spawn_bat(bat_x, bat_y);

    get_camera2d().offset.y = GetScreenHeight() / 2.0f;

    set_has_been_initialized(true);
  }
  return true;
}

void GameplayScene::draw_debug_panel() {
  string camera_info_str =
      "Current Frame: " + to_string(get_current_frame()) + "\n" +
      "Control mode: " + to_string(get_control_mode()) + "\n" +
      "Player Position: " + to_string(get_sprites()[get_player_id()]->get_x()) +
      ", " + to_string(get_sprites()[get_player_id()]->get_y()) + "\n" +
      "Player HP: " + to_string(get_sprites()[get_player_id()]->get_hp()) +
      "/" + to_string(get_sprites()[get_player_id()]->get_maxhp()) + "\n" +
      "Camera target: " + to_string(get_camera2d().target.x) + ", " +
      to_string(get_camera2d().target.y) + "\n" + "GameplayScene" +
      "Sprites: " + to_string(get_sprites().size()) + "\n" +
      "Knife Catches: " + to_string(get_knife_catches()) + "\n" +
      "IsPaused: " + to_string(get_paused()) + "\n";
  DrawRectangle(0, 0, 500, 200, Fade(BLACK, 0.5f));
  DrawTextEx(get_global_font(), camera_info_str.c_str(), (Vector2){10, 10}, 16,
             0.5f, WHITE);
}

void GameplayScene::cleanup() {
  // mPrint("cleanup");
  for (int i = 0; i < get_entity_ids().size(); i++) {
    entity_id id = get_entity_ids()[i];
    if (get_sprites()[id]->get_is_marked_for_deletion()) {
      get_sprites().erase(id);
      // also need to erase from entity_ids
      get_entity_ids().erase(get_entity_ids().begin() + i);
      // update the index so that we don't skip any
      i--;
    }
  }
}
