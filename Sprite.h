#pragma once

#include "raylib.h"

class Sprite {

public:
  Sprite(const char *filepath, const int anim_frames, const float x,
         const float y);
  Sprite(Texture2D &texture, const int anim_frames, const float x,
         const float y);

  ~Sprite();

private:
  Texture2D texture;
  Rectangle rect;
  Vector2 position;
  int anim_frames;
};
