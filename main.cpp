#include "raylib.h"
// #include "rcamera.h"

#include <string>

using std::string;
using std::to_string;

void draw_debug_panel(Camera2D &camera, Font &font);

int main(void) {
  const int screenWidth = 1280;
  const int screenHeight = 720;
  const char *title = "test";
  bool debug_panel_on = true;
  RenderTexture target;
  Rectangle screenRect;
  Camera2D camera = {0};
  const char *img_path = "../ecs/img/skull-sheet.png";
  float scale = 8;
  Rectangle skull_rect;
  Vector2 skull_position = (Vector2){0, 0};
  const char *font_path = "hack.ttf";
  Font font;
  int base_move_unit = 1;
  int move_unit = base_move_unit * scale;

  InitWindow(screenWidth, screenHeight, title);
  target = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());
  screenRect = (Rectangle){0, 0, screenWidth, -screenHeight};
  // set the button to close the window
  SetExitKey(KEY_Q);

  Texture2D texture = LoadTexture(img_path);
  skull_rect =
      (Rectangle){0, 0, (float)texture.width / 2.0f, (float)texture.height};
  font = LoadFont(font_path);
  // DisableCursor();

  SetTargetFPS(60); // Set our game to run at 60 frames-per-second
  while (!WindowShouldClose()) {
    if (IsKeyDown(KEY_UP)) {
      skull_position.y -= move_unit;
    } else if (IsKeyDown(KEY_DOWN)) {
      skull_position.y += move_unit;
    }

    if (IsKeyDown(KEY_LEFT)) {
      skull_position.x -= move_unit;
    } else if (IsKeyDown(KEY_RIGHT)) {
      skull_position.x += move_unit;
    }

    if (IsKeyPressed(KEY_F)) {
      ToggleFullscreen();
    }

    if (IsKeyPressed(KEY_D)) {
      debug_panel_on = !debug_panel_on;
    }

    BeginDrawing();
    BeginMode2D(camera);
    BeginTextureMode(target);
    ClearBackground(WHITE);
    DrawTexturePro(texture, skull_rect,
                   (Rectangle){skull_position.x, skull_position.y,
                               ((float)texture.width / 2) * scale,
                               texture.height * scale},
                   (Vector2){0, 0}, 0, WHITE);
    // DrawRectangleLines(48, 24, 24, 24, RED);
    EndTextureMode();
    EndMode2D();
    DrawTextureRec(target.texture, screenRect, (Vector2){0, 0}, WHITE);
    DrawFPS(10, 10);

    // draw debug panel
    if (debug_panel_on) {
      draw_debug_panel(camera, font);
    }
    EndDrawing();
  }
  UnloadRenderTexture(target);
  CloseWindow();
  return 0;
}

void draw_debug_panel(Camera2D &camera, Font &font) {
  string camera_info_str = "Camera: " + to_string(camera.target.x) + ", " +
                           to_string(camera.target.y);
  DrawRectangle(0, 0, 500, 200, Fade(BLACK, 0.5f));
  DrawTextEx(font, camera_info_str.c_str(), (Vector2){10, 10}, 16, 0.5f, WHITE);
}
