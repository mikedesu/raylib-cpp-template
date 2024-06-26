#include "Game.h"
#include "mPrint.h"
#include <cassert>
// #include <cstdio>
// #include <cstdlib>
// #include <cstring>

using std::to_string;

static entity_id next_entity_id = 0;

Game::Game() {
  screen_rect = (Rectangle){0, 0, 0, 0};
  camera = {0};
  has_been_initialized = false;
}

void Game::init() {
  if (!has_been_initialized) {
    mPrint("Initializing game...");
    has_been_initialized = true;
    InitWindow(screen_rect.width, -screen_rect.height,
               get_window_title().c_str());
    SetTargetFPS(60); // Set our game to run at 60 frames-per-second
    load_assets();
    target = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());
    SetExitKey(KEY_Q);
    spawn_player();
  }
}

void Game::set_camera_default_values() {
  camera.target.x = 0;
  camera.target.y = 0;
  camera.offset.x = 0;
  camera.offset.y = 0;
  camera.rotation = 0.0f;
  camera.zoom = 0;
}

void Game::set_window_title(const char *title) { window_title = title; }

string Game::get_window_title() { return window_title; }

void Game::set_screen_width(int w) { screen_rect.width = w; }

void Game::set_screen_height(int h) { screen_rect.height = -h; }

void Game::set_debug_panel(bool b) { debug_panel_on = b; }

void Game::set_global_scale(float s) {
  assert(s > 0.0f);
  global_scale = s;
}

void Game::load_fonts() {
  mPrint("Loading fonts...");
  const char font_path[] = "fonts/hack.ttf";
  global_font = LoadFont(font_path);
}

void Game::add_texture_to_load(const char *asset_name, const char *asset_path,
                               int num_frames, int is_player) {
  Texture2D texture = {0};
  texture_info info = {texture, num_frames, is_player, asset_path};
  textures[asset_name] = info;
}

void Game::load_texture(const char *asset_name) {
  const char *asset_path = textures[asset_name].asset_path.c_str();
  const int num_frames = textures[asset_name].num_frames;
  const int is_player = textures[asset_name].is_player;
  Texture2D texture = LoadTexture(asset_path);
  texture_info info = {texture, num_frames, is_player, asset_path};
  textures[asset_name] = info;
}

void Game::load_assets() {
  mPrint("Loading assets...");
  load_fonts();
  for (auto &texture : textures) {
    load_texture(texture.first.c_str());
  }
}

void Game::spawn_player() {
  shared_ptr<Sprite> sprite = make_shared<Sprite>(textures["skull"].texture,
                                                  textures["skull"].num_frames,
                                                  0, 0, SPRITETYPE_PLAYER);
  sprite->set_scale(global_scale);
  sprites[next_entity_id] = sprite;
  player_id = next_entity_id;
  next_entity_id++;
}

void Game::spawn_bat() {
  int x = GetRandomValue(0, GetScreenWidth());
  int y = GetRandomValue(0, GetScreenHeight());
  shared_ptr<Sprite> sprite =
      make_shared<Sprite>(textures["bat"].texture, textures["bat"].num_frames,
                          x, y, SPRITETYPE_ENEMY);
  sprite->set_scale(global_scale);
  sprite->set_is_animating(true);
  sprites[next_entity_id] = sprite;
  next_entity_id++;
}

void Game::spawn_knife() {
  // get skull's position
  int x = sprites[player_id]->get_dest().x;
  int y = sprites[player_id]->get_dest().y;
  int w = sprites[player_id]->get_width();
  int h = sprites[player_id]->get_height();
  shared_ptr<Sprite> sprite =
      make_shared<Sprite>(textures["knife"].texture,
                          textures["knife"].num_frames, x, y, SPRITETYPE_KNIFE);
  sprite->set_scale(global_scale);
  if (sprites[player_id]->get_is_flipped()) {
    // need to subtrack width of KNIFE not skull
    x -= sprite->get_width();
  } else {
    x += w;
  }
  // x += w;
  y += h / 2;
  sprite->set_x(x);
  sprite->set_y(y);
  if (sprites[player_id]->get_is_flipped()) {
    sprite->set_vx(-1.0f);
    sprite->set_is_flipped(true);
  } else {
    sprite->set_vx(1.0f);
  }
  sprite->set_vy(0.0f);
  sprites[next_entity_id] = sprite;
  next_entity_id++;
}

