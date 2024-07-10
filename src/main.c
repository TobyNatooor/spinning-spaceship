#include "main.h"
#include "include/raylib.h"
#include <stdio.h>
#include <stdlib.h>

#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

int main(void) {
  struct LoopArg *loopArg = Setup();
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Spinning Spaceship");

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
  while (loopArg->sections != NULL) {
    RemoveSection(&loopArg->sections);
    loopArg->sections = loopArg->sections->next;
  }
  free(loopArg);

  return 0;
}

struct LoopArg *Setup(void) {
  struct Player *player = malloc(sizeof(struct Player));
  player->position.x = SCREEN_WIDTH / 2.0;
  player->position.y = SCREEN_HEIGHT / 2.0;
  player->radius = 50;

  struct LoopArg *loopArg = malloc(sizeof(struct LoopArg));
  loopArg->player = player;
  loopArg->sections = NULL;
  AddStraightSection(&loopArg->sections, (Vector2){0, 0});

  return loopArg;
}

void Loop(void *loopArg_) {
  struct LoopArg *arg = loopArg_;
  struct Player *player = arg->player;
  struct Section **sections = &arg->sections;

  float frameTime = GetFrameTime();

  if (IsKeyDown(KEY_A))
    player->position.x -= 100 * frameTime;
  if (IsKeyDown(KEY_D))
    player->position.x += 100 * frameTime;
  if (IsKeyDown(KEY_W))
    player->position.y -= 100 * frameTime;
  if (IsKeyDown(KEY_S))
    player->position.y += 100 * frameTime;

  MoveSectionsDown(*sections, 200 * frameTime);

  RemoveSectionIfOutOfScreen(sections);

  if (IsPlayerCollidingWalls(player, *sections))
    printf("hit wall\n");

  if (CountSections(*sections) <= 1) {
    switch (rand() % 3) {
    case 0:
      AddStraightSection(sections, (Vector2){0, -SCREEN_HEIGHT});
      break;
    case 1:
      AddCurveLeftSection(sections, (Vector2){0, -SCREEN_HEIGHT});
      break;
    case 2:
      AddCurveRightSection(sections, (Vector2){0, -SCREEN_HEIGHT});
      break;
    }
  }

  BeginDrawing();

  ClearBackground(DARKGRAY);
  DrawSections(*sections);
  DrawCircle(player->position.x, player->position.y, player->radius, WHITE);
  DrawText(TextFormat("section count: %d\nwall count: %d",
                      CountSections(*sections),
                      CountWalls((*sections)->wallList)),
           0, 0, 12, WHITE);

  EndDrawing();
}

bool IsPlayerCollidingWalls(struct Player *player, struct Section *sections) {
  while (sections != NULL) {
    struct WallList *wallList = sections->wallList;
    while (wallList != NULL) {
      if (CheckCollisionCircleLine(player->position, player->radius,
                                   wallList->wallStart, wallList->wallEnd))
        return true;
      wallList = wallList->next;
    }
    sections = sections->next;
  }
  return false;
}
