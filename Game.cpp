#include "Game.h"
#include "mPrint.h"
#include <cassert>
#include <cstdio>
#include <cstring>
#include <raylib.h>
#include <string>

using std::to_string;

static entity_id next_entity_id = 0;

Game::Game() {
  global_scale = 2.0f;
  screen_rect = (Rectangle){0, 0, 1280, -720};
  camera2d = {0};
  current_frame = 0;
  set_camera_default_values();
  has_been_initialized = false;
  controlmode = CONTROL_MODE_PLAYER;
}

void Game::init() {
  if (!has_been_initialized) {
    mPrint("Initializing game...");
    mPrint("Checking that player texture key is set...");
    mPrint("Initializing window...");
    InitWindow(screen_rect.width, -screen_rect.height,
               get_window_title().c_str());
    mPrint("Initializing camera...");
    set_camera_default_values();

    SetTargetFPS(60); // Set our game to run at 60 frames-per-second
    // mPrint("Loading assets...");
    mPrint("Loading scene...");
    scene.init();

    // load_fonts();
    // bool result = load_textures();
    // if (!result) {
    //   mPrint("Error loading textures. Exiting...");
    //   return;
    // }

    mPrint("Loading render texture...");
    target = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());
    mPrint("Setting exit key...");
    SetExitKey(KEY_Q);

    // spawn_player(0, 0);
    // const int sprite_width = textures["skull"].texture.width;
    // const int sprite_height = textures["skull"].texture.height;
    // const float x = (float)GetScreenWidth() / 2 - (float)sprite_width;
    // const float y = (float)GetScreenHeight() / 2 - (float)sprite_height;
    // spawn_player(x, y);

    has_been_initialized = true;
  }
}

bool Game::load_textures() {
  mPrint("Loading textures...");
  const char *filename = "assets.txt";
  FILE *file = fopen(filename, "r");
  if (file == NULL) {
    mPrint("Error opening file: " + string(filename));
    return false;
  }
  char line[256];
  while (fgets(line, sizeof(line), file)) {
    char *asset_name = strtok(line, " ");
    char *num_frames = strtok(NULL, " ");
    char *is_player = strtok(NULL, " ");
    char *asset_path = strtok(NULL, " ");
    mPrint("Asset name: " + string(asset_name));
    mPrint("Num frames: " + string(num_frames));
    mPrint("Is player: " + string(is_player));
    mPrint("Asset path: " + string(asset_path));
    bool result =
        load_texture(asset_name, asset_path, atoi(num_frames), atoi(is_player));
    if (!result) {
      mPrint("Error loading texture: " + string(asset_path));
      return false;
    }
  }
  fclose(file);
  return true;
}

bool Game::load_texture(const char *asset_name, const char *asset_path,
                        const int num_frames, const int is_player) {
  mPrint("Attempting to load texture: " + string(asset_name));
  Texture2D t = LoadTexture(asset_path);
  texture_info ti;
  ti.texture = t;
  ti.num_frames = num_frames;
  ti.is_player = is_player;
  ti.asset_path = asset_path;
  textures[asset_name] = ti;
  return true;
}

void Game::set_camera_default_values() {
  camera2d.target.x = 0;
  camera2d.target.y = 0;
  camera2d.offset.x = 0;
  camera2d.offset.y = 0;
  camera2d.rotation = 0.0f;
  camera2d.zoom = 1;
}

string Game::get_window_title() { return window_title; }

void Game::set_window_title(const char *title) { window_title = title; }
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

void Game::handle_camera_input() {
  /*
  if (IsKeyPressed(KEY_C)) {
    controlmode = CONTROL_MODE_PLAYER;
  }

  if (IsKeyPressed(KEY_R)) {
    set_camera_default_values();
  }

  if (IsKeyDown(KEY_LEFT)) {
    // move camera
    camera2d.target.x -= 2;
  } else if (IsKeyDown(KEY_RIGHT)) {
    camera2d.target.x += 2;
  }

  if (IsKeyDown(KEY_UP)) {
    camera2d.target.y -= 2;
  } else if (IsKeyDown(KEY_DOWN)) {
    camera2d.target.y += 2;
  }

  if (IsKeyDown(KEY_Z)) {
    camera2d.zoom += 0.1f;
  } else if (IsKeyDown(KEY_X)) {
    camera2d.zoom -= 0.1f;
  }
  */
}

void Game::handle_player_input() {
  /*
  if (IsKeyPressed(KEY_C)) {
    controlmode = CONTROL_MODE_CAMERA;
  }

  if (IsKeyPressed(KEY_SPACE)) {
    sprites[player_id]->set_ay(0.00f);
    sprites[player_id]->set_vy(-2.0f);
    sprites[player_id]->update();
  }

  if (IsKeyDown(KEY_LEFT)) {
    // move player
    sprites[player_id]->move(-2, 0);
    if (!sprites[player_id]->get_is_flipped()) {
      sprites[player_id]->flip();
    }
  } else if (IsKeyDown(KEY_RIGHT)) {
    sprites[player_id]->move(2, 0);
    if (sprites[player_id]->get_is_flipped()) {
      sprites[player_id]->flip();
    }
  }
  */
}

