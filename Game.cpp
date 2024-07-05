#include "Game.h"
#include "mPrint.h"
#include "raylib.h"
#include <cassert>

Game::Game() {
  global_scale = 2.0f;
  screen_rect = (Rectangle){0, 0, 1280, -720};
  camera2d = {0};
  current_frame = 0;
  set_camera_default_values();
  has_been_initialized = false;
  controlmode = CONTROL_MODE_PLAYER;
}

bool Game::init() {
  if (!has_been_initialized) {
    mPrint("Initializing game...");
    mPrint("Checking that player texture key is set...");
    mPrint("Initializing window...");
    InitWindow(screen_rect.width, -screen_rect.height,
               get_window_title().c_str());
    mPrint("Initializing camera...");
    set_camera_default_values();
    SetTargetFPS(60); // Set our game to run at 60 frames-per-second
    mPrint("Loading scene...");

    scene.set_global_scale(global_scale);
    scene.set_texture_filepath("assets.txt");
    bool result = scene.init();
    if (!result) {
      mPrint("Error loading scene. Exiting...");
      return false;
    }

    mPrint("Loading render texture...");
    target = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());
    mPrint("Setting exit key...");
    SetExitKey(KEY_Q);

    // seed the RNG
    SetRandomSeed(GetTime());

    has_been_initialized = true;
  }
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

Game::~Game() { close(); }
string Game::get_window_title() { return window_title; }
void Game::set_window_title(const char *title) { window_title = title; }
void Game::set_screen_width(int w) { screen_rect.width = w; }
void Game::set_screen_height(int h) { screen_rect.height = -h; }
void Game::set_debug_panel(bool b) { debug_panel_on = b; }
void Game::handle_input() { scene.handle_input(); }
void Game::update() { scene.update(); }
void Game::set_has_been_initialized(bool b) { has_been_initialized = b; }
bool Game::get_has_been_initialized() { return has_been_initialized; }

void Game::set_global_scale(float s) {
  assert(s > 0.0f);
  global_scale = s;
}

void Game::load_fonts() {
  mPrint("Loading fonts...");
  const char font_path[] = "fonts/hack.ttf";
  global_font = LoadFont(font_path);
}

void Game::draw() {
  BeginDrawing();
  BeginTextureMode(target);
  scene.draw();
  EndTextureMode();
  DrawTextureRec(target.texture, screen_rect, (Vector2){0, 0}, WHITE);
  EndDrawing();
  current_frame++;
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

void Game::close() {
  mPrint("Closing game...");
  // have to close the scene first otherwise it crashes
  // CloseWindow() must be the LAST raylib call before exiting
  scene.close();
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
