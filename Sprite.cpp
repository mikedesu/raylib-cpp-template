#include "Sprite.h"

Sprite::Sprite(const char *filepath, const int anim_frames, const float x,
               const float y) {
  texture = LoadTexture(filepath);
  rect = (Rectangle){0, 0, (float)texture.width / anim_frames,
                     (float)texture.height};
  position = (Vector2){x, y};
  this->anim_frames = anim_frames;
}

Sprite::Sprite(Texture2D &texture, const int anim_frames, const float x,
               const float y) {
  this->texture = texture;
  rect = (Rectangle){0, 0, (float)texture.width / anim_frames,
                     (float)texture.height};
  position = (Vector2){x, y};
  this->anim_frames = anim_frames;
}

Sprite::~Sprite() {}
