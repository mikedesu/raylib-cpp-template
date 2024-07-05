#include "GameplayScene.h"
#include "mPrint.h"

static entity_id next_entity_id = 0;

GameplayScene::GameplayScene() {
  mPrint("GameplayScene constructor");
  set_control_mode(CONTROL_MODE_PLAYER);
  set_texture_filepath("game_textures.txt");
  set_global_scale(4.0f);
  set_scene_transition(SCENE_TRANSITION_IN);
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

void GameplayScene::draw_debug_panel() {
  string camera_info_str =
      "Current Frame: " + to_string(get_current_frame()) + "\n" +
      "Control mode: " + to_string(get_control_mode()) + "\n" +
      //"Player Position: " + to_string(sprites[player_id]->get_x()) + ", " +
      // to_string(sprites[player_id]->get_y()) + "\n" +
      "Camera target: " + to_string(get_camera2d().target.x) + ", " +
      to_string(get_camera2d().target.y) + "\n" + "GameplayScene";
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
