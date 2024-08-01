#include "GameplayScene.h"
#include "Sprite.h"
#include "mPrint.h"

// static entity_id next_entity_id = 0;

GameplayScene::GameplayScene() {
  mPrint("GameplayScene constructor");
  set_control_mode(CONTROL_MODE_PLAYER);
  set_texture_filepath("game_textures.txt");
  set_global_scale(4.0f);
  // set_global_scale(4.0f);
  set_scene_transition(SCENE_TRANSITION_IN);
  set_scene_type(SCENE_TYPE_GAMEPLAY);

  // load_music("/home/darkmage/Music/darkmage/lets-fkn-go.mp3");
}

GameplayScene::~GameplayScene() { mPrint("GameplayScene destructor"); }

void GameplayScene::gameover() { set_scene_transition(SCENE_TRANSITION_OUT); }

void GameplayScene::update_player_movement() {
  shared_ptr<Sprite> player = get_sprites()[player_id];

  // this handles the 'flappy bird' bounce
  // player->incr_ay(gravity);
  // player->set_y(player->get_y() + player->get_vy());
  // player->update();

  const int height = player->get_height();
  const int width = player->get_width();
  const int bottom_of_sprite = player->get_y() + height;
  // lock the player to a relative "ground" location

  const int beginning_of_lava = ground_y;
  if (bottom_of_sprite >= beginning_of_lava) {
    gameover();
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
    case SPRITETYPE_ENEMY: {
      s.second->update();

      // if (s.second->get_movement_type() == movement_type::MOVEMENT_TYPE_NONE)
      // {
      // }
      if (s.second->get_movement_type() ==
          movement_type::MOVEMENT_TYPE_NORMAL) {
        s.second->set_x(s.second->get_x() + s.second->get_vx());
        s.second->set_y(s.second->get_y() + s.second->get_vy());
      } else if (s.second->get_movement_type() ==
                 movement_type::MOVEMENT_TYPE_HOMING) {

        // we need to calculate the angle between the enemy and the player
        // and then move the enemy towards the player
        // we can use the atan2 function to calculate the angle
        // we can then use the angle to calculate the x and y components of the
        // velocity
        //

        // get the player position
        const float player_x = get_sprites()[player_id]->get_x();
        const float player_y = get_sprites()[player_id]->get_y();

        // get the enemy position
        const float enemy_x = s.second->get_x();
        const float enemy_y = s.second->get_y();

        // calculate the angle
        const float angle = atan2(player_y - enemy_y, player_x - enemy_x);

        // calculate the x and y components of the velocity
        float vx = cos(angle) * s.second->get_vx();
        float vy = sin(angle) * s.second->get_vy();

        // there may be a bug in the movement if the bat has just spawned and is
        // not fully on-screen yet we can fix this by checking if the bat is
        // off-screen and then moving it on-screen
        if (enemy_x < 0) {
          // the bat is to the left of the player so we need to make sure vx is
          // positive
          vx = 4.0;

        } else if (enemy_x > GetScreenWidth()) {
          // the bat is to the right of the player so we need to make sure vx is
          // negative
          vx = -4.0;
        }

        // update the enemy position
        s.second->set_x(s.second->get_x() + vx);
        s.second->set_y(s.second->get_y() + vy);
      }

      const int height = s.second->get_height();
      // const int width = s.second->get_width();
      const int bottom_of_sprite = s.second->get_y() + height;
      // lock the s.second to a relative "ground" location
      const int beginning_of_lava = ground_y;
      if (bottom_of_sprite >= beginning_of_lava) {
        s.second->mark_for_deletion();
      }
    } break;

    default:
      break;
    }
  }
}

