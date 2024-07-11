#include "Sprite.h"

Sprite::Sprite(const char *filepath, const int anim_frames, const float x,
               const float y, sprite_type t) {
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
  is_spinning = false;
}

Sprite::Sprite(Texture2D &texture, const int anim_frames, const float x,
               const float y, sprite_type t) {
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
  is_spinning = false;
}

void Sprite::mark_for_deletion() { is_marked_for_deletion = true; }

void Sprite::set_ax(const float ax) { acceleration.x = ax; }
void Sprite::set_ay(const float ay) { acceleration.y = ay; }
void Sprite::set_vx(const float vx) { velocity.x = vx; }
void Sprite::set_vy(const float vy) { velocity.y = vy; }
void Sprite::set_type(const sprite_type t) { type = t; }
void Sprite::set_x(const float x) { dest.x = x; }
void Sprite::set_y(const float y) { dest.y = y; }
void Sprite::set_velocity(const Vector2 v) { velocity = v; }
void Sprite::set_acceleration(const Vector2 a) { acceleration = a; }

void Sprite::incr_vx(const float vx) { velocity.x += vx; }
void Sprite::incr_vy(const float vy) { velocity.y += vy; }
void Sprite::incr_ax(const float ax) { acceleration.x += ax; }
void Sprite::incr_ay(const float ay) { acceleration.y += ay; }

const float Sprite::get_ax() const { return acceleration.x; }
const float Sprite::get_ay() const { return acceleration.y; }
const float Sprite::get_scale() const { return scale; }
const float Sprite::get_vx() const { return velocity.x; }
const float Sprite::get_vy() const { return velocity.y; }
const bool Sprite::get_is_flipped() const { return is_flipped; }
const sprite_type Sprite::get_type() const { return type; }
const Vector2 Sprite::get_velocity() const { return velocity; }
const Vector2 Sprite::get_acceleration() const { return acceleration; }
const float Sprite::get_x() const { return dest.x; }
const float Sprite::get_y() const { return dest.y; };
const Rectangle Sprite::get_dest() const { return dest; }
const int Sprite::get_width() const { return src.width * scale; }
const int Sprite::get_height() const { return src.height * scale; }
const int Sprite::get_anim_frames() const { return anim_frames; }
const int Sprite::get_current_frame() const { return current_frame; }

void Sprite::set_is_animating(const bool is_animating) {
  this->is_animating = is_animating;
}

const bool Sprite::get_is_animating() const { return is_animating; }

void Sprite::init_rects() {
  src = (Rectangle){0, 0, (float)texture.width / anim_frames,
                    (float)texture.height};

  flipped_src = (Rectangle){src.x, src.y, src.width * -1.0f, src.height};

  dest =
      (Rectangle){dest.x, dest.y, ((float)texture.width / anim_frames) * scale,
                  (float)texture.height * scale};

  hitbox = (Rectangle){dest.x, dest.y, dest.width, dest.height};

  // origin = (Vector2){dest.width / 2, dest.height / 2};
}

Sprite::~Sprite() {}

void Sprite::draw() {

  const Color color = WHITE;

  if (is_flipped) {
    DrawTexturePro(texture, flipped_src, dest, origin, rotation_angle, color);

  } else {
    DrawTexturePro(texture, src, dest, origin, rotation_angle, color);
  }

  if (is_animating && frame_counter % 10 == 0) {
    incr_frame();
    // frame_counter = 0;
  }
  frame_counter++;
}

void Sprite::draw_hitbox() {
  // DrawRectangleLines(dest.x, dest.y, dest.width, dest.height, RED);

  // drawing an extra line to help debug the hitbox/rotation problem with knives
  int x = hitbox.x;
  int y = hitbox.y;
  int w = hitbox.width;
  int h = hitbox.height;
  const int w2 = w / 2;
  const int h2 = h / 2;
  const int x0 = x + w2;
  const int y0 = y + h2;
  const int x1 = x - w2;
  const int y1 = y + h2;
  DrawLine(x0, y0, x1, y1, BLUE);

  DrawRectangleLines(hitbox.x, hitbox.y, hitbox.width, hitbox.height, RED);

  // DrawRectanglePro(hitbox, (Vector2){0, 0}, rotation_angle, RED);
}

void Sprite::move(const float x, const float y) {
  dest.x += x * scale;
  dest.y += y * scale;
}

void Sprite::move_rect(Rectangle &r) {
  dest.x = r.x;
  dest.y = r.y;
}

void Sprite::set_scale(const float scale) {
  this->scale = scale;
  init_rects();
}

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

const bool Sprite::get_is_marked_for_deletion() const {
  return is_marked_for_deletion;
}

void Sprite::set_is_flipped(const bool is_flipped) {
  this->is_flipped = is_flipped;
}

void Sprite::update() {
  // update the velocity
  velocity.x += acceleration.x;
  velocity.y += acceleration.y;
  // update the position
  // dest.x += velocity.x;
  // dest.y += velocity.y;
  // update the rotation_angle

  // hitbox = (Rectangle){origin.x, origin.y, dest.width, dest.height};
  hitbox = (Rectangle){dest.x, dest.y, dest.width, dest.height};

  // origin = (Vector2){-dest.width / 2, -dest.height / 2};
  // origin = (Vector2){dest.width / 2, dest.height / 2};
  origin = (Vector2){0, 0};

  if (is_spinning) {
    // hitbox = (Rectangle){dest.x + dest.width / 2.0f,
    hitbox = (Rectangle){dest.x, dest.y - dest.height / 2.0f, dest.width,
                         dest.height};

    origin = (Vector2){dest.width / 2, dest.height / 2};

    rotation_angle += velocity.x;
  }
}

void Sprite::flip() { set_is_flipped(!get_is_flipped()); }

void Sprite::set_hp(const int hp) { this->hp = hp; }
void Sprite::set_maxhp(const int maxhp) { this->maxhp = maxhp; }
const int Sprite::get_hp() const { return hp; }
const int Sprite::get_maxhp() const { return maxhp; }

void Sprite::decr_hp(const int hp) {

  if (this->hp - hp <= 0) {
    this->hp = 0;
    // mark_for_deletion();
  } else {
    this->hp -= hp;
  }
}

void Sprite::incr_hp(const int hp) {
  if (this->hp + hp >= maxhp) {
    this->hp = maxhp;
  } else {
    this->hp += hp;
  }
}

void Sprite::set_rotation_angle(const float angle) { rotation_angle = angle; }
const float Sprite::get_rotation_angle() const { return rotation_angle; }

void Sprite::set_is_spinning(const bool is_spinning) {

  this->is_spinning = is_spinning;
}

const bool Sprite::get_is_spinning() const { return is_spinning; }

const Rectangle Sprite::get_hitbox() const { return hitbox; }
