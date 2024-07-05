#include "GameplayScene.h"
#include "mPrint.h"

static entity_id next_entity_id = 0;

GameplayScene::GameplayScene() {
  mPrint("GameplayScene constructor");
  set_control_mode(CONTROL_MODE_PLAYER);
  set_texture_filepath("game_textures.txt");
  set_global_scale(4.0f);
}

GameplayScene::~GameplayScene() { mPrint("GameplayScene destructor"); }

void GameplayScene::update() {}

void GameplayScene::handle_input() {
  if (IsKeyPressed(KEY_D)) {
    flip_debug_panel();
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

    set_has_been_initialized(true);
  }
  return true;
}
