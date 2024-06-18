#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include "Sprite.h"

using std::string;
using std::unordered_map;
// using std::unique_ptr;
// using std::make_unique;
using std::make_shared;
using std::shared_ptr;

typedef int entity_id;

class Game {

public:
  Game();
  void run();
  void draw_debug_panel(Camera2D &camera, Font &font);
  ~Game();

private:
  unordered_map<string, Texture2D> textures;

  unordered_map<entity_id, shared_ptr<Sprite>> sprites;

  bool debug_panel_on;

  Font global_font;
  Camera2D camera;
  RenderTexture target;
  Rectangle screenRect;
};
