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
  ~Game();

private:
  // unordered_map<string, Texture2D> textures;
  unordered_map<string, texture_info> textures;

  unordered_map<entity_id, shared_ptr<Sprite>> sprites;
  bool debug_panel_on;
  Font global_font;
  Camera2D camera;
  RenderTexture target;
  Rectangle screenRect;
  entity_id player_id;
  float global_scale;

  unsigned int current_frame;
};
