#include "raylib.h"
// #include "rlgl.h"

RenderTexture target;
Rectangle screenRect;
Camera2D camera = {0};
CameraMode camera_mode = CAMERA_CUSTOM;

int main(void) {
  const int screenWidth = 1280;
  const int screenHeight = 720;

  InitWindow(screenWidth, screenHeight, "test");
  target = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());
  screenRect = (Rectangle){0, 0, -screenWidth, screenHeight};
  camera.target = (Vector2){0, 0};
  camera.offset = (Vector2){screenWidth / 2.0f, screenHeight / 2.0f};
  camera.rotation = 0.0f;
  camera.zoom = 1.0f;

  SetTargetFPS(60); // Set our game to run at 60 frames-per-second
  while (!WindowShouldClose()) {

    if (IsKeyDown(KEY_LEFT)) {
      camera.target.x -= 10;
    } else if (IsKeyDown(KEY_RIGHT)) {
      camera.target.x += 10;
    } else if (IsKeyDown(KEY_UP)) {
      camera.target.y -= 10;
    } else if (IsKeyDown(KEY_DOWN)) {
      camera.target.y += 10;
    }

    BeginDrawing();
    BeginTextureMode(target);
    BeginMode2D(camera);
    ClearBackground(BLACK);
    DrawRectangle(0, 0, 100, 100, RED);
    EndMode2D();
    EndTextureMode();
    DrawTextureRec(target.texture, screenRect, (Vector2){0, 0}, WHITE);
    DrawFPS(10, 10);
    EndDrawing();
  }
  UnloadRenderTexture(target);
  CloseWindow();
  return 0;
}
