#include "main.h"
#include "include/raylib.h"
#include <stdlib.h>

#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

int main(void) {
  struct LoopArg *loopArg = Setup();
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Cartpole Visualization");

#if defined(PLATFORM_WEB)
  emscripten_set_main_loop_arg(Loop, loopArg, 0, 1);
#else
  SetTargetFPS(60);

  while (!WindowShouldClose()) {
    Loop(loopArg);
  }
#endif

  CloseWindow();

  free(loopArg->player);
  free(loopArg);

  return 0;
}

struct LoopArg *Setup(void) {
  struct Player *player = malloc(sizeof(struct Player));
  player->position.x = SCREEN_WIDTH / 2;
  player->position.y = SCREEN_HEIGHT / 2;

  struct LoopArg *loopArg = malloc(sizeof(struct LoopArg));
  loopArg->player = player;

  return loopArg;
}

void Loop(void *loopArg_) {
  struct LoopArg *arg = loopArg_;
  struct Player *player = arg->player;

  if (IsKeyDown(KEY_A))
    player->position.x -= 1;
  if (IsKeyDown(KEY_D))
    player->position.x += 1;
  if (IsKeyDown(KEY_W))
    player->position.y -= 1;
  if (IsKeyDown(KEY_S))
    player->position.y += 1;

  BeginDrawing();

  DrawCircle(player->position.x, player->position.y, 50, BLACK);

  EndDrawing();
}
