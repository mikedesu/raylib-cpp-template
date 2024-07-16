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

  // do box2d stuff
  // init world with gravity
  gravity = b2Vec2(0.0f, -10.0f);
  world = new b2World(gravity);

  // define ground and place it
  b2BodyDef groundBodyDef;
  groundBodyDef.position.Set(0.0f, 0.0f);
  // groundBodyDef.position.Set(0.0f, -10.0f);
  groundBody = world->CreateBody(&groundBodyDef);

  b2PolygonShape groundBox;
  // groundBox.SetAsBox(50.0f, 10.0f);
  groundBox.SetAsBox(GetScreenWidth(), 10.0f);
  groundBody->CreateFixture(&groundBox, 0.0f);

  // load_music("/home/darkmage/Music/darkmage/lets-fkn-go.mp3");
}

GameplayScene::~GameplayScene() { mPrint("GameplayScene destructor"); }

void GameplayScene::update_player_movement() {
  shared_ptr<Sprite> player = get_sprites()[get_player_id()];
  // player->incr_ay(0.0032f);
  // player->incr_ay(gravity);
  // player->incr_ay(gravity);
  // player->set_y(player->get_y() + player->get_vy());
  // player->update();
  // player->set_y(player->get_y() + player->get_vy());

  // const int height = player->get_height();
  // const int width = player->get_width();
  // const int bottom_of_sprite = player->get_y() + height;
  //  lock the player to a relative "ground" location
  // const int bottom_of_screen = GetScreenHeight();
  // if (bottom_of_sprite >= bottom_of_screen) {
  //   player->set_y(bottom_of_screen - height);
  // }

  // prevent player from moving off-screen
  // if (player->get_x() < 0) {
  //  player->set_x(0);
  //}
  // if (player->get_x() > GetScreenWidth() - width) {
  //  player->set_x(GetScreenWidth() - width);
  //}

  // player->set_do_update(true);
}

void GameplayScene::update_enemy_movement() {
  // for (auto &s : get_sprites()) {
  //   switch (s.second->get_type()) {
  //   case SPRITETYPE_ENEMY:
  //   case SPRITETYPE_PIPEBASE:
  //     s.second->update();
  //     break;
  //     s.second->update();
  //     break;
  //   }
  // }
}

void GameplayScene::update_knife_movement() {
  // for (auto &s : get_sprites()) {
  //   switch (s.second->get_type()) {
  //   case SPRITETYPE_KNIFE:
  //     s.second->update();
  //     s.second->set_x(s.second->get_x() + s.second->get_vx());
  //     break;
  //   }
  // }
}

void GameplayScene::handle_offscreen() {
  // for (auto &s : get_sprites()) {
  //   const int width = s.second->get_width() * 2;
  //   if (s.second->get_x() > GetScreenWidth() + width ||
  //       s.second->get_x() < -width) {
  //     s.second->mark_for_deletion();
  //   }
  // }
}

void GameplayScene::handle_player_collision() {
  // shared_ptr<Sprite> player = get_sprites()[get_player_id()];
  // for (auto &s : get_sprites()) {
  //   sprite_type t = s.second->get_type();
  //   switch (t) {
  //   case SPRITETYPE_ENEMY:
  //   case SPRITETYPE_KNIFE:
  //   case SPRITETYPE_RED_BRICK:
  //  if (CheckCollisionRecs(player->get_dest(), s.second->get_dest())) {
  //  if (CheckCollisionRecs(player->get_hitbox(), s.second->get_hitbox())) {
  //    if (CheckCollisionRecs(player->get_dest(), s.second->get_dest())) {
  //      if (t == SPRITETYPE_KNIFE) {
  //        s.second->mark_for_deletion();
  //        set_knife_catches(get_knife_catches() + 1);
  //      } else if (t == SPRITETYPE_ENEMY) {
  //        s.second->mark_for_deletion();
  //        player->set_hp(player->get_hp() - 1);
  //        enemies_killed++;
  // if (player->get_hp() <= 0) {
  //   set_scene_transition(SCENE_TRANSITION_OUT);
  //   set_next_scene_id(SCENE_ID_GAMEOVER);
  // }
  //      } else if (t == SPRITETYPE_PIPEBASE) {
  // player cannot move thru the pipe
  //      } else if (t == SPRITETYPE_RED_BRICK) {

  // create a 2nd thin rectangle area at the top of the brick
  // need to research Box2D for collision stuff possibly...
  //        Rectangle top_of_brick = s.second->get_dest();
  //        top_of_brick.height /= 2.0f;
  //        Rectangle bottom_of_player = player->get_dest();
  //        bottom_of_player.y += player->get_height() / 2.0f;
  //        bottom_of_player.height /= 2.0f;

  //        if (CheckCollisionRecs(bottom_of_player, top_of_brick)) {
  // player->set_y(s.second->get_y() - player->get_height());
  // player->set_y(s.second->get_y() - player->get_height());
  //          player->set_do_update(false);
  //        }
  //      }
  //    }
  //    break;
  //  default:
  //    break;
  //  }
  //}
}

