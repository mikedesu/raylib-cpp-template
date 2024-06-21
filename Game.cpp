#include "Game.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>

using std::to_string;

static entity_id next_entity_id = 0;

Game::Game() {
  const int screenWidth = 1280;
  const int screenHeight = 720;
  const char *title = "test";
  InitWindow(screenWidth, screenHeight, title);
  SetTargetFPS(60); // Set our game to run at 60 frames-per-second
  camera = {0};
  load_assets();
  debug_panel_on = true;
  target = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());
  screenRect = (Rectangle){0, 0, screenWidth, -screenHeight};
  SetExitKey(KEY_Q);
  global_scale = 2.0f;
  spawn_player();
}

void Game::load_fonts() {
  const char font_path[] = "fonts/hack.ttf";
  global_font = LoadFont(font_path);
}

void Game::load_assets() {
  load_fonts();
  const char *assets_file_path = "assets.txt";
  FILE *assets_file = fopen(assets_file_path, "r");
  if (assets_file == NULL) {
    printf("Error: could not open file %s\n", assets_file_path);
    exit(1);
  }
  char asset_name[256] = {0};
  while (fscanf(assets_file, "%s", asset_name) != EOF) {
    char asset_path[256] = {0};
    int num_frames;
    int is_player;
    fscanf(assets_file, "%d %d %s", &num_frames, &is_player, asset_path);
    Texture2D texture = LoadTexture(asset_path);
    texture_info info = {texture, num_frames, is_player, asset_path};
    textures[asset_name] = info;
    bzero(asset_name, 256);
  }
  fclose(assets_file);
}

void Game::spawn_player() {
  shared_ptr<Sprite> sprite = make_shared<Sprite>(
      textures["skull"].texture, textures["skull"].num_frames, 0, 0);
  sprite->set_scale(global_scale);
  // sprite->set_scale(4.0f);
  sprites[next_entity_id] = sprite;
  player_id = next_entity_id;
  next_entity_id++;
}

void Game::spawn_bat() {
  int x = GetRandomValue(0, GetScreenWidth());
  int y = GetRandomValue(0, GetScreenHeight());
  shared_ptr<Sprite> sprite = make_shared<Sprite>(
      textures["bat"].texture, textures["bat"].num_frames, x, y);
  // sprite->set_scale(2.0f);
  sprite->set_scale(global_scale);
  // sprite->set_scale(4.0f);
  //  sprite->set_vx(0.0f);
  //  sprite->set_vy(0.0f);
  sprite->set_is_animating(true);
  sprites[next_entity_id] = sprite;
  next_entity_id++;
}

void Game::spawn_knife() {
  // get skull's position
  int x = sprites[player_id]->get_dest().x;
  int y = sprites[player_id]->get_dest().y;
  int w = sprites[player_id]->get_width();
  int h = sprites[player_id]->get_height();

  x += w;
  y += h / 2;

  // int x = GetRandomValue(0, GetScreenWidth());
  // int y = GetRandomValue(0, GetScreenHeight());
  shared_ptr<Sprite> sprite = make_shared<Sprite>(
      textures["knife"].texture, textures["knife"].num_frames, x, y);
  // sprite->set_scale(2.0f);
  sprite->set_scale(global_scale);
  // sprite->set_scale(4.0f);
  sprite->set_vx(1.0f);
  sprite->set_vy(0.0f);
  sprites[next_entity_id] = sprite;
  next_entity_id++;
}

void Game::handle_input() {
  if (IsKeyDown(KEY_UP)) {
    if (sprites[player_id]->get_dest().y > 0) {
      sprites[player_id]->move_pos_y(-global_scale);
    }
  } else if (IsKeyDown(KEY_DOWN)) {
    if (sprites[player_id]->get_dest().y <
        GetScreenHeight() - sprites[player_id]->get_height()) {
      sprites[player_id]->move_pos_y(global_scale);
    }
  }
  if (IsKeyDown(KEY_LEFT)) {
    if (sprites[player_id]->get_dest().x > 0) {
      sprites[player_id]->move_pos_x(-global_scale);
      sprites[player_id]->set_is_flipped(true);
    }
  } else if (IsKeyDown(KEY_RIGHT)) {
    if (sprites[player_id]->get_dest().x <
        GetScreenWidth() - sprites[player_id]->get_width()) {
      sprites[player_id]->move_pos_x(global_scale);
      sprites[player_id]->set_is_flipped(false);
    }
  }

  if (IsKeyDown(KEY_Z)) {
    sprites[player_id]->set_current_frame(1);
  } else if (IsKeyUp(KEY_Z)) {
    sprites[player_id]->set_current_frame(0);
  }

  if (IsKeyPressed(KEY_Z)) {
    spawn_knife();
  }

  if (IsKeyPressed(KEY_S)) {
    sprites[player_id]->set_scale(sprites[player_id]->get_scale() + 1.0f);
  }

  if (IsKeyPressed(KEY_A)) {
    if (sprites[player_id]->get_scale() > 1.0f) {
      sprites[player_id]->set_scale(sprites[player_id]->get_scale() - 1.0f);
    }
  }

  if (IsKeyPressed(KEY_B)) {
    spawn_bat();
  }

  // if (IsKeyDown(KEY_X)) {
  //  sprites[player_id]->set_scale(2.0f);
  //}

  if (IsKeyPressed(KEY_F)) {
    ToggleFullscreen();
  }
  if (IsKeyPressed(KEY_D)) {
    debug_panel_on = !debug_panel_on;
  }
}

