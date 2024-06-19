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
    fscanf(assets_file, "%s %d %d", asset_path, &num_frames, &is_player);
    Texture2D texture = LoadTexture(asset_path);
    textures[asset_name] = texture;
    shared_ptr<Sprite> sprite = make_shared<Sprite>(texture, num_frames, 0, 0);
    if (is_player) {
      sprite->set_scale(4.0f);
    }
    sprites[next_entity_id] = sprite;
    if (is_player) {
      player_id = next_entity_id;
    }
    next_entity_id++;
    // clear the asset_name buffer
    bzero(asset_name, 256);
  }
  fclose(assets_file);
}

void Game::handle_input() {
  if (IsKeyDown(KEY_UP)) {
    sprites[player_id]->move_pos_y(-1);
  } else if (IsKeyDown(KEY_DOWN)) {
    sprites[player_id]->move_pos_y(1);
  }
  if (IsKeyDown(KEY_LEFT)) {
    sprites[player_id]->move_pos_x(-1);
  } else if (IsKeyDown(KEY_RIGHT)) {
    sprites[player_id]->move_pos_x(1);
  }
  if (IsKeyPressed(KEY_F)) {
    ToggleFullscreen();
  }
  if (IsKeyPressed(KEY_D)) {
    debug_panel_on = !debug_panel_on;
  }
}

void Game::draw() {
  BeginDrawing();
  BeginMode2D(camera);
  BeginTextureMode(target);
  ClearBackground(WHITE);
  for (auto &sprite : sprites) {
    sprite.second->draw();
    if (debug_panel_on) {
      sprite.second->draw_hitbox();
    }
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
}

void Game::run() {
  while (!WindowShouldClose()) {
    handle_input();
    draw();
  }
}

void Game::draw_debug_panel(Camera2D &camera, Font &font) {
  string camera_info_str = "Camera: " + to_string(camera.target.x) + ", " +
                           to_string(camera.target.y) + "\nPlayer: " +
                           to_string(sprites[player_id]->get_dest().x) + ", " +
                           to_string(sprites[player_id]->get_dest().y);
  DrawRectangle(0, 0, 500, 200, Fade(BLACK, 0.5f));
  DrawTextEx(font, camera_info_str.c_str(), (Vector2){10, 10}, 16, 0.5f, WHITE);
}

Game::~Game() {
  for (auto &texture : textures) {
    UnloadTexture(texture.second);
  }
  textures.clear();
  sprites.clear();
  UnloadRenderTexture(target);
  CloseWindow();
}