void GameplayScene::update_knife_movement() {
  for (auto &s : get_sprites()) {
    switch (s.second->get_type()) {
    case SPRITETYPE_KNIFE: {
      s.second->update();
      s.second->set_x(s.second->get_x() + s.second->get_vx());

      const int height = s.second->get_height();
      // const int width = s.second->get_width();
      const int bottom_of_sprite = s.second->get_y() + height;
      // lock the s.second to a relative "ground" location
      const int beginning_of_lava = ground_y;
      if (bottom_of_sprite >= beginning_of_lava) {
        s.second->mark_for_deletion();
      }
    }

    break;
    default:
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

    const bool condition_0 = s.second->get_x() > GetScreenWidth() + width;
    const bool condition_1 = s.second->get_x() < -width;

    if (condition_0) {
      cout << "Condition 0" << endl;
      s.second->mark_for_deletion();
    } else if (condition_1) {
      cout << "Condition 1" << endl;
      s.second->mark_for_deletion();
    }
  }
}

void GameplayScene::handle_player_collision() {
  shared_ptr<Sprite> player = get_sprites()[player_id];
  for (auto &s : get_sprites()) {
    sprite_type t = s.second->get_type();
    switch (t) {
    case SPRITETYPE_ENEMY:
    case SPRITETYPE_KNIFE:
    case SPRITETYPE_SOULSHARD:
      // if (CheckCollisionRecs(player->get_dest(), s.second->get_dest())) {
      // if (CheckCollisionRecs(player->get_hitbox(), s.second->get_hitbox())) {
      if (CheckCollisionRecs(player->get_dest(), s.second->get_dest())) {

        if (t == SPRITETYPE_KNIFE) {
          s.second->mark_for_deletion();
          knife_catches = (knife_catches + 1);
          get_popup_manager()->render("knife catch " +
                                      to_string(knife_catches));
        } else if (t == SPRITETYPE_ENEMY) {
          s.second->mark_for_deletion();
          player->set_hp(player->get_hp() - 1);
          enemies_killed++;
          get_popup_manager()->render("enemies killed " +
                                      to_string(enemies_killed));

          if (player->get_hp() <= 0) {
            set_scene_transition(SCENE_TRANSITION_OUT);
          }
        } else if (t == SPRITETYPE_SOULSHARD) {

          soulshard_catches++;
          s.second->mark_for_deletion();
          // player->set_hp(player->get_hp() + 1);
          get_popup_manager()->render("soulshard catch " +
                                      to_string(soulshard_catches));
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
          if (CheckCollisionRecs(knife.second->get_dest(),
                                 s.second->get_dest())) {

            s.second->set_hp(s.second->get_hp() - 1);
            if (s.second->get_hp() <= 0) {
              s.second->mark_for_deletion();
              knife.second->mark_for_deletion();

              // Mix_PlayChannel(-1, sfx_knife_hit, 0);
              enemies_killed++;

              get_popup_manager()->render("enemies killed " +
                                          to_string(enemies_killed));

              spawn_soulshard(s.second->get_x(), s.second->get_y());
            }
          }
          break;
        }
        default:
          break;
        }
      }
      break;
    }
    default:
      break;
    }
  }
}

void GameplayScene::update() {
  const float star_move_speed = -1.0f;

  if (!get_paused()) {

    update_stars_vx(star_move_speed);

    handle_offscreen();
    handle_player_collision();
    handle_knife_collisions();

    update_player_movement();
    update_enemy_movement();
    update_knife_movement();

    // we want the camera to follow the player in such a way that
    // the player is always in the center of the screen
    // except for the game beginning, when they begin flapping at the bottom
    // once they hit the center, the camera should follow them
    Camera2D &camera2d = get_camera2d();
    camera2d.target.y = get_sprites()[player_id]->get_y();
    // camera2d.offset.y = GetScreenHeight() / 2.0f;

    if (do_ground_movement) {
      ground_y += ground_y_movement;
    }

    if (get_current_frame() % 120 == 0) {
      spawn_bat();
    }
  }
}

