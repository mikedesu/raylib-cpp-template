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

void Scene::update() {}

void Scene::handle_input() {}

void Scene::draw_stars() {
  for (auto &s : stars) {
    // DrawPixelV(s.second, WHITE);
    // cant see them so draw rectangles
    DrawRectangle(s.second.x, s.second.y, 4, 4, WHITE);
  }
}
void Scene::draw_ground() {
  // want a real ground sprite texture
  const int w = GetScreenWidth();
  const int h = GetScreenHeight();
  Color c = BROWN;
  DrawRectangle(0, GetScreenHeight() - 10, w, h, c);
}

void Scene::draw() {
  BeginMode2D(camera2d);

  Color clear_color = BLACK;

  switch (scenetype) {
  case SCENE_TYPE_TITLE:
    clear_color = BLACK;
    break;
  case SCENE_TYPE_GAMEPLAY:
    clear_color = (Color){0x10, 0x10, 0x10, 0xFF};
    break;
  }

  ClearBackground(clear_color);
  // no background yet, but lets mock one up with shapes
  // draw a large rectangle to represent a scene
  // but lets make the dimension ratio 720x1280
  // DrawRectangle(GetScreenWidth() / 2 - 405 / 2, 0, 405, 720, BLACK);

  // draw stars
  // want: real stars

  if (scenetype != SCENE_TYPE_TITLE) {

    draw_stars();
    draw_ground();
  }

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

bool Scene::init() { return true; }

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

void Scene::draw_debug_panel() {}

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

  const int player_starting_hp = 3;
  const int player_max_hp = 3;
  sprites[player_id]->set_maxhp(player_max_hp);
  sprites[player_id]->set_hp(player_starting_hp);

  return id;
}

entity_id Scene::spawn_bat(const float x, const float y) {
  mPrint("Spawning bat...");
  entity_id id = spawn_entity("bat", x, y, SPRITETYPE_ENEMY, true);
  sprites[id]->set_vx(2.0f);
  sprites[id]->set_vy(0.0f);
  sprites[id]->set_ax(0.0f);
  sprites[id]->set_ay(0.0f);
  sprites[id]->set_hp(1);
  sprites[id]->set_maxhp(1);
  return id;
}

entity_id Scene::spawn_knife() {
  mPrint("Spawning knife...");
  // calculate offsets
  // half the width of the sprite
  const float o_x = sprites[player_id]->get_width();
  const float o_y = sprites[player_id]->get_height() / 2.0;
  float x = sprites[player_id]->get_x();
  float y = sprites[player_id]->get_y() + o_y;
  // get the width of the knife texture
  const float knife_width = textures["knife"].texture.width;
  if (sprites[player_id]->get_is_flipped()) {
    x -= knife_width * global_scale;
  } else {
    x += o_x;
  }

  // spawn the knife
  entity_id id = spawn_entity("knife", x, y, SPRITETYPE_KNIFE, false);
  // set variables
  if (sprites[player_id]->get_is_flipped()) {
    sprites[id]->set_vx(-1);
    sprites[id]->set_is_flipped(true);
  } else {
    sprites[id]->set_vx(1);
  }

  sprites[id]->set_vy(0);
  sprites[id]->set_ax(0);
  sprites[id]->set_ay(0);
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
void Scene::set_scene_type(scene_type st) { scenetype = st; }
scene_type Scene::get_scene_type() { return scenetype; }
unordered_map<entity_id, Vector2> &Scene::get_stars() { return stars; }

void Scene::update_stars_vx(const float vx) {
  for (auto &s : stars) {
    s.second.x += vx;
    if (s.second.x < 0) {
      s.second.x = GetScreenWidth();
    }
  }
}
