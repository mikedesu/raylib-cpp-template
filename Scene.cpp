#include "Scene.h"
#include "mPrint.h"
#include <cassert>
#include <cstring>

static entity_id next_entity_id = 0;

Scene::Scene() {
  mPrint("Scene constructor");
  // textures = {};
  //  sprites = {};
  //  entity_ids = {};
  //  gravity = {};
}

Scene::~Scene() {
  mPrint("Scene destructor");
  // textures.clear();
  // sprites.clear();
  // entity_ids.clear();
  // gravity.clear();
}

void Scene::update() {
  for (auto &s : sprites) {
    if (s.second->get_type() == SPRITETYPE_PLAYER) {
      s.second->incr_ay(0.0032f);
      s.second->update();
    }
  }
}
void Scene::handle_cam_input() {}

void Scene::handle_player_input() {}

void Scene::handle_input() {
  if (IsKeyPressed(KEY_D)) {
    debug_panel_on = !debug_panel_on;
  }

  switch (controlmode) {
  case CONTROL_MODE_CAMERA:
    handle_cam_input();
    break;
  case CONTROL_MODE_PLAYER:
    handle_player_input();
    break;
  default:
    break;
  }
}

void Scene::draw() {
  BeginMode2D(camera2d);
  ClearBackground(BROWN);
  // no background yet, but lets mock one up with shapes
  // draw a large rectangle to represent a scene
  // but lets make the dimension ratio 720x1280
  DrawRectangle(GetScreenWidth() / 2 - 405 / 2, 0, 405, 720, BLACK);

  for (auto &s : sprites) {
    s.second->draw();
    if (debug_panel_on) {
      s.second->draw_hitbox();
    }
  }

  // draw debug panel
  if (debug_panel_on) {
    DrawFPS(10, 10);
    draw_debug_panel();
  }

  EndMode2D();

  current_frame++;
}

void Scene::init() {
  if (!has_been_initialized) {

    mPrint("Initializing scene...");
    // mPrint("Initializing window...");
    // InitWindow(screen_rect.width, -screen_rect.height,
    //            get_window_title().c_str());
    mPrint("Initializing camera...");
    set_camera_default_values();

    // SetTargetFPS(60); // Set our game to run at 60 frames-per-second
    mPrint("Loading assets...");
    load_fonts();

    bool result = load_textures();
    if (!result) {
      mPrint("Error loading textures. Exiting...");
      return;
    }

    // mPrint("Loading render texture...");
    // target = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());
    // mPrint("Setting exit key...");
    // SetExitKey(KEY_Q);
    // spawn_player(0, 0);

    const int sprite_width = textures["skull"].texture.width;
    const int sprite_height = textures["skull"].texture.height;
    const float x = (float)GetScreenWidth() / 2 - (float)sprite_width;
    const float y = (float)GetScreenHeight() / 2 - (float)sprite_height;
    spawn_player(x, y);

    has_been_initialized = true;
  }
}

bool Scene::load_textures() {
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

bool Scene::load_texture(const char *asset_name, const char *asset_path,
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

void Scene::set_camera_default_values() {
  camera2d.target.x = 0;
  camera2d.target.y = 0;
  camera2d.offset.x = 0;
  camera2d.offset.y = 0;
  camera2d.rotation = 0.0f;
  camera2d.zoom = 1;
}

bool Scene::get_debug_panel_on() { return debug_panel_on; }
void Scene::set_debug_panel_on(bool b) { debug_panel_on = b; }
void Scene::flip_debug_panel() { debug_panel_on = !debug_panel_on; }

void Scene::draw_debug_panel() {
  string camera_info_str = "Current Frame: " + to_string(current_frame) + "\n";
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
  //"Control mode: " + to_string(controlmode) + "\n";

  DrawRectangle(0, 0, 500, 200, Fade(BLACK, 0.5f));
  DrawTextEx(global_font, camera_info_str.c_str(), (Vector2){10, 10}, 16, 0.5f,
             WHITE);
}

void Scene::set_global_scale(float s) {
  assert(s > 0);
  global_scale = s;
}

void Scene::load_fonts() {
  mPrint("Loading fonts...");
  const char font_path[] = "fonts/hack.ttf";
  global_font = LoadFont(font_path);
}

entity_id Scene::spawn_player(float x, float y) {
  mPrint("Attempting to spawn player...");
  if (player_id != -1) {
    mPrint("Player already spawned.");
    return player_id;
  }

  mPrint("Spawning player...");
  entity_id id = spawn_entity("skull", x, y, SPRITETYPE_PLAYER);
  player_id = id;
  return id;
}

entity_id Scene::spawn_entity(const char *texture_key, float x, float y,
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
