#pragma once

#include "raylib.h"

typedef enum {
  SPRITETYPE_NONE,
  SPRITETYPE_PLAYER,
  SPRITETYPE_ENEMY,
  SPRITETYPE_BULLET,
  SPRITETYPE_KNIFE,
  SPRITETYPE_COUNT
} sprite_type;

class Sprite {

public:
  Sprite(const char *filepath, const int anim_frames, const float x,
         const float y, sprite_type t);
  Sprite(Texture2D &texture, const int anim_frames, const float x,
         const float y, sprite_type t);
  ~Sprite();

  void draw();
  void draw_hitbox();
  void move(const float x, const float y);
  void move_rect(Rectangle &r);
  void mark_for_deletion();

  const float get_scale() const;
  const float get_x() const;
  const float get_y() const;
  const Rectangle get_dest() const;
  const int get_width() const;
  const int get_height() const;
  const int get_anim_frames() const;
  const int get_current_frame() const;
  const float get_vx() const;
  const float get_vy() const;
  const Vector2 get_velocity() const;
  const Vector2 get_acceleration() const;
  const bool get_is_marked_for_deletion() const;
  const bool get_is_animating() const;
  const bool get_is_flipped() const;
  const sprite_type get_type() const;
  const float get_ax() const;
  const float get_ay() const;

  void incr_frame();
  void incr_ax(const float ax);
  void incr_ay(const float ay);
  void incr_vx(const float vx);
  void incr_vy(const float vy);

  void set_scale(const float scale);
  void set_current_frame(const int frame);
  void set_x(const float x);
  void set_y(const float y);
  void set_vx(const float vx);
  void set_vy(const float vy);
  void set_velocity(const Vector2 v);
  void set_acceleration(const Vector2 a);
  void set_is_animating(const bool is_animating);
  void set_is_flipped(const bool is_flipped);
  void set_type(const sprite_type t);
  void set_ax(const float ax);
  void set_ay(const float ay);

  void flip();

  void update();

private:
  Texture2D texture;
  Rectangle src;
  Rectangle flipped_src;
  Rectangle dest;
  Vector2 origin;
  int anim_frames;
  int current_frame;
  float scale;

  Vector2 velocity;
  Vector2 acceleration;

  bool is_marked_for_deletion;
  bool is_animating;
  bool is_flipped;

  sprite_type type;

  void init_rects();
};
