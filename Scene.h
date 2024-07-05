#pragma once

#include "Sprite.h"
#include "control_mode.h"
#include "entity_id.h"
#include "raylib.h"
#include "texture_info.h"
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

using std::make_shared;
using std::shared_ptr;
using std::string;
using std::unordered_map;
using std::vector;

class Scene {
public:
  Scene();
  ~Scene();

  void update();
  void draw();

  void init();
  bool load_texture(const char *asset_name, const char *asset_path,
                    const int num_frames, const int is_player);

  bool load_textures();
  void set_camera_default_values();

  bool get_debug_panel_on();
  void set_debug_panel_on(bool b);
  void flip_debug_panel();
  void draw_debug_panel();

  void set_global_scale(float s);
  void handle_input();
  void handle_cam_input();
  void handle_player_input();
  void load_fonts();

  entity_id spawn_entity(const char *texture_key, float x, float y,
                         sprite_type type);
  entity_id spawn_player(float x, float y);

private:
  unordered_map<string, texture_info> textures;
  unordered_map<entity_id, shared_ptr<Sprite>> sprites;
  vector<entity_id> entity_ids;
  unordered_map<entity_id, bool> gravity;

  float global_scale = 1.0f;
  bool has_been_initialized = false;
  entity_id player_id = -1;
  Camera2D camera2d;
  bool debug_panel_on = true;
  Font global_font;
  control_mode controlmode;
  unsigned int current_frame = 0;
};