void Game::handle_input() {
  if (IsKeyDown(KEY_UP)) {
    // shift key
    if (sprites[player_id]->get_dest().y > 0) {
      if (IsKeyDown(KEY_LEFT_SHIFT)) {
        sprites[player_id]->move_pos_y(-2.0f);
      } else {
        sprites[player_id]->move_pos_y(-1.0f);
      }
    }
  } else if (IsKeyDown(KEY_DOWN)) {
    if (sprites[player_id]->get_dest().y <
        GetScreenHeight() - sprites[player_id]->get_height()) {
      if (IsKeyDown(KEY_LEFT_SHIFT)) {
        sprites[player_id]->move_pos_y(2.0f);
      } else {
        sprites[player_id]->move_pos_y(1.0f);
      }
    }
  }
  if (IsKeyDown(KEY_LEFT)) {
    if (sprites[player_id]->get_dest().x > 0) {

      if (IsKeyDown(KEY_LEFT_SHIFT)) {
        sprites[player_id]->move_pos_x(-2.0f);
      } else {
        sprites[player_id]->move_pos_x(-1.0f);
      }
      sprites[player_id]->set_is_flipped(true);
    }
  } else if (IsKeyDown(KEY_RIGHT)) {
    if (sprites[player_id]->get_dest().x <
        GetScreenWidth() - sprites[player_id]->get_width()) {
      if (IsKeyDown(KEY_LEFT_SHIFT)) {
        sprites[player_id]->move_pos_x(2.0f);
      } else {
        sprites[player_id]->move_pos_x(1.0f);
      }
      sprites[player_id]->set_is_flipped(false);
    }
  }

  if (IsKeyDown(KEY_Z)) {
    sprites[player_id]->set_current_frame(1);
  } else if (IsKeyUp(KEY_Z)) {
    sprites[player_id]->set_current_frame(0);
  }

  if (IsKeyPressed(KEY_Z)) {
    spawn_knife();
  }

  if (IsKeyPressed(KEY_S)) {
    sprites[player_id]->set_scale(sprites[player_id]->get_scale() + 1.0f);
  }

  if (IsKeyPressed(KEY_A)) {
    if (sprites[player_id]->get_scale() > 1.0f) {
      sprites[player_id]->set_scale(sprites[player_id]->get_scale() - 1.0f);
    }
  }

  if (IsKeyPressed(KEY_B)) {
    spawn_bat();
  }

  if (IsKeyPressed(KEY_F)) {
    ToggleFullscreen();
  }

  if (IsKeyPressed(KEY_D)) {
    debug_panel_on = !debug_panel_on;
  }
}

// we want to slow down how fast the animations occur
// so we only increment the frame every 10 frames
void Game::draw_sprite(entity_id id) {
  sprites[id]->draw();
  if (sprites[id]->get_is_animating() && current_frame % 10 == 0) {
    sprites[id]->incr_frame();
  }
  if (debug_panel_on) {
    sprites[id]->draw_hitbox();
  }
}

void Game::draw() {
  BeginDrawing();
  BeginMode2D(camera);
  BeginTextureMode(target);
  ClearBackground(WHITE);
  for (auto &sprite : sprites) {
    entity_id id = sprite.first;
    draw_sprite(id);
  }
  EndTextureMode();
  EndMode2D();
  DrawTextureRec(target.texture, screen_rect, (Vector2){0, 0}, WHITE);
  DrawFPS(10, 10);
  // draw debug panel
  if (debug_panel_on) {
    draw_debug_panel(camera, global_font);
  }
  EndDrawing();
  current_frame++;
}

void Game::handle_player_movement() {
  // check for collision with other sprites
  for (auto &sprite : sprites) {
    // avoid comparing with self
    if (sprite.first == player_id) {
      continue;
    }
    // check if player collides with other sprites
    if (CheckCollisionRecs(sprites[player_id]->get_dest(),
                           sprite.second->get_dest())) {
      // if the sprite is the player, we don't want to delete it
      sprite.second->mark_for_deletion();
    }
  }
}