void GameplayScene::handle_knife_collisions() {
  // for (auto &knife : get_sprites()) {
  //   sprite_type t = knife.second->get_type();
  //   switch (t) {
  //   case SPRITETYPE_KNIFE: {
  //     for (auto &s : get_sprites()) {
  //       sprite_type t2 = s.second->get_type();
  //       switch (t2) {
  //       case SPRITETYPE_ENEMY: {
  //         // if (CheckCollisionRecs(knife.second->get_dest(),
  //         // s.second->get_dest())) {
  //         if (CheckCollisionRecs(knife.second->get_dest(),
  //                                s.second->get_dest())) {
  //
  //            s.second->set_hp(s.second->get_hp() - 1);
  //            if (s.second->get_hp() <= 0) {
  //              s.second->mark_for_deletion();
  //              knife.second->mark_for_deletion();
  //              // Mix_PlayChannel(-1, sfx_knife_hit, 0);
  //              enemies_killed++;
  //            }
  //          }
  //          break;
  //        }
  //        }
  //      }
  //      break;
  //    }
  //    }
  //  }
}

void GameplayScene::update() {
  const float star_move_speed = -1.0f;

  if (!get_paused()) {

    // handle_offscreen();
    // handle_player_collision();
    // handle_knife_collisions();

    update_stars_vx(star_move_speed);

    // update_player_movement();
    // update_enemy_movement();
    // update_knife_movement();

    /*
    handle_offscreen();
    handle_player_collision();
    handle_knife_collisions();
    */

    // we want the camera to follow the player in such a way that
    // the player is always in the center of the screen
    // except for the game beginning, when they begin flapping at the bottom
    // once they hit the center, the camera should follow them

    Camera2D &camera2d = get_camera2d();
    camera2d.target.y = get_sprites()[get_player_id()]->get_y();

    // camera2d.offset.y = GetScreenHeight() / 2.0f;
    //
    // UpdateMusicStream(get_music());
  }
}

