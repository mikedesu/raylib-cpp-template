#pragma once

#include "Sprite.h"
#include <memory>
#include <string>
#include <unordered_map>

using std::make_shared;
using std::shared_ptr;
using std::string;
using std::unordered_map;

typedef int entity_id;

typedef struct {
  Texture2D texture;
  int num_frames;
  int is_player;
  string asset_path;
} texture_info;

class Game {
public:
  Game();
  void run();
  void draw_sprite(entity_id id);
  void draw();
  void handle_input();
  void handle_sprite_movement(entity_id id);
  void draw_debug_panel(Camera2D &camera, Font &font);
  void load_fonts();
  void load_assets();
  void spawn_player();
  void spawn_knife();
  void update();
  void spawn_bat();
  void handle_sprites();
  void handle_player_movement();
  void handle_knife_movement();
  void handle_enemy_movement();
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
  void load_texture(const char *asset_name);
  void add_texture_to_load(const char *asset_name, const char *asset_path,
                           int num_frames, int is_player);

  ~Game();

private:
  // unordered_map<string, Texture2D> textures;
  unordered_map<string, texture_info> textures;

  unordered_map<entity_id, shared_ptr<Sprite>> sprites;
  bool debug_panel_on;
  bool has_been_initialized;
  Font global_font;
  Camera2D camera;
  RenderTexture target;
  Rectangle screen_rect;
  entity_id player_id;
  float global_scale;

  unsigned int current_frame;

  string window_title;
};
