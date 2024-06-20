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
  void move_rect(Rectangle &r);
  void set_scale(const float scale);
  float get_scale() const;

  int get_x() const;
  int get_y() const;
  Rectangle get_dest() const;
  int get_width() const;
  int get_height() const;
  int get_anim_frames() const;
  int get_current_frame() const;
  void set_current_frame(const int frame);

  void set_vx(const float vx);
  void set_vy(const float vy);
  int get_vx() const;
  int get_vy() const;

  void mark_for_deletion();
  bool get_is_marked_for_deletion() const;

private:
  Texture2D texture;
  Rectangle src;
  Rectangle dest;
  Vector2 origin;
  int anim_frames;
  int current_frame;
  float scale;

  Vector2 velocity;

  bool is_marked_for_deletion;

  void init_rects();
};
