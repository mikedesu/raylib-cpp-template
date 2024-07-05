#pragma once

#include "Scene.h"
#include "Sprite.h"
#include "control_mode.h"
#include "entity_id.h"
#include "texture_info.h"
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

using std::function;
using std::make_shared;
using std::shared_ptr;
using std::string;
using std::unordered_map;
using std::vector;

class Game {
public:
  Game();
  void run();
  // void draw_sprite(entity_id id);
  void update();
  void draw();
  void handle_input();
  void handle_camera_input();
  void handle_player_input();
  void draw_debug_panel();
  void load_fonts();
  void set_global_scale(float s);
  void set_debug_panel(bool b);
  void set_screen_width(int w);
  void set_screen_height(int h);
  void set_window_title(const char *t);
  string get_window_title();
  void set_camera_default_values();
  void set_has_been_initialized(bool b);
  bool get_has_been_initialized();
  void init();
  void close();

  // bool load_texture(const char *asset_name, const char *asset_path,
  //                   const int num_frames, const int is_player);

  // bool load_textures();
  //  void add_texture_to_load(const char *asset_name, const char *asset_path,
  //                           int num_frames, int is_player);

  // entity_id spawn_entity(const char *texture_key, float x, float y,
  //                        sprite_type type);
  // entity_id spawn_player(float x, float y);

  ~Game();

private:
  unordered_map<string, texture_info> textures;
  unordered_map<entity_id, shared_ptr<Sprite>> sprites;
  vector<entity_id> entity_ids;
  unordered_map<entity_id, bool> gravity;

  float global_scale;
  entity_id player_id;
  Font global_font;
  bool has_been_initialized;
  bool debug_panel_on;
  unsigned int current_frame;
  control_mode controlmode;
  string window_title;

  Camera2D camera2d;
  RenderTexture target;
  Rectangle screen_rect;

  Scene scene;
};
