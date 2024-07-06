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
  set_scene_transition(SCENE_TRANSITION_IN);
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
    entity_id title_id = spawn_entity("title", x, y, SPRITETYPE_PLAYER, false);

    set_has_been_initialized(true);
  }
  return true;
}

void TitleScene::draw_debug_panel() {
  string camera_info_str =
      "Current Frame: " + to_string(get_current_frame()) + "\n" +
      "Control mode: " + to_string(get_control_mode()) + "\n" +
      //"Player Position: " + to_string(sprites[player_id]->get_x()) + ", " +
      // to_string(sprites[player_id]->get_y()) + "\n" +
      "Camera target: " + to_string(get_camera2d().target.x) + ", " +
      to_string(get_camera2d().target.y) + "\n" + "TitleScene";
  //"Current Frame: " + to_string(current_frame) + "\n" +
  //"Camera2D target: " + to_string(camera2d.target.x) + ", " +
  // to_string(camera2d.target.y) + "\n" +
  //"Camera2D offset: " + to_string(camera2d.offset.x) + ", " +
  // to_string(camera2d.offset.y) + "\n" +
  //"Camera2D rotation: " + to_string(camera2d.rotation) + "\n" +
  //"Camera2D zoom: " + to_string(camera2d.zoom) + "\n" +
  //"Player Position: " + to_string(sprites[player_id]->get_x()) + ", " +
  // to_string(sprites[player_id]->get_y()) + "\n" +
  //"Player Velocity: " + to_string(sprites[player_id]->get_vx()) + ", " +
  // to_string(sprites[player_id]->get_vy()) + "\n" +
  //"Player Acceleration: " + to_string(sprites[player_id]->get_ax()) + ", " +
  // to_string(sprites[player_id]->get_ay()) + "\n" +
  DrawRectangle(0, 0, 500, 200, Fade(BLACK, 0.5f));
  DrawTextEx(get_global_font(), camera_info_str.c_str(), (Vector2){10, 10}, 16,
             0.5f, WHITE);
}