// we want to slow down how fast the animations occur
// so we only increment the frame every 10 frames
void Game::draw_sprite(entity_id id) {
  sprites[id]->draw();
  if (sprites[id]->get_is_animating() && current_frame % 10 == 0) {
    sprites[id]->incr_frame();
  }
  if (debug_panel_on) {
    sprites[id]->draw_hitbox();
  }
}

void Game::draw() {
  BeginDrawing();
  BeginMode2D(camera);
  BeginTextureMode(target);
  ClearBackground(WHITE);
  for (auto &sprite : sprites) {
    entity_id id = sprite.first;
    draw_sprite(id);
  }
  EndTextureMode();
  EndMode2D();
  DrawTextureRec(target.texture, screenRect, (Vector2){0, 0}, WHITE);
  DrawFPS(10, 10);
  // draw debug panel
  if (debug_panel_on) {
    draw_debug_panel(camera, global_font);
  }
  EndDrawing();

  current_frame++;
}

void Game::handle_sprite_movement(entity_id id) {
  sprites[id]->move_pos_x(sprites[id]->get_vx());
  sprites[id]->move_pos_y(sprites[id]->get_vy());

  // handle collision detection
  // we will need a function to check bounding box intersection

  // check if sprite moves off-screen
  if (sprites[id]->get_dest().x < 0 ||
      sprites[id]->get_dest().x > GetScreenWidth() ||
      sprites[id]->get_dest().y < 0 ||
      sprites[id]->get_dest().y > GetScreenHeight()) {
    sprites[id]->mark_for_deletion();
    return;
  }

  // for every other sprite, check if it collides with the current sprite
  // for (auto &sprite : sprites) {
  //  if (CheckCollisionRecs(sprites[id]->get_dest(),
  //                         sprite.second->get_dest())) {
  //    // if the sprite is the player, we don't want to delete it
  //    if (sprite.first == player_id) {
  //      continue;
  //    }
  //    sprites[id]->mark_for_deletion();
  //  }
  //}

  // check if sprite collides with player
  // if (CheckCollisionRecs(sprites[id]->get_dest(),
  //                       sprites[player_id]->get_dest())) {
  //  if (id != player_id) {
  //    sprites[id]->mark_for_deletion();
  //  }
  //}
}

void Game::update() {
  for (auto &sprite : sprites) {
    entity_id id = sprite.first;
    handle_sprite_movement(id);
  }

  // handle collision separate from movement

  for (auto it = sprites.begin(); it != sprites.end();) {
    if (it->second->get_is_marked_for_deletion()) {
      it = sprites.erase(it);
    } else {
      it++;
    }
  }
}

void Game::run() {
  while (!WindowShouldClose()) {
    handle_input();
    update();
    draw();
  }
}

void Game::draw_debug_panel(Camera2D &camera, Font &font) {
  string camera_info_str = "Current Frame: " + to_string(current_frame) +
                           "\nCamera: " + to_string(camera.target.x) + ", " +
                           to_string(camera.target.y) + "\nPlayer: " +
                           to_string(sprites[player_id]->get_dest().x) + ", " +
                           to_string(sprites[player_id]->get_dest().y) + "\n" +
                           to_string(sprites.size()) + " sprites";
  DrawRectangle(0, 0, 500, 200, Fade(BLACK, 0.5f));
  DrawTextEx(font, camera_info_str.c_str(), (Vector2){10, 10}, 16, 0.5f, WHITE);
}

Game::~Game() {
  for (auto &texture : textures) {
    UnloadTexture(texture.second.texture);
  }
  textures.clear();
  sprites.clear();
  UnloadRenderTexture(target);
  CloseWindow();
}
