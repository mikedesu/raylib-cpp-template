#pragma once

#include "raylib.h"

class Sprite {

public:
  Sprite(const char *filepath, const int anim_frames, const float x,
         const float y);
  Sprite(Texture2D &texture, const int anim_frames, const float x,
         const float y);
  ~Sprite();

  void draw();
  void draw_hitbox();
  void move_pos_x(const int x);
  void move_pos_y(const int y);
  void set_scale(const float scale);

private:
  Texture2D texture;
  Rectangle src;
  Rectangle dest;
  Vector2 position;
  Vector2 origin;
  int anim_frames;
  float scale;

  void init_rects();
};
