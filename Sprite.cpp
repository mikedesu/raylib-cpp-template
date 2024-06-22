#include "Sprite.h"

Sprite::Sprite(const char *filepath, const int anim_frames, const float x,
               const float y, SpriteType t) {
  texture = LoadTexture(filepath);
  this->anim_frames = anim_frames;
  set_scale(1.0f);
  init_rects();
  origin = (Vector2){0, 0};
  this->dest.x = x;
  this->dest.y = y;
  current_frame = 0;
  velocity = (Vector2){0, 0};
  is_marked_for_deletion = false;
  is_animating = false;
  is_flipped = false;
  type = t;
}

Sprite::Sprite(Texture2D &texture, const int anim_frames, const float x,
               const float y, SpriteType t) {
  this->texture = texture;
  this->anim_frames = anim_frames;
  set_scale(1.0f);
  init_rects();
  origin = (Vector2){0, 0};
  this->dest.x = x;
  this->dest.y = y;
  current_frame = 0;
  velocity = (Vector2){0, 0};
  is_marked_for_deletion = false;
  is_animating = false;
  is_flipped = false;
  type = t;
}

void Sprite::set_is_animating(const bool is_animating) {
  this->is_animating = is_animating;
}

bool Sprite::get_is_animating() const { return is_animating; }

void Sprite::init_rects() {
  src = (Rectangle){0, 0, (float)texture.width / anim_frames,
                    (float)texture.height};

  flipped_src = (Rectangle){src.x, src.y, src.width * -1.0f, src.height};

  dest =
      (Rectangle){dest.x, dest.y, ((float)texture.width / anim_frames) * scale,
                  (float)texture.height * scale};
}

Sprite::~Sprite() {}

void Sprite::draw() {
  DrawTexturePro(texture, is_flipped ? flipped_src : src, dest, origin, 0,
                 WHITE);
}

void Sprite::draw_hitbox() {
  DrawRectangleLines(dest.x, dest.y, dest.width, dest.height, RED);
}

void Sprite::move_pos_x(const float x) { dest.x += x * scale; }
void Sprite::move_pos_y(const float y) { dest.y += y * scale; }

void Sprite::move_rect(Rectangle &r) {
  dest.x = r.x;
  dest.y = r.y;
}

void Sprite::set_scale(const float scale) {
  this->scale = scale;
  init_rects();
}

float Sprite::get_x() const { return dest.x; }
float Sprite::get_y() const { return dest.y; };

Rectangle Sprite::get_dest() const { return dest; }

int Sprite::get_width() const { return src.width * scale; }

int Sprite::get_height() const { return src.height * scale; }

int Sprite::get_anim_frames() const { return anim_frames; }

int Sprite::get_current_frame() const { return current_frame; }

void Sprite::set_current_frame(const int frame) {
  if (frame < 0 || frame >= anim_frames) {
    return;
  }

  current_frame = frame;

  // update the src rectangle
  src.x = (float)frame * src.width;
  src.y = 0;
}

void Sprite::incr_frame() {
  current_frame++;
  if (current_frame >= anim_frames) {
    current_frame = 0;
  }

  src.x = (float)current_frame * src.width;
}

float Sprite::get_scale() const { return scale; }

void Sprite::set_vx(const float vx) { velocity.x = vx; }

void Sprite::set_vy(const float vy) { velocity.y = vy; }

int Sprite::get_vx() const { return velocity.x; }

int Sprite::get_vy() const { return velocity.y; }

bool Sprite::get_is_marked_for_deletion() const {
  return is_marked_for_deletion;
}

void Sprite::mark_for_deletion() { is_marked_for_deletion = true; }

void Sprite::set_is_flipped(const bool is_flipped) {

  this->is_flipped = is_flipped;
}

bool Sprite::get_is_flipped() const { return is_flipped; }

SpriteType Sprite::get_type() const { return type; }

void Sprite::set_type(const SpriteType t) { type = t; }

void Sprite::set_x(const float x) { dest.x = x; }
void Sprite::set_y(const float y) { dest.y = y; }
