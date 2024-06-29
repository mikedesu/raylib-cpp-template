#include "Game.h"
#include "mPrint.h"
#include <algorithm>
#include <cassert>
// #include <cstdio>
// #include <cstdlib>
// #include <cstring>

using std::for_each;
using std::to_string;

static entity_id next_entity_id = 0;

Game::Game() {
  screen_rect = (Rectangle){0, 0, 0, 0};
  camera = {0};
  has_been_initialized = false;
  entity_ids.clear();
  collision_functions.clear();
}

void Game::init() {
  if (!has_been_initialized) {
    mPrint("Initializing game...");

    mPrint("Checking that player texture key is set...");
    if (player_texture_key.empty()) {
      mPrint("Player texture key not set. Exiting...");
      return;
    }

    mPrint("Initializing window...");
    InitWindow(screen_rect.width, -screen_rect.height,
               get_window_title().c_str());
    mPrint("Initializing camera...");
    SetTargetFPS(60); // Set our game to run at 60 frames-per-second
    mPrint("Loading assets...");
    load_assets();
    mPrint("Loading render texture...");
    target = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());
    mPrint("Setting exit key...");
    SetExitKey(KEY_Q);

    mPrint("Spawning player...");
    spawn_player(get_player_texture_key().c_str());
    has_been_initialized = true;
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

entity_id Game::spawn_entity(const char *texture_key, sprite_type type) {
  if (textures.find(texture_key) == textures.end()) {
    mPrint("Texture key not found. Exiting...");
    return -1;
  }

  shared_ptr<Sprite> sprite =
      make_shared<Sprite>(textures[texture_key].texture,
                          textures[texture_key].num_frames, 0, 0, type);
  sprite->set_scale(global_scale);
  sprites[next_entity_id] = sprite;
  entity_ids.push_back(next_entity_id);
  return next_entity_id++;
}

entity_id Game::spawn_player(const char *texture_key) {
  player_id = spawn_entity(texture_key, SPRITETYPE_PLAYER);
  return player_id;
}

void Game::spawn_bat() {
  int x = GetRandomValue(0, GetScreenWidth());
  int y = GetRandomValue(0, GetScreenHeight());
  entity_id bat_id = spawn_entity("bat", SPRITETYPE_ENEMY);
  sprites[bat_id]->set_x(x);
  sprites[bat_id]->set_y(y);
  sprites[bat_id]->set_is_animating(true);
}

void Game::spawn_knife() {
  // get skull's position
  int x = sprites[player_id]->get_dest().x;
  int y = sprites[player_id]->get_dest().y;
  int w = sprites[player_id]->get_width();
  int h = sprites[player_id]->get_height();
  entity_id id = spawn_entity("knife", SPRITETYPE_KNIFE);
  if (sprites[player_id]->get_is_flipped()) {
    // need to subtrack width of KNIFE not skull
    x -= sprites[id]->get_width();
  } else {
    x += w;
  }
  y += h / 2;
  sprites[id]->set_x(x);
  sprites[id]->set_y(y);
  if (sprites[player_id]->get_is_flipped()) {
    sprites[id]->set_vx(-1.0f);
    sprites[id]->set_is_flipped(true);
  } else {
    sprites[id]->set_vx(1.0f);
  }
  sprites[id]->set_vy(0.0f);
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

void Game::handle_player_movement() {}

void Game::handle_knife_movement() {}

void Game::update_cleanup() {
  for (auto it = sprites.cbegin(); it != sprites.cend();) {
    if (it->second->get_is_marked_for_deletion()) {
      entity_id id = it->first;
      it = sprites.erase(it);
      entity_ids.erase(remove(entity_ids.begin(), entity_ids.end(), id),
                       entity_ids.end());
    } else {
      it++;
    }
  }
}

void Game::update() {
  for_each(collision_functions.begin(), collision_functions.end(),
           [&](auto f) { for_each(entity_ids.begin(), entity_ids.end(), f); });
  // for (auto id : entity_ids) {
  //   for (auto &f : collision_functions) {
  //     f(id);
  //   }
  // }
  update_cleanup();
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

    mPrint("Window closed.");
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
  // attempting to clear entity_ids causes a crash on close lol
  // mPrint("Clearing entity ids...");
  // entity_ids.clear();

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

string Game::get_player_texture_key() { return player_texture_key; }

void Game::set_player_texture_key(const char *key) { player_texture_key = key; }

void Game::add_collision_func(function<void(entity_id)> f) {
  collision_functions.push_back(f);
}

shared_ptr<Sprite> Game::get_sprite(entity_id id) { return sprites[id]; }

entity_id Game::get_player_id() { return player_id; }

bool Game::entity_id_exists(entity_id id) {

  return find(entity_ids.begin(), entity_ids.end(), id) != entity_ids.end();
}

vector<entity_id> &Game::get_entity_ids() { return entity_ids; }
