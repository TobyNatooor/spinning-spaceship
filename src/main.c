#include "main.h"
#include "include/raylib.h"
#include <stdio.h>
#include <stdlib.h>

#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

int main(void) {
  LoopArg *loopArg = Setup();
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
  while (loopArg->sections != NULL)
    RemoveSection(&loopArg->sections);
  free(loopArg);

  return 0;
}

LoopArg *Setup(void) {
  Player *player = malloc(sizeof(Player));
  player->position.x = SCREEN_WIDTH / 2.0;
  player->position.y = SCREEN_HEIGHT / 2.0;
  player->radius = 50;

  LoopArg *loopArg = malloc(sizeof(LoopArg));
  loopArg->player = player;
  loopArg->sections = NULL;
  AddStraightSection(&loopArg->sections);

  return loopArg;
}

void Loop(void *loopArg_) {
  LoopArg *arg = loopArg_;
  Player *player = arg->player;
  SectionNode **sections = &arg->sections;

  float frameTime = GetFrameTime();

  if (IsKeyDown(KEY_A))
    player->position.x -= 200 * frameTime;
  if (IsKeyDown(KEY_D))
    player->position.x += 200 * frameTime;
  if (IsKeyDown(KEY_W))
    player->position.y -= 200 * frameTime;
  if (IsKeyDown(KEY_S))
    player->position.y += 200 * frameTime;

  MoveSections(*sections, (Vector2){0, 500 * frameTime});

  RemoveSectionIfOutOfScreen(sections);

  if (IsPlayerCollidingWalls(player, *sections))
    printf("hit wall\n");

  if (CountSections(*sections) <= 1) {
    switch (rand() % 3) {
    case 0:
      AddStraightSection(sections);
      break;
    case 1:
      AddCurveLeftSection(sections);
      break;
    case 2:
      AddCurveRightSection(sections);
      break;
    }
  }

  BeginDrawing();

  ClearBackground(DARKGRAY);
  DrawSections(*sections);
  DrawCircle(player->position.x, player->position.y, player->radius, WHITE);

  EndDrawing();
}

bool IsPlayerCollidingWalls(Player *player, SectionNode *sections) {
  while (sections != NULL) {
    WallNode *wallList = sections->walls;
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

void DrawWalls(WallNode *wallList) {
  while (wallList != NULL) {
    DrawLineV(wallList->wallStart, wallList->wallEnd, WHITE);
    wallList = wallList->next;
  }
}

void DrawSections(SectionNode *sections) {
  while (sections != NULL) {
    DrawWalls(sections->walls);
    sections = sections->next;
  }
}
