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

class Game {
public:
  Game();
  void run();
  void draw();
  void handle_input();
  void draw_debug_panel(Camera2D &camera, Font &font);
  void load_fonts();
  void load_assets();
  ~Game();

private:
  unordered_map<string, Texture2D> textures;
  unordered_map<entity_id, shared_ptr<Sprite>> sprites;
  bool debug_panel_on;
  Font global_font;
  Camera2D camera;
  RenderTexture target;
  Rectangle screenRect;
  entity_id player_id;
};
