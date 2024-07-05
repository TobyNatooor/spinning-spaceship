#include "main.h"
#include "include/raylib.h"
#include <stdio.h>
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
  free(loopArg->wallList);
  free(loopArg);

  return 0;
}

struct LoopArg *Setup(void) {
  // try without allocating
  struct Player *player = malloc(sizeof(struct Player));
  player->position.x = SCREEN_WIDTH / 2;
  player->position.y = SCREEN_HEIGHT / 2;

  struct WallList wall1 = {
      .wallStart = (Vector2){300, 0},
      .wallEnd = (Vector2){300, 450},
      .next = NULL,
  };
  struct WallList wall2 = {
      .wallStart = (Vector2){500, 0},
      .wallEnd = (Vector2){500, 450},
      .next = NULL,
  };
  struct WallList *wallList = AddWall(NULL, &wall1);
  wallList = AddWall(wallList, &wall2);

  struct LoopArg *loopArg = malloc(sizeof(struct LoopArg));
  loopArg->player = player;
  loopArg->wallList = wallList;

  return loopArg;
}

void Loop(void *loopArg_) {
  struct LoopArg *arg = loopArg_;
  struct Player *player = arg->player;
  struct WallList *wallList = arg->wallList;

  float frameTime = GetFrameTime();

  if (IsKeyDown(KEY_A))
    player->position.x -= 100 * frameTime;
  if (IsKeyDown(KEY_D))
    player->position.x += 100 * frameTime;
  if (IsKeyDown(KEY_W))
    player->position.y -= 100 * frameTime;
  if (IsKeyDown(KEY_S))
    player->position.y += 100 * frameTime;

  MoveWallsDown(wallList, 100 * frameTime);

  if (CheckCollisionPlayerWallList(player, wallList))
    printf("hit wall\n");

  BeginDrawing();

  DrawWallList(wallList);
  DrawCircle(player->position.x, player->position.y, 50, BLACK);

  EndDrawing();
}

struct WallList *AddWall(struct WallList *head, struct WallList *newWall) {
  struct WallList *start = head;

  struct WallList *wallList = malloc(sizeof(struct WallList));
  *wallList = *newWall;

  if (head == NULL) {
    return wallList;
  }
  while (head->next != NULL) {
    head = head->next;
  }
  head->next = wallList;
  return start;
}

void MoveWallsDown(struct WallList *head, int offset) {
  while (head != NULL) {
    head->wallStart.y += offset;
    head->wallEnd.y += offset;
    head = head->next;
  }
}

void DrawWallList(struct WallList *head) {
  while (head != NULL) {
    DrawLineV(head->wallStart, head->wallEnd, BLACK);
    head = head->next;
  }
}

bool CheckCollisionPlayerWallList(struct Player *player,
                                  struct WallList *head) {
  while (head != NULL) {
    if (CheckCollisionCircleLine(player->position, 50, head->wallStart,
                                 head->wallEnd))
      return true;
    head = head->next;
  }
  return false;
}