void Game::handle_input() {

  scene.handle_input();

  // if (IsKeyPressed(KEY_D)) {
  //   debug_panel_on = !debug_panel_on;
  //   scene.set_debug_panel_on(debug_panel_on);
  // }

  /*
  switch (controlmode) {
  case CONTROL_MODE_PLAYER:
    handle_player_input();
    break;
  case CONTROL_MODE_CAMERA:
    handle_camera_input();
    break;
  default:
    break;
  }
  */
}

void Game::draw() {
  BeginDrawing();
  BeginTextureMode(target);

  scene.draw();

  // BeginMode2D(camera2d);
  // ClearBackground(BROWN);
  //  no background yet, but lets mock one up with shapes
  //  draw a large rectangle to represent a scene
  //  but lets make the dimension ratio 720x1280
  // DrawRectangle(GetScreenWidth() / 2 - 405 / 2, 0, 405, 720, BLACK);
  // for (auto &s : sprites) {
  //   s.second->draw();
  //   if (debug_panel_on) {
  //     s.second->draw_hitbox();
  //   }
  // }
  // EndMode2D();

  EndTextureMode();
  DrawTextureRec(target.texture, screen_rect, (Vector2){0, 0}, WHITE);

  EndDrawing();
  current_frame++;
}

void Game::update() {
  // gravity
  // for (auto &s : sprites) {
  //  if (s.second->get_type() == SPRITETYPE_PLAYER) {
  //    s.second->incr_ay(0.0032f);
  //    s.second->update();
  //  }
  //}

  scene.update();
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

void Game::draw_debug_panel() {

  string camera_info_str =
      "Current Frame: " + to_string(current_frame) + "\n" +
      "Camera2D target: " + to_string(camera2d.target.x) + ", " +
      to_string(camera2d.target.y) + "\n" +
      "Camera2D offset: " + to_string(camera2d.offset.x) + ", " +
      to_string(camera2d.offset.y) + "\n" +
      "Camera2D rotation: " + to_string(camera2d.rotation) + "\n" +
      "Camera2D zoom: " + to_string(camera2d.zoom) + "\n" +
      "Player Position: " + to_string(sprites[player_id]->get_x()) + ", " +
      to_string(sprites[player_id]->get_y()) + "\n" +
      "Player Velocity: " + to_string(sprites[player_id]->get_vx()) + ", " +
      to_string(sprites[player_id]->get_vy()) + "\n" +
      "Player Acceleration: " + to_string(sprites[player_id]->get_ax()) + ", " +
      to_string(sprites[player_id]->get_ay()) + "\n" +
      "Control mode: " + to_string(controlmode) + "\n";

  DrawRectangle(0, 0, 500, 200, Fade(BLACK, 0.5f));
  DrawTextEx(global_font, camera_info_str.c_str(), (Vector2){10, 10}, 16, 0.5f,
             WHITE);
}

void Game::close() {
  mPrint("Closing game...");
  mPrint("Unloading textures...");
  for (auto &t : textures) {
    UnloadTexture(t.second.texture);
  }
  mPrint("Clearing textures...");
  textures.clear();
  mPrint("Clearing sprites...");
  sprites.clear();
  if (IsRenderTextureReady(target)) {
    mPrint("Unloading render texture...");
    UnloadRenderTexture(target);
  }
  // close font
  UnloadFont(global_font);
  if (IsWindowReady()) {
    mPrint("Closing window...");
    CloseWindow();
  }
  mPrint("Game closed.");
}

Game::~Game() { close(); }

void Game::set_has_been_initialized(bool b) { has_been_initialized = b; }
bool Game::get_has_been_initialized() { return has_been_initialized; }

entity_id Game::spawn_player(float x, float y) {
  mPrint("Spawning player...");
  entity_id id = spawn_entity("skull", x, y, SPRITETYPE_PLAYER);
  player_id = id;
  return id;
}

entity_id Game::spawn_entity(const char *texture_key, float x, float y,
                             sprite_type type) {
  mPrint("Spawning entity...");
  shared_ptr<Sprite> s =
      make_shared<Sprite>(textures[texture_key].texture,
                          textures[texture_key].num_frames, x, y, type);
  if (s == nullptr) {
    mPrint("Error creating sprite.");
    return -1;
  }
  s->set_scale(global_scale);

  sprites[next_entity_id] = s;
  return next_entity_id++;
}
