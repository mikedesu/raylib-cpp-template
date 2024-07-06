#include "Scene.h"
#include "mPrint.h"
#include <cassert>
#include <cstring>

static entity_id next_entity_id = 0;

Scene::Scene() {
  mPrint("Scene constructor");
  controlmode = CONTROL_MODE_PLAYER;
  texture_filepath = "";
  debug_panel_on = false;
}

void Scene::set_texture_filepath(const char *filepath) {
  texture_filepath = filepath;
}

Scene::~Scene() { mPrint("Scene destructor"); }

void Scene::close() {
  mPrint("Closing scene...");
  mPrint("Unloading textures...");
  for (auto &t : textures) {
    UnloadTexture(t.second.texture);
  }
  mPrint("Clearing textures...");
  textures.clear();
  mPrint("Clearing sprites...");
  sprites.clear();
  UnloadFont(global_font);
}

void Scene::update() {
  for (auto &s : sprites) {
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
  camera2d.target.y = sprites[player_id]->get_y();
  camera2d.offset.y = GetScreenHeight() / 2;
}

void Scene::handle_cam_input() {
  if (IsKeyPressed(KEY_C)) {
    controlmode = CONTROL_MODE_PLAYER;
  }

  if (IsKeyDown(KEY_LEFT)) {
    camera2d.target.x -= 2.0f;
  } else if (IsKeyDown(KEY_RIGHT)) {
    camera2d.target.x += 2.0f;
  }

  if (IsKeyDown(KEY_UP)) {
    camera2d.target.y -= 2.0f;
  } else if (IsKeyDown(KEY_DOWN)) {
    camera2d.target.y += 2.0f;
  }
}

void Scene::handle_player_input() {
  if (IsKeyPressed(KEY_C)) {
    controlmode = CONTROL_MODE_CAMERA;
  }

  if (IsKeyPressed(KEY_SPACE)) {
    sprites[player_id]->set_ay(0.00f);

    // this value affects how high skull 'flaps' or jumps
    const float vy = -4.0f;
    // const float vy = -32.0f;
    //  eventually skull will be able to make big jumps
    //  by acquiring powerups to modify this value
    //  this will mean less spacebar presses or taps
    //  which will encourage players to get the powerup
    //  but for testing we can play with this value
    sprites[player_id]->set_vy(vy);
    sprites[player_id]->update();
  }

  if (IsKeyDown(KEY_LEFT)) {
    sprites[player_id]->set_x(sprites[player_id]->get_x() - 2.0f);
    if (!sprites[player_id]->get_is_flipped()) {
      sprites[player_id]->flip();
    }
  } else if (IsKeyDown(KEY_RIGHT)) {
    sprites[player_id]->set_x(sprites[player_id]->get_x() + 2.0f);
    if (sprites[player_id]->get_is_flipped()) {
      sprites[player_id]->flip();
    }
  }
}

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
  ClearBackground(BLACK);
  // no background yet, but lets mock one up with shapes
  // draw a large rectangle to represent a scene
  // but lets make the dimension ratio 720x1280
  // DrawRectangle(GetScreenWidth() / 2 - 405 / 2, 0, 405, 720, BLACK);

  // draw stars
  // want: real stars
  // for (auto &s : stars) {
  // DrawPixelV(s.second, WHITE);
  // cant see them so draw rectangles
  //    DrawRectangle(s.second.x, s.second.y, 2, 2, WHITE);
  //  }

  // want a real ground sprite texture
  // DrawRectangle(0, GetScreenHeight() - 10, GetScreenWidth(),
  // GetScreenHeight(),
  //              BROWN);

  for (auto &s : sprites) {
    s.second->draw();
    if (debug_panel_on) {
      s.second->draw_hitbox();
    }
  }
  EndMode2D();

  // draw debug panel
  if (debug_panel_on) {
    DrawFPS(GetScreenWidth() - 80, 10);
    draw_debug_panel();
  }
  current_frame++;
}