void GameplayScene::handle_input() {
  shared_ptr<Sprite> player = get_sprites()[player_id];
  // this value affects how high skull 'flaps' or jumps
  const float vy = -6.75f;
  const float ay = 0.0f;
  const float move_speed = 2.0f;

  if (IsKeyPressed(KEY_D)) {
    flip_debug_panel();
  }

  if (IsKeyPressed(KEY_F)) {
    ToggleFullscreen();
  }

  if (!get_paused()) {

    if (IsKeyPressed(KEY_C)) {
      set_control_mode(CONTROL_MODE_CAMERA);
    }

    // if (IsKeyPressed(KEY_I)) {
    // if (IsKeyDown(KEY_I)) {
    //  ground_y -= 1;
    //}

    // if (IsKeyPressed(KEY_B)) {
    //   do_ground_movement = !do_ground_movement;
    // }

    if (IsKeyPressed(KEY_P)) {
      // const int bat_width = get_textures()["bat"].texture.width;
      // const int bat_height = get_textures()["bat"].texture.height;
      // const float bat_x = -bat_width;
      // const float bat_y =
      //     (float)GetScreenHeight() / 2 - (float)bat_height + 300;
      // spawn_bat(bat_x, bat_y);
      // spawn_bat();
      // if (get_popup_manager() != nullptr) {
      //  get_popup_manager()->print("bat");
      //}

      show_test_popup = !show_test_popup;
    }

    if (IsKeyPressed(KEY_R)) {
      static int count = 0;
      get_popup_manager()->render("test " + to_string(count++));
    }

    // if (IsKeyPressed(KEY_P)) {
    //   const int pipebase_width = get_textures()["pipebase"].texture.width;
    //   const int pipebase_height = get_textures()["pipebase"].texture.height;
    //   const float pipebase_x = -pipebase_width;
    //   const float pipebase_y = GetScreenHeight() - pipebase_height * 4;
    //   spawn_pipebase(pipebase_x, pipebase_y);
    // }

    if (IsKeyPressed(KEY_Z)) {
      // incr the player sprite frame
      // get_sprites()[player_id]->incr_frame();
      get_sprite(player_id)->incr_frame();

      // fire a knife
      spawn_knife();
    }

    if (IsKeyReleased(KEY_Z)) {
      // reset the player sprite frame
      // get_sprites()[player_id]->set_frame(0);
      get_sprite(player_id)->incr_frame();
    }

    if (IsKeyPressed(KEY_X)) {
      spawn_bat();
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

    if (IsKeyDown(KEY_UP)) {
      player->set_y(player->get_y() - new_move_speed);
    } else if (IsKeyDown(KEY_DOWN)) {
      player->set_y(player->get_y() + new_move_speed);
    }

    if (IsKeyPressed(KEY_ESCAPE)) {
      pause();
    }

  } else {
    if (IsKeyPressed(KEY_ESCAPE)) {
      unpause();
    }
  }
}

bool GameplayScene::init() {
  if (!get_has_been_initialized()) {
    mPrint("Initializing gameplay scene...");
    mPrint("Initializing camera...");
    set_camera_default_values();
    mPrint("Loading assets...");
    load_fonts();
    bool result = load_textures();
    if (!result) {
      mPrint("Error loading textures. Exiting...");
      return false;
    }
    mPrint("Spawning player...");

    const int sprite_width = get_textures()["skull"].texture.width;
    const int sprite_height = get_textures()["skull"].texture.height;
    const float x = (float)GetScreenWidth() / 2 - (float)sprite_width;
    const float y = (float)GetScreenHeight() / 2 - (float)sprite_height;
    spawn_player(x, y);

    for (int i = 0; i < 1000; i++) {
      add_star();
    }

    get_camera2d().offset.y = GetScreenHeight() / 2.0f;

    // load_music("audio/skull-title-0.mp3");
    // Mix_PlayMusic(get_music(), -1);
    mPrint("Loading sound effects...");

    sfx_knife_throw = Mix_LoadWAV("audio/knife-throw.mp3");
    sfx_knife_hit = Mix_LoadWAV("audio/knife-hit.mp3");

    if (sfx_knife_throw == nullptr || sfx_knife_hit == nullptr) {
      mPrint("Error loading sound effects. Exiting...");
      return false;
    }

    ground_y = GetScreenHeight();

    set_has_been_initialized(true);

    mPrint("GameplayScene initialized");
  }
  return true;
}

void GameplayScene::draw_debug_panel() {
  string camera_info_str =
      "Current Frame: " + to_string(get_current_frame()) + "\n" +
      "Control mode: " + to_string(get_control_mode()) + "\n" +
      "Player Position: " + to_string(get_sprites()[player_id]->get_x()) +
      ", " + to_string(get_sprites()[player_id]->get_y()) + "\n" +
      "Player Velocity: " + to_string(get_sprites()[player_id]->get_vx()) +
      ", " + to_string(get_sprites()[player_id]->get_vy()) + "\n" +
      "Player Acceleration: " + to_string(get_sprites()[player_id]->get_ax()) +
      ", " + to_string(get_sprites()[player_id]->get_ay()) + "\n" +
      "Player HP: " + to_string(get_sprites()[player_id]->get_hp()) + "/" +
      to_string(get_sprites()[player_id]->get_maxhp()) + "\n" +
      "Camera target: " + to_string(get_camera2d().target.x) + ", " +
      to_string(get_camera2d().target.y) + "\n" + "GameplayScene" +
      "Sprites: " + to_string(get_sprites().size()) + "\n" +
      "Knife Catches: " + to_string(knife_catches) + "\n" +
      "IsPaused: " + to_string(get_paused()) + "\n" +
      "Enemies Killed: " + to_string(enemies_killed) + "\n" +
      "Soulshard Catches: " + to_string(soulshard_catches) + "\n";
  DrawRectangle(0, 0, 500, 200, Fade(BLACK, 0.5f));
  DrawTextEx(get_global_font(), camera_info_str.c_str(), (Vector2){10, 10}, 16,
             0.5f, WHITE);
}

void GameplayScene::draw_hud() {
  const int player_hp = get_sprites()[player_id]->get_hp();
  const int player_maxhp = get_sprites()[player_id]->get_maxhp();
  const string hp_str =
      "HP: " + to_string(player_hp) + "/" + to_string(player_maxhp);

  // const int knife_catches = knife_catches;
  const string knife_catches_str = "Knife Catches: " + to_string(knife_catches);
  const string enemies_killed_str = "Killed: " + to_string(enemies_killed);

  const string soulshard_catches_str =
      "Soulshards: " + to_string(soulshard_catches);

  const string full_str = hp_str + " " + knife_catches_str + " " +
                          enemies_killed_str + " " + soulshard_catches_str +
                          "\nPress Z to throw knife";

  // DrawRectangle(0, 0, 500, 20, Fade(BLACK, 0.5f));
  // DrawTextEx(get_global_font(), full_str.c_str(), (Vector2){10, 10}, 20,
  // 0.5f,
  //           WHITE);
  DrawText(full_str.c_str(), 10, 10, 20, WHITE);
}

void GameplayScene::cleanup() {
  // mPrint("cleanup");
  for (int i = 0; i < (int)get_entity_ids().size(); i++) {
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

void GameplayScene::draw() {
  BeginMode2D(get_camera2d());
  Color clear_color = (Color){0x10, 0x10, 0x10, 0xFF};
  ClearBackground(clear_color);
  // no background yet, but lets mock one up with shapes
  // draw a large rectangle to represent a scene
  // but lets make the dimension ratio 720x1280
  // DrawRectangle(GetScreenWidth() / 2 - 405 / 2, 0, 405, 720, BLACK);
  // draw stars
  // want: real stars
  if (get_scene_type() == SCENE_TYPE_GAMEPLAY) {
    draw_stars();
    draw_ground();
  }

  for (auto &s : get_sprites()) {
    s.second->draw();
    if (get_debug_panel_on()) {
      s.second->draw_hitbox();

      // draw a line from the sprite to the player
      if (s.second->get_type() == SPRITETYPE_ENEMY) {
        DrawLine(s.second->get_x(), s.second->get_y(),
                 get_sprites()[player_id]->get_x(),
                 get_sprites()[player_id]->get_y(), RED);
      }
    }
  }

  // if (get_scene_type() == SCENE_TYPE_TITLE) {
  //   const float x =
  //       GetScreenWidth() / 2.0f - get_textures()["title"].texture.width
  //       / 2.0f;
  //
  //    const float y0 = GetScreenHeight() / 4.0f - 32.0f;
  //    const float y1 = GetScreenHeight() * 3.0f / 4.0f;
  //
  //    DrawTextEx(get_global_font(), "@evildojo666 presents", (Vector2){x, y0},
  //    32,
  //               0.5f, WHITE);
  //    DrawTextEx(get_global_font(), "by darkmage", (Vector2){x, y1}, 32, 0.5f,
  //               WHITE);
  //  }

  EndMode2D();

  // draw debug panel
  // if (get_debug_panel_on()) {
  //  DrawFPS(GetScreenWidth() - 80, 10);
  //  draw_debug_panel();
  //} else if (get_hud_on()) {
  //  draw_hud();
  //}

  handle_draw_debug_panel();

  if (show_test_popup) {
    if (get_popup_manager() != nullptr) {

      // const float x = GetScreenWidth() / 2.0f - 100.0f;
      // const float y = GetScreenHeight() / 2.0f - 100.0f;
      //  get player x
      const float x = get_sprite(player_id)->get_x();

      // get player y
      const float y = get_sprite(player_id)->get_y();

      Vector2 s = GetWorldToScreen2D(
          (Vector2){x - 200, y - 100},
          get_camera2d()); // Get the screen space position for
                           // a 2d camera world space position

      get_popup_manager()->draw(s.x, s.y);
    }
  }

  // update music frame
  // current_frame++;
  incr_current_frame();
}

inline void GameplayScene::handle_draw_debug_panel() {
  if (get_debug_panel_on()) {
    DrawFPS(GetScreenWidth() - 80, 10);
    draw_debug_panel();
  } else if (get_hud_on()) {
    draw_hud();
  }
}

void GameplayScene::draw_ground() {
  // want a real ground sprite texture
  const int w = GetScreenWidth();
  const int h = GetScreenHeight() * 4;
  const int offset_h = 20;
  const int x = 0;
  const int y = ground_y - offset_h;
  DrawRectangle(x, y, w, h, RED);
  // DrawRectangle(0, GetScreenHeight() - 10, w, h, c);
}

void GameplayScene::draw_stars() {
  for (auto &s : get_stars()) {
    DrawRectangle(s.second.x, s.second.y, 4, 4, WHITE);
  }
}

void GameplayScene::close() {
  mPrint("Closing gameplay scene...");
  mPrint("Unloading textures...");
  for (auto &t : get_textures()) {
    UnloadTexture(t.second.texture);
  }
  mPrint("Clearing textures...");
  get_textures().clear();
  mPrint("Clearing sprites...");
  get_sprites().clear();
  get_bgsprites().clear();
  mPrint("Clearing entity ids...");
  get_entity_ids().clear();
  get_bg_entity_ids().clear();
  mPrint("Unloading font...");
  UnloadFont(get_global_font());
  mPrint("Clearing stars...");
  get_stars().clear();

  // if (music != NULL) {
  //  stop music
  //  Mix_PauseMusic();
  //  Mix_FreeMusic(music);
  //}

  set_has_been_initialized(false);
  // set_player_id(-1);
  player_id = -1;

  enemies_killed = 0;

  get_popup_manager()->zero_alpha();

  mPrint("Scene closed.");
}

entity_id GameplayScene::spawn_bat() {
  // get player position
  // const float x = sprites[player_id]->get_x();
  auto textures = get_textures();
  auto sprites = get_sprites();
  const int bat_width = textures["bat"].texture.width;

  int roll = rand() % 2;

  const float x = roll ? -bat_width : GetScreenWidth() - 4.0;
  // const float vx = roll ? 2.0f : -2.0f;
  const float vx = 2.0f;

  // here, we are hardcoding the bat y to be the player y
  // const float y = sprites[player_id]->get_y();

  // here, we are adjusting the spawned y in a random direction
  // in order to provide some variance as to the bat spawn location
  // perhaps over time this value can increase in scale to how far
  // into the level the player is
  const float y = sprites[player_id]->get_y() + (rand() % 200 - 100);

  // const int bat_height = get_textures()["bat"].texture.height;
  //(float)GetScreenHeight() / 2 - (float)bat_height + 300;
  return spawn_bat(x, y, vx);
}

entity_id GameplayScene::spawn_bat(const float x, const float y,
                                   const float vx) {
  // mPrint("Spawning bat...");
  entity_id id = spawn_entity("bat", x, y, SPRITETYPE_ENEMY, true);
  auto sprites = get_sprites();
  sprites[id]->set_vx(vx);
  sprites[id]->set_vy(1.0f);
  sprites[id]->set_ax(0.0f);
  sprites[id]->set_ay(0.0f);
  sprites[id]->set_hp(1);
  sprites[id]->set_maxhp(1);

  // sprites[id]->set_movement_type(MOVEMENT_TYPE_NORMAL);
  sprites[id]->set_movement_type(MOVEMENT_TYPE_HOMING);

  return id;
}

void GameplayScene::set_do_ground_movement(const bool d) {
  do_ground_movement = d;
}

void GameplayScene::set_ground_y_movement(const float dy) {
  ground_y_movement = dy;
}

entity_id GameplayScene::spawn_knife() {
  // mPrint("Spawning knife...");
  //  calculate offsets
  //  half the width of the sprite
  const float o_x = get_sprites()[player_id]->get_width();
  const float o_y = get_sprites()[player_id]->get_height() / 2.0;
  float x = get_sprites()[player_id]->get_x();
  float y = get_sprites()[player_id]->get_y() + o_y;
  // get the width of the knife texture
  const float knife_width = get_textures()["knife"].texture.width;
  if (get_sprites()[player_id]->get_is_flipped()) {
    // left side
    x -= knife_width * get_global_scale() + knife_speed.x * 2.0f;
  } else {
    // right side
    x += o_x + knife_speed.x * 2.0f;
  }

  // spawn the knife
  entity_id id = spawn_entity("knife", x, y, SPRITETYPE_KNIFE, false);
  const bool is_spinning = knife_catches > 0;
  const bool is_flipped = get_sprite(player_id)->get_is_flipped();

  float vx = knife_speed.x;
  // const float vy = knife_speed.y;

  if (is_flipped) {
    get_sprite(id)->set_is_flipped(true);
    vx = -vx;
  }

  if (is_spinning) {
    vx = vx * (1 + knife_catches);
    get_sprite(id)->set_is_spinning(true);
    get_sprite(id)->set_rotation_speed(1.0f * knife_catches);
    knife_catches = knife_catches - 1;
  }

  get_sprite(id)->set_vx(vx);
  get_sprite(id)->set_vy(knife_speed.y);
  get_sprite(id)->set_ax(0);
  get_sprite(id)->set_ay(0);
  get_sprite(id)->set_rotation_angle(0.0f);

  return id;
}

entity_id GameplayScene::spawn_player(float x, float y) {
  mPrint("Attempting to spawn player...");
  if (player_id != -1) {
    mPrint("Player already spawned.");
    return player_id;
  }
  mPrint("Spawning player...");
  player_id = spawn_entity("skull", x, y, SPRITETYPE_PLAYER, false);

  const int player_starting_hp = 3;
  const int player_max_hp = 3;
  get_sprite(player_id)->set_maxhp(player_max_hp);
  get_sprite(player_id)->set_hp(player_starting_hp);

  return player_id;
}

entity_id GameplayScene::spawn_soulshard(float x, float y) {

  // mPrint("Spawning soulshard...");
  entity_id id = spawn_entity("soulshard", x, y, SPRITETYPE_SOULSHARD, false);
  get_sprite(id)->set_vx(0.0f);
  get_sprite(id)->set_vy(0.0f);
  get_sprite(id)->set_ax(0.0f);
  get_sprite(id)->set_ay(0.0f);
  get_sprite(id)->set_hp(1);
  get_sprite(id)->set_maxhp(1);
  get_sprite(id)->set_movement_type(MOVEMENT_TYPE_NONE);
  get_sprite(id)->set_is_animating(true);

  return id;
}

/*
const Vector2 Scene::get_starting_knife_speed() const {
  return starting_knife_speed;
}

void Scene::set_knife_speed(const Vector2 speed) { knife_speed = speed; }

const Vector2 Scene::get_knife_speed() const { return knife_speed; }

void Scene::set_knife_catches(const unsigned int catches) {

  knife_catches = catches;
}

const unsigned int Scene::get_knife_catches() const { return knife_catches; }


*/
