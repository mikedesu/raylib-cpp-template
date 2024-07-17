#pragma once

#include "Scene.h"

#include <box2d/box2d.h>

class GameplayScene : public Scene {
public:
  GameplayScene();
  ~GameplayScene();

  void draw();
  void update();
  bool init();
  void handle_input();

  void draw_debug_panel();
  void draw_hud();

  void cleanup();

  void update_player_movement();
  void update_knife_movement();
  void update_enemy_movement();
  void handle_player_collision();
  void handle_offscreen();
  void handle_knife_collisions();

  entity_id spawn_player(float x, float y);
  void draw_ground();
  void create_solid_ground(const float x, const float y, const float w,
                           const float h);

private:
  // const float gravity = 0.0032f;

  float timeStep = 1.0f / 60.0f;
  int velocityIterations = 8;
  int positionIterations = 3;

  // Define the gravity vector.
  b2Vec2 gravity;
  b2World *world = nullptr;

  // b2Body *groundBody = world.CreateBody(&groundBodyDef);
  b2Body *groundBody = nullptr;

  vector<b2Body *> bodies;

  // const float gravity = 0.0064f;

  Mix_Chunk *sfx_knife_throw = nullptr;
  Mix_Chunk *sfx_knife_hit = nullptr;

  unsigned int enemies_killed = 0;

  // bool line_did_cross_line(Vector4 &line1, Vector4 &line2);
  // bool ccw(float x1, float y1, float x2, float y2, float x3, float y3);
};
