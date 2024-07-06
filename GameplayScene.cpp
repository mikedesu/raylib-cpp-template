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

void GameplayScene::update() {
  for (auto &s : get_sprites()) {
    if (s.second->get_type() == SPRITETYPE_PLAYER) {
      s.second->incr_ay(0.0032f);
      s.second->update();
      s.second->set_y(s.second->get_y() + s.second->get_vy());

      // lock the player to a relative "ground" location
      const int height = s.second->get_height();
      const int bottom_of_sprite = s.second->get_y() + height;
      if (bottom_of_sprite >= GetScreenHeight()) {
        s.second->set_y(GetScreenHeight() - height);
      }
    }
  }

  // we want the camera to follow the player in such a way that
  // the player is always in the center of the screen
  // except for the game beginning, when they begin flapping at the bottom
  // once they hit the center, the camera should follow them

  // if (sprites[player_id]->get_y() < 3 * GetScreenHeight() / 4) {
  // } else if (sprites[player_id]->get_y() > 3 * GetScreenHeight() / 4 &&
  //            camera2d.target.y < 0) {
  //   camera2d.target.y = 0;
  // }
  Camera2D &camera2d = get_camera2d();
  camera2d.target.y = get_sprites()[get_player_id()]->get_y();
  camera2d.offset.y = GetScreenHeight() / 2;
}

void GameplayScene::handle_input() {
  if (IsKeyPressed(KEY_D)) {
    flip_debug_panel();
  }

  if (IsKeyPressed(KEY_C)) {
    set_control_mode(CONTROL_MODE_CAMERA);
  }

  if (IsKeyPressed(KEY_SPACE)) {
    get_sprites()[get_player_id()]->set_ay(0.00f);

    // this value affects how high skull 'flaps' or jumps
    const float vy = -4.0f;
    // const float vy = -32.0f;
    //  eventually skull will be able to make big jumps
    //  by acquiring powerups to modify this value
    //  this will mean less spacebar presses or taps
    //  which will encourage players to get the powerup
    //  but for testing we can play with this value
    get_sprites()[get_player_id()]->set_vy(vy);
    get_sprites()[get_player_id()]->update();
  }

  if (IsKeyDown(KEY_LEFT)) {
    get_sprites()[get_player_id()]->set_x(
        get_sprites()[get_player_id()]->get_x() - 2.0f);

    if (!get_sprites()[get_player_id()]->get_is_flipped()) {
      get_sprites()[get_player_id()]->flip();
    }
  } else if (IsKeyDown(KEY_RIGHT)) {
    get_sprites()[get_player_id()]->set_x(
        get_sprites()[get_player_id()]->get_x() + 2.0f);
    if (get_sprites()[get_player_id()]->get_is_flipped()) {
      get_sprites()[get_player_id()]->flip();
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

    for (int i = 0; i < 1000; i++) {
      add_soulshard();
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

void GameplayScene::add_soulshard() {
  const int x = GetRandomValue(0, GetScreenWidth());
  const int y = GetRandomValue(-GetScreenHeight() * 100, GetScreenHeight());

  entity_id id =
      spawn_entity("soulshard", (float)x, (float)y, SPRITETYPE_SOULSHARD, true);

  // stars[next_entity_id] = (Vector2){(float)x, (float)y};
  // mPrint("Star added at: " + to_string(x) + ", " + to_string(y));
  // next_entity_id++;
}
