#pragma once

#include "Sprite.h"
#include "control_mode.h"
#include "entity_id.h"
#include "raylib.h"
#include "texture_info.h"
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

class Scene {
public:
  Scene();
  ~Scene();

  virtual void update();
  virtual bool init();
  virtual void handle_input();
  virtual void handle_cam_input();
  virtual void handle_player_input();

  void draw();

  bool load_texture(const char *asset_name, const char *asset_path,
                    const int num_frames, const int is_player);
  bool load_textures();

  bool get_debug_panel_on();

  void set_camera_default_values();
  void set_debug_panel_on(bool b);
  void set_global_scale(float s);
  float get_global_scale();

  void set_control_mode(control_mode cm);

  void flip_debug_panel();
  virtual void draw_debug_panel();

  void load_fonts();
  void close();

  entity_id spawn_entity(const char *texture_key, float x, float y,
                         sprite_type type, bool is_anim);
  entity_id spawn_player(float x, float y);
  void set_texture_filepath(const char *filepath);

  void add_star();
  bool get_has_been_initialized();
  void set_has_been_initialized(bool b);

  unordered_map<entity_id, shared_ptr<Sprite>> &get_sprites();
  unordered_map<string, texture_info> &get_textures();
  Camera2D &get_camera2d();
  unsigned int get_current_frame();
  control_mode get_control_mode();
  Font &get_global_font();

  void set_scene_transition(scene_transition st);
  scene_transition get_scene_transition();

  void set_alpha(float a);
  const float get_alpha() const;

  void set_id(scene_id i);
  scene_id get_id();

  void set_player_id(entity_id id);
  entity_id get_player_id();

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
};
