#include "Sprite.h"

Sprite::Sprite(const char *filepath, const int anim_frames, const float x,
               const float y) {
  texture = LoadTexture(filepath);
  this->anim_frames = anim_frames;
  set_scale(1.0f);
  init_rects();
  origin = (Vector2){0, 0};
}

Sprite::Sprite(Texture2D &texture, const int anim_frames, const float x,
               const float y) {
  this->texture = texture;
  this->anim_frames = anim_frames;
  set_scale(1.0f);
  init_rects();
  origin = (Vector2){0, 0};
}

void Sprite::init_rects() {
  src = (Rectangle){0, 0, (float)texture.width / anim_frames,
                    (float)texture.height};
  dest = (Rectangle){0, 0, ((float)texture.width / anim_frames) * scale,
                     (float)texture.height * scale};
}

Sprite::~Sprite() {}

void Sprite::draw() { DrawTexturePro(texture, src, dest, origin, 0, WHITE); }

void Sprite::draw_hitbox() {
  DrawRectangleLines(dest.x, dest.y, dest.width, dest.height, RED);
}

void Sprite::move_pos_x(const int x) { dest.x += x * scale; }
void Sprite::move_pos_y(const int y) { dest.y += y * scale; }

void Sprite::move_rect(Rectangle &r) {
  dest.x = r.x;
  dest.y = r.y;
}

void Sprite::set_scale(const float scale) {
  this->scale = scale;
  init_rects();
}

int Sprite::get_x() const { return dest.x; }
int Sprite::get_y() const { return dest.y; };

Rectangle Sprite::get_dest() const { return dest; }
