#pragma once

#include "Sprite.h"
#include "control_mode.h"
#include "entity_id.h"
#include "raylib.h"
#include "texture_info.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

using std::make_shared;
using std::shared_ptr;
using std::string;
using std::unordered_map;
using std::vector;

typedef int scene_id;

typedef enum {

  SCENE_TRANSITION_NONE,
  SCENE_TRANSITION_IN,
  SCENE_TRANSITION_OUT,

} scene_transition;

typedef enum {

  SCENE_TYPE_TITLE,
  SCENE_TYPE_GAMEPLAY,
  SCENE_TYPE_GAMEOVER,

} scene_type;

class Scene {
public:
  Scene();
  ~Scene();

  virtual void update();
  virtual bool init();
  virtual void handle_input();
  virtual void draw_debug_panel();
  virtual void cleanup();
  virtual void draw_hud();

  virtual void draw();

  bool load_texture(const char *asset_name, const char *asset_path,
                    const int num_frames, const int is_player);
  bool load_textures();
  bool get_debug_panel_on();
  float get_global_scale();

  entity_id spawn_entity(const char *texture_key, float x, float y,
                         sprite_type type, bool is_anim, float scale);
  entity_id spawn_entity(const char *texture_key, float x, float y,
                         sprite_type type, bool is_anim);

  entity_id spawn_player(float x, float y);
  entity_id spawn_bat(const float x, const float y);
  entity_id spawn_bat();
  entity_id spawn_knife();
  unordered_map<entity_id, Vector2> &get_stars();
  unordered_map<entity_id, shared_ptr<Sprite>> &get_sprites();
  unordered_map<string, texture_info> &get_textures();
  Camera2D &get_camera2d();
  unsigned int get_current_frame();
  control_mode get_control_mode();
  Font &get_global_font();

  entity_id get_player_id();
  vector<entity_id> &get_entity_ids();

  scene_transition get_scene_transition();
  scene_id get_id();
  scene_type get_scene_type();

  void set_camera_default_values();
  void set_debug_panel_on(bool b);
  void set_global_scale(float s);
  void set_control_mode(control_mode cm);
  void flip_debug_panel();
  void load_fonts();
  void close();
  void set_texture_filepath(const char *filepath);
  void add_star();
  void update_stars_vx(const float vx);
  void set_has_been_initialized(bool b);
  void set_scene_transition(scene_transition st);
  void set_alpha(float a);
  void set_id(scene_id i);
  void set_player_id(entity_id id);
  void set_scene_type(scene_type st);
  // void draw_ground();
  // void draw_stars();
  void pause();
  void unpause();
  void set_knife_speed(const Vector2 speed);
  void set_knife_catches(const unsigned int catches);

  const bool get_has_been_initialized() const;
  const bool get_paused() const;
  const float get_alpha() const;
  const Vector2 get_starting_knife_speed() const;
  const Vector2 get_knife_speed() const;
  const unsigned int get_knife_catches() const;

  Mix_Music *get_music();
  void load_music(const char *path);
  void set_music_path(const char *path);
  const string get_music_path() const;

  const bool get_hud_on() const;
  void set_hud_on(const bool b);

  void incr_current_frame();

private:
  unordered_map<string, texture_info> textures;
  unordered_map<entity_id, shared_ptr<Sprite>> sprites;
  unordered_map<entity_id, shared_ptr<Sprite>> bgsprites;
  unordered_map<entity_id, bool> gravity;

  vector<entity_id> entity_ids;
  vector<entity_id> bg_entity_ids;

  Font global_font;

  Camera2D camera2d = {0};
  float global_scale = 1.0f;
  bool has_been_initialized = false;
  entity_id player_id = -1;
  bool debug_panel_on = true;
  control_mode controlmode = CONTROL_MODE_PLAYER;
  unsigned int current_frame = 0;

  string texture_filepath;

  unordered_map<entity_id, Vector2> stars;

  scene_transition transition = SCENE_TRANSITION_NONE;
  float alpha = 1.0f;

  scene_id id;
  scene_type scenetype;

  bool is_paused = false;

  const Vector2 starting_knife_speed = {1, 0};
  Vector2 knife_speed = {1, 0};

  unsigned int knife_catches = 0;

  string music_path;
  Mix_Music *music = NULL;

  bool hud_on = true;

  // Music music;
};
