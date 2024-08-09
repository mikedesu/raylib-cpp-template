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
  // void update_knife_movement();
  void update_enemy_movement();
  void handle_player_collision();
  // void handle_offscreen();
  // void handle_knife_collisions();
  // void draw_ground();
  // void draw_stars();

  entity_id spawn_player(float x, float y);
  // entity_id spawn_knife();
  // entity_id spawn_bat(const float x, const float y, const float vx);
  // entity_id spawn_bat();
  // entity_id spawn_soulshard(const float x, const float y);
  // entity_id spawn_heart(float x, float y);

  // void set_ground_y_movement(const float dy);

  // void set_do_ground_movement(const bool d);
  // void handle_knife_recovery();

  // void set_knife_speed(const Vector2 speed);
  // void set_knife_catches(const unsigned int catches);
  // const Vector2 get_starting_knife_speed() const;
  // const Vector2 get_knife_speed() const;
  // const unsigned int get_knife_catches() const;

private:
  //  const float gravity = 0.0032f;
  // const float gravity = 0.0064f;

  // vector<Rectangle> damage_zones;

  // Mix_Chunk *sfx_knife_throw = nullptr;
  // Mix_Chunk *sfx_knife_hit = nullptr;

  // unsigned int enemies_killed = 0;

  // int ground_y = 0;

  // bool show_test_popup = false;
  bool show_test_popup = true;

  // bool do_ground_movement = false;
  //  bool do_ground_movement = true;
  // float ground_y_movement = -1;

  // const Vector2 starting_knife_speed = {2, 0};
  // Vector2 knife_speed = {2, 0};
  // unsigned int knife_catches = 0;

  // unsigned int max_knives = 2;
  // unsigned int current_knives = 2;

  entity_id player_id = -1;
  // unsigned int soulshard_catches = 0;

  // void init_damage_zones();
};