void Game::handle_knife_movement() {
  for (auto &sprite : sprites) {
    SpriteType type = sprite.second->get_type();
    if (type != SPRITETYPE_KNIFE) {
      continue;
    }
    // entity_id id = sprite.first;
    sprite.second->move_pos_x(sprite.second->get_vx());
    sprite.second->move_pos_y(sprite.second->get_vy());
    // handle collision detection
    for (auto &sprite2 : sprites) {
      entity_id id = sprite2.first;
      SpriteType type2 = sprite2.second->get_type();
      if (type2 != SPRITETYPE_ENEMY) {
        continue;
      }
      // for some reason, game messing up when skull runs into knife...
      if (CheckCollisionRecs(sprite.second->get_dest(),
                             sprite2.second->get_dest())) {
        // if the sprite is the player, we don't want to delete it
        sprite.second->mark_for_deletion();
        sprite2.second->mark_for_deletion();
        continue;
      }
    }
    // we will need a function to check bounding box intersection
    // check if sprite moves off-screen
    if (sprite.second->get_dest().x < 0 ||
        sprite.second->get_dest().x > GetScreenWidth() ||
        sprite.second->get_dest().y < 0 ||
        sprite.second->get_dest().y > GetScreenHeight()) {
      sprite.second->mark_for_deletion();
      continue;
    }
  }
}

void Game::handle_sprite_movement(entity_id id) {
  // we need to separate out movement handling by entity type
  //
  // player
  // knives
  // enemies
  //
  // this way we can likely avoid some of the bugginess currently experienced
  if (id == player_id) {
    return;
  }
  sprites[id]->move_pos_x(sprites[id]->get_vx());
  sprites[id]->move_pos_y(sprites[id]->get_vy());
  // handle collision detection
  // we will need a function to check bounding box intersection
  // check if sprite moves off-screen
  if (sprites[id]->get_dest().x < 0 ||
      sprites[id]->get_dest().x > GetScreenWidth() ||
      sprites[id]->get_dest().y < 0 ||
      sprites[id]->get_dest().y > GetScreenHeight()) {
    sprites[id]->mark_for_deletion();
    return;
  }
  // for every other sprite, check if it collides with the current sprite
  for (auto &sprite : sprites) {
    entity_id sprite_id = sprite.first;
    if (sprite.first == player_id || sprite_id == id) {
      continue;
    }
    // for some reason, game messing up when skull runs into knife...
    if (CheckCollisionRecs(sprites[id]->get_dest(),
                           sprite.second->get_dest())) {
      // if the sprite is the player, we don't want to delete it
      sprites[id]->mark_for_deletion();
    }
  }
}

void Game::handle_sprites() {
  handle_player_movement();
  handle_knife_movement();
  // handle_enemy_movement();
  // for (auto &sprite : sprites) {
  //     entity_id id = sprite.first;
  //     handle_sprite_movement(id);
  //   }
}

void Game::update() {
  handle_sprites();
  // handle collision separate from movement
  // this style of loop is likely the culprit of the freezing...
  // constant iterator desu ka?
  for (auto it = sprites.cbegin(); it != sprites.cend();) {
    if (it->second->get_is_marked_for_deletion()) {
      it = sprites.erase(it);
    } else {
      it++;
    }
  }
}

void Game::run() {
  if (!has_been_initialized) {
    mPrint("Game has not been initialized. Exiting...");
  } else {
    while (!WindowShouldClose()) {
      handle_input();
      update();
      draw();
    }
  }
}

void Game::draw_debug_panel(Camera2D &camera, Font &font) {
  string camera_info_str = "Current Frame: " + to_string(current_frame) +
                           "\nCamera: " + to_string(camera.target.x) + ", " +
                           to_string(camera.target.y) + "\nPlayer: " +
                           to_string(sprites[player_id]->get_dest().x) + ", " +
                           to_string(sprites[player_id]->get_dest().y) + "\n" +
                           to_string(sprites.size()) + " sprites";
  DrawRectangle(0, 0, 500, 200, Fade(BLACK, 0.5f));
  DrawTextEx(font, camera_info_str.c_str(), (Vector2){10, 10}, 16, 0.5f, WHITE);
}

void Game::close() {
  mPrint("Closing game...");
  mPrint("Unloading textures...");
  for (auto &texture : textures) {
    UnloadTexture(texture.second.texture);
  }
  mPrint("Clearing textures...");
  textures.clear();
  mPrint("Clearing sprites...");
  sprites.clear();

  if (IsRenderTextureReady(target)) {
    mPrint("Unloading render texture...");
    UnloadRenderTexture(target);
  }

  if (IsWindowReady()) {
    mPrint("Closing window...");
    CloseWindow();
  }
  mPrint("Game closed.");
}

Game::~Game() { close(); }

void Game::set_has_been_initialized(bool b) { has_been_initialized = b; }
bool Game::get_has_been_initialized() { return has_been_initialized; }
