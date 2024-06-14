#include "raylib.h"

RenderTexture target;
Rectangle screenRect;
// Camera2D camera = {0};
Camera3D camera = {0};
CameraMode camera_mode = CAMERA_CUSTOM;

int main(void) {
  const int screenWidth = 1280;
  const int screenHeight = 720;

  const char *title = "test";

  InitWindow(screenWidth, screenHeight, title);
  target = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());
  screenRect = (Rectangle){0, 0, -screenWidth, screenHeight};

  // 2d camera
  // camera.target = (Vector2){0, 0};
  // camera.offset = (Vector2){screenWidth / 2.0f, screenHeight / 2.0f};
  // camera.rotation = 0.0f;
  // camera.zoom = 1.0f;

  camera.position = (Vector3){10.0f, 10.0f, 10.0f};
  camera.target = (Vector3){0.0f, 0.0f, 0.0f};
  camera.up = (Vector3){0.0f, 1.0f, 0.0f};
  camera.fovy = 45.0f;

  // set the button to close the window
  SetExitKey(KEY_Q);

  SetTargetFPS(60); // Set our game to run at 60 frames-per-second
  while (!WindowShouldClose()) {

    // if (IsKeyDown(KEY_LEFT)) {
    //   camera.target.x -= 10;
    // } else if (IsKeyDown(KEY_RIGHT)) {
    //   camera.target.x += 10;
    // } else if (IsKeyDown(KEY_UP)) {
    //   camera.target.y -= 10;
    // } else if (IsKeyDown(KEY_DOWN)) {
    //   camera.target.y += 10;
    // }

    BeginDrawing();
    BeginTextureMode(target);
    // BeginMode2D(camera);
    // ClearBackground(BLACK);
    // DrawRectangle(0, 0, 100, 100, RED);
    // EndMode2D();

    BeginMode3D(camera);

    ClearBackground(RAYWHITE);
    DrawGrid(10, 1.0f);
    DrawCube((Vector3){0, 0, 0}, 2.0f, 2.0f, 2.0f, RED);
    EndMode3D();

    EndTextureMode();
    DrawTextureRec(target.texture, screenRect, (Vector2){0, 0}, WHITE);
    DrawFPS(10, 10);
    EndDrawing();
  }
  UnloadRenderTexture(target);
  CloseWindow();
  return 0;
}