void GameplayScene::handle_input() {
  shared_ptr<Sprite> player = get_sprites()[get_player_id()];
  // this value affects how high skull 'flaps' or jumps
  const float vy = -6.75f;
  // const float vy = -6.75f;
  const float ay = 0.0f;
  const float move_speed = 2.0f;

  if (IsKeyPressed(KEY_D)) {
    flip_debug_panel();
  }

  if (!get_paused()) {

    if (IsKeyPressed(KEY_C)) {
      set_control_mode(CONTROL_MODE_CAMERA);
    }

    // if (IsKeyPressed(KEY_B)) {
    //   const int bat_width = get_textures()["bat"].texture.width;
    //   const int bat_height = get_textures()["bat"].texture.height;
    //   const float bat_x = -bat_width;
    //   const float bat_y =
    //       (float)GetScreenHeight() / 2 - (float)bat_height + 300;
    //   spawn_bat(bat_x, bat_y);
    // }

    // if (IsKeyPressed(KEY_P)) {
    //   const int pipebase_width = get_textures()["pipebase"].texture.width;
    //   const int pipebase_height = get_textures()["pipebase"].texture.height;
    //   const float pipebase_x = -pipebase_width;
    //   const float pipebase_y = GetScreenHeight() - pipebase_height * 4;
    //   spawn_pipebase(pipebase_x, pipebase_y);
    // }

    // if (IsKeyPressed(KEY_Z)) {
    //   spawn_knife();
    // }

    // if (IsKeyPressed(KEY_SPACE)) {
    //   player->set_ay(0.0f);
    //   player->set_vy(vy);
    //   player->update();
    // }

    // bool shift = IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT);
    // float new_move_speed = move_speed;
    // if (shift) {
    //   new_move_speed = new_move_speed * 2;
    // }

    // if (IsKeyDown(KEY_LEFT)) {
    //   player->set_vx(-new_move_speed);
    //   if (!player->get_is_flipped()) {
    //     player->flip();
    //   }
    // } else if (IsKeyDown(KEY_RIGHT)) {
    //   player->set_vx(new_move_speed);
    //   if (player->get_is_flipped()) {
    //     player->flip();
    //   }
    // } else {
    //   player->set_vx(0.0f);
    // }

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

    for (int i = 0; i < 10000; i++) {
      add_star();
    }

    // const int bat_width = get_textures()["bat"].texture.width;
    // const int bat_height = get_textures()["bat"].texture.height;
    // const float bat_x = -bat_width;
    // const float bat_y = (float)GetScreenHeight() / 2 - (float)bat_height +
    // 300; spawn_bat(bat_x, bat_y);

    // spawn a red brick
    // const int redbrick_width = get_textures()["redbrick"].texture.width;
    // const int redbrick_height = get_textures()["redbrick"].texture.height;
    // const float redbrick_x = GetScreenWidth() / 2.0 - redbrick_width;
    // const float redbrick_y = 0;
    // spawn_redbrick(redbrick_x, redbrick_y);

    get_camera2d().offset.y = GetScreenHeight() / 2.0f;

    load_music("audio/skull-title-0.mp3");
    Mix_PlayMusic(get_music(), -1);

    sfx_knife_throw = Mix_LoadWAV("audio/knife-throw.mp3");
    sfx_knife_hit = Mix_LoadWAV("audio/knife-hit.mp3");

    if (sfx_knife_throw == nullptr || sfx_knife_hit == nullptr) {
      mPrint("Error loading sound effects. Exiting...");
      return false;
    }

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
      "Player Velocity: " +
      to_string(get_sprites()[get_player_id()]->get_vx()) + ", " +
      to_string(get_sprites()[get_player_id()]->get_vy()) + "\n" +
      "Player Acceleration: " +
      to_string(get_sprites()[get_player_id()]->get_ax()) + ", " +
      to_string(get_sprites()[get_player_id()]->get_ay()) + "\n" +
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

void GameplayScene::draw_hud() {
  const int player_hp = get_sprites()[get_player_id()]->get_hp();
  const int player_maxhp = get_sprites()[get_player_id()]->get_maxhp();
  const string hp_str =
      "HP: " + to_string(player_hp) + "/" + to_string(player_maxhp);

  const int knife_catches = get_knife_catches();
  const string knife_catches_str = "Knife Catches: " + to_string(knife_catches);
  const string enemies_killed_str = "Killed: " + to_string(enemies_killed);

  const string full_str = hp_str + " " + knife_catches_str + " " +
                          enemies_killed_str + " " + "\nPress Z to throw knife";

  // DrawRectangle(0, 0, 500, 20, Fade(BLACK, 0.5f));
  DrawTextEx(get_global_font(), full_str.c_str(), (Vector2){10, 10}, 20, 0.5f,
             WHITE);
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

bool GameplayScene::ccw(float x1, float y1, float x2, float y2, float x3,
                        float y3) {
  float a = x1 * (y2 - y3) - y1 * (x2 - x3) + (x2 * y3 - x3 * y2);
  return a < 0;
}

bool GameplayScene::line_did_cross_line(Vector4 &line1, Vector4 &line2) {

  return ccw(line1.x, line1.y, line2.x, line2.y, line2.z, line2.w) !=
             ccw(line1.z, line1.w, line2.x, line2.y, line2.z, line2.w) &&
         ccw(line1.x, line1.y, line1.z, line1.w, line2.x, line2.y) !=
             ccw(line1.x, line1.y, line1.z, line1.w, line2.z, line2.w);
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
  draw_stars();
  draw_ground();

  for (auto &s : get_sprites()) {
    s.second->draw();
    if (get_debug_panel_on()) {
      s.second->draw_hitbox();
    }
  }

  EndMode2D();

  // draw debug panel
  if (get_debug_panel_on()) {
    DrawFPS(GetScreenWidth() - 80, 10);
    draw_debug_panel();
  } else if (get_hud_on()) {
    draw_hud();
  }

  increment_frame();
}
