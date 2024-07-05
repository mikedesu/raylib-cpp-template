#include "TitleScene.h"

#include "mPrint.h"
// #include <cassert>
// #include <cstring>

static entity_id next_entity_id = 0;

TitleScene::TitleScene() {
  mPrint("TitleScene constructor");
  set_control_mode(CONTROL_MODE_PLAYER);
  set_texture_filepath("title_textures.txt");
  set_global_scale(32.0f);
  set_scene_transition(SCENE_TRANSITION_NONE);
}

TitleScene::~TitleScene() { mPrint("TitleScene destructor"); }

void TitleScene::update() {}

void TitleScene::handle_input() {
  if (IsKeyPressed(KEY_D)) {
    flip_debug_panel();
  } else if (IsKeyPressed(KEY_SPACE)) {
    set_scene_transition(SCENE_TRANSITION_OUT);
  }
}

bool TitleScene::init() {
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

    const int w = get_textures()["title"].texture.width;
    const int h = get_textures()["title"].texture.height;
    const int off_w = 0;
    const int off_h = h / 2;

    const int x = w / 8 * get_global_scale();
    const int y = GetScreenHeight() / 4;
    entity_id title_id = spawn_entity("title", x, y, SPRITETYPE_PLAYER);

    set_has_been_initialized(true);
  }
  return true;
}
