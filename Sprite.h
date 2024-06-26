#pragma once

#include "raylib.h"

typedef enum {
  SPRITETYPE_NONE,
  SPRITETYPE_PLAYER,
  SPRITETYPE_ENEMY,
  SPRITETYPE_BULLET,
  SPRITETYPE_KNIFE,
  SPRITETYPE_COUNT
} SpriteType;

class Sprite {

public:
  Sprite(const char *filepath, const int anim_frames, const float x,
         const float y, SpriteType t);
  Sprite(Texture2D &texture, const int anim_frames, const float x,
         const float y, SpriteType t);
  ~Sprite();

  void draw();
  void draw_hitbox();
  void move_pos_x(const float x);
  void move_pos_y(const float y);
  void move_rect(Rectangle &r);
  void set_scale(const float scale);
  float get_scale() const;

  float get_x() const;
  float get_y() const;
  void set_x(const float x);
  void set_y(const float y);
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
  void set_is_animating(const bool is_animating);
  bool get_is_animating() const;
  void incr_frame();

  void set_is_flipped(const bool is_flipped);
  bool get_is_flipped() const;

  SpriteType get_type() const;
  void set_type(const SpriteType t);

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
  bool is_marked_for_deletion;
  bool is_animating;
  bool is_flipped;

  SpriteType type;

  void init_rects();
};
