#include "Game.h"
#include "mPrint.h"
#include "raylib.h"
#include <cassert>

static scene_id next_scene_id = 0;

Game::Game() {
  global_scale = 1.0f;
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

    shared_ptr<Scene> title_scene = make_shared<TitleScene>();
    title_scene->set_id(next_scene_id++);
    // title_scene->set_alpha(1.0f);
    scenes[title_scene->get_id()] = title_scene;
    scene_keys["title"] = title_scene->get_id();

    shared_ptr<Scene> gameplay_scene = make_shared<GameplayScene>();
    gameplay_scene->set_id(next_scene_id++);
    scenes[gameplay_scene->get_id()] = gameplay_scene;
    scene_keys["gameplay"] = gameplay_scene->get_id();

    for (auto &scene : scenes) {
      mPrint("Initializing scenes...");
      bool result = scene.second->init();
      if (!result) {
        mPrint("Error loading scene. Exiting...");
        return false;
      }
    }

    if (scene_keys.find("title") == scene_keys.end()) {
      mPrint("Error: title scene not found.");
      return false;
    }

    current_scene_id = scene_keys["title"];

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

void Game::handle_input() { scenes[current_scene_id]->handle_input(); }

void Game::update() { scenes[current_scene_id]->update(); }

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

void Game::handle_transition_out() {
  const Color c = (Color){0x66, 0x66, 0x66};
  const float a = scenes[current_scene_id]->get_alpha();
  const int w = GetScreenWidth();
  const int h = GetScreenHeight();
  const float transition_speed = 0.005f;
  DrawRectangle(0, 0, w, h, Fade(c, a));

  if (a < 1.0f) {
    scenes[current_scene_id]->set_alpha(scenes[current_scene_id]->get_alpha() +
                                        transition_speed);
  } else {
    // scenes[current_scene_id]->set_scene_transition(SCENE_TRANSITION_NONE);
  }
}

void Game::handle_transition_in() {
  const Color c = (Color){0x66, 0x66, 0x66};
  const float a = scenes[current_scene_id]->get_alpha();
  const int w = GetScreenWidth();
  const int h = GetScreenHeight();
  const float transition_speed = 0.005f;
  DrawRectangle(0, 0, w, h, Fade(c, a));

  if (a > 0.0f) {
    scenes[current_scene_id]->set_alpha(scenes[current_scene_id]->get_alpha() -
                                        transition_speed);
  } else {
    scenes[current_scene_id]->set_scene_transition(SCENE_TRANSITION_NONE);
  }
}

void Game::draw() {
  BeginDrawing();
  BeginTextureMode(target);

  ClearBackground(BLACK);

  scenes[current_scene_id]->draw();

  EndTextureMode();

  DrawTextureRec(target.texture, screen_rect, (Vector2){0, 0}, WHITE);

  // if (scenes[current_scene_id]->get_scene_transition() ==
  //     SCENE_TRANSITION_OUT) {
  //   handle_transition_out();
  // } else if (scenes[current_scene_id]->get_scene_transition() ==
  //            SCENE_TRANSITION_IN) {
  //   handle_transition_in();
  // }

  switch (scenes[current_scene_id]->get_scene_transition()) {
  case SCENE_TRANSITION_NONE:
    break;
  case SCENE_TRANSITION_IN:
    handle_transition_in();
    break;
  case SCENE_TRANSITION_OUT:
    handle_transition_out();
    break;
  default:
    break;
  }

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

  scenes[current_scene_id]->close();

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
