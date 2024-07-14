#pragma once

#include "Scene.h"

class GameplayScene : public Scene {
public:
  GameplayScene();
  ~GameplayScene();

  void update();
  bool init();
  void handle_input();
  void draw_debug_panel();
  void cleanup();

  void update_player_movement();
  void update_knife_movement();
  void update_enemy_movement();
  void handle_player_collision();
  void handle_offscreen();
  void handle_knife_collisions();

private:
  const float gravity = 0.0032f;

  Mix_Chunk *sfx_knife_throw = nullptr;
  Mix_Chunk *sfx_knife_hit = nullptr;
};