bool Scene::init() {
  if (!has_been_initialized) {
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
    const int sprite_width = textures["skull"].texture.width;
    const int sprite_height = textures["skull"].texture.height;
    const float x = (float)GetScreenWidth() / 2 - (float)sprite_width;
    const float y = (float)GetScreenHeight() / 2 - (float)sprite_height;
    spawn_player(x, y);

    // for (int i = 0; i < 1000; i++) {
    //   add_star();
    // }

    has_been_initialized = true;
  }
  return true;
}

bool Scene::load_textures() {
  mPrint("Loading textures...");
  // check if the file exists
  if (texture_filepath == "") {
    mPrint("Error: texture_filepath not set.");
    return false;
  }
  FILE *file = fopen(texture_filepath.c_str(), "r");
  if (file == NULL) {
    mPrint("Error opening file: " + texture_filepath);
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
    mPrint("Asset path: [" + string(asset_path) + "]");

    // check to see if asset path has newline at end
    if (asset_path[strlen(asset_path) - 1] == '\n') {
      asset_path[strlen(asset_path) - 1] = '\0';
    }

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

void Scene::draw_debug_panel() {
  string camera_info_str =
      "Current Frame: " + to_string(current_frame) + "\n" +

      "Control mode: " + to_string(controlmode) + "\n" +
      //"Player Position: " + to_string(sprites[player_id]->get_x()) + ", " +
      // to_string(sprites[player_id]->get_y()) + "\n" +
      "Camera target: " + to_string(camera2d.target.x) + ", " +
      to_string(camera2d.target.y) + "\n";
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
  entity_id id = spawn_entity("skull", x, y, SPRITETYPE_PLAYER, false);
  player_id = id;
  return id;
}

entity_id Scene::spawn_entity(const char *texture_key, float x, float y,
                              sprite_type type, bool is_anim) {
  mPrint("Spawning entity...");
  shared_ptr<Sprite> s =
      make_shared<Sprite>(textures[texture_key].texture,
                          textures[texture_key].num_frames, x, y, type);
  if (s == nullptr) {
    mPrint("Error creating sprite.");
    return -1;
  }
  s->set_scale(global_scale);
  s->set_is_animating(is_anim);

  sprites[next_entity_id] = s;
  return next_entity_id++;
}

bool Scene::get_debug_panel_on() { return debug_panel_on; }
void Scene::set_debug_panel_on(bool b) { debug_panel_on = b; }
void Scene::flip_debug_panel() { debug_panel_on = !debug_panel_on; }

void Scene::add_star() {
  const int x = GetRandomValue(0, GetScreenWidth());
  const int y = GetRandomValue(-GetScreenHeight() * 100, GetScreenHeight());
  stars[next_entity_id] = (Vector2){(float)x, (float)y};
  mPrint("Star added at: " + to_string(x) + ", " + to_string(y));
  next_entity_id++;
}

void Scene::set_control_mode(control_mode cm) { controlmode = cm; }

bool Scene::get_has_been_initialized() { return has_been_initialized; }

void Scene::set_has_been_initialized(bool b) { has_been_initialized = b; }

unordered_map<entity_id, shared_ptr<Sprite>> &Scene::get_sprites() {
  return sprites;
}

unordered_map<string, texture_info> &Scene::get_textures() { return textures; }

float Scene::get_global_scale() { return global_scale; }

void Scene::set_scene_transition(scene_transition st) { transition = st; }

scene_transition Scene::get_scene_transition() { return transition; }

void Scene::set_alpha(float a) { alpha = a; }

const float Scene::get_alpha() const { return alpha; }

void Scene::set_id(scene_id i) { id = i; }

scene_id Scene::get_id() { return id; }

Camera2D &Scene::get_camera2d() { return camera2d; }

unsigned int Scene::get_current_frame() { return current_frame; }

control_mode Scene::get_control_mode() { return controlmode; }

Font &Scene::get_global_font() { return global_font; }

void Scene::set_player_id(entity_id id) { player_id = id; }

entity_id Scene::get_player_id() { return player_id; }
