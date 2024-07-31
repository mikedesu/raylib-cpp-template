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
  void draw_hud();
  void draw();

  void close();

  void cleanup();
  void gameover();
  inline void handle_draw_debug_panel();

  void update_player_movement();
  void update_knife_movement();
  void update_enemy_movement();
  void handle_player_collision();
  void handle_offscreen();
  void handle_knife_collisions();
  void draw_ground();
  void draw_stars();

  entity_id spawn_bat(const float x, const float y, const float vx);
  entity_id spawn_bat();

  void set_ground_y_movement(const float dy);

  void set_do_ground_movement(const bool d);

private:
  const float gravity = 0.0032f;
  // const float gravity = 0.0064f;

  Mix_Chunk *sfx_knife_throw = nullptr;
  Mix_Chunk *sfx_knife_hit = nullptr;

  unsigned int enemies_killed = 0;

  int ground_y = 0;

  bool show_test_popup = false;

  bool do_ground_movement = true;
  float ground_y_movement = -1;

  // bool line_did_cross_line(Vector4 &line1, Vector4 &line2);
  // bool ccw(float x1, float y1, float x2, float y2, float x3, float y3);
};
