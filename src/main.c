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
  struct Player *player = malloc(sizeof(struct Player));
  player->position.x = SCREEN_WIDTH / 2;
  player->position.y = SCREEN_HEIGHT / 2;

  struct WallList wall1 = {
      .wallStart = (Vector2){300, 0},
      .wallEnd = (Vector2){300, 450},
      .next = NULL,
      .previous = NULL,
  };
  struct WallList wall2 = {
      .wallStart = (Vector2){500, 200},
      .wallEnd = (Vector2){500, 450},
      .next = NULL,
      .previous = NULL,
  };
  struct WallList *wallList = NULL;
  AddWall(&wallList, &wall1);
  AddWall(&wallList, &wall2);

  struct LoopArg *loopArg = malloc(sizeof(struct LoopArg));
  loopArg->player = player;
  loopArg->wallList = wallList;

  return loopArg;
}

void Loop(void *loopArg_) {
  struct LoopArg *arg = loopArg_;
  struct Player *player = arg->player;
  struct WallList **wallList = &arg->wallList;

  float frameTime = GetFrameTime();

  if (IsKeyDown(KEY_A))
    player->position.x -= 100 * frameTime;
  if (IsKeyDown(KEY_D))
    player->position.x += 100 * frameTime;
  if (IsKeyDown(KEY_W))
    player->position.y -= 100 * frameTime;
  if (IsKeyDown(KEY_S))
    player->position.y += 100 * frameTime;

  MoveWallsDown(*wallList, 100 * frameTime);

  RemoveWallNodeIf(wallList, WallIsOutOfScreen);

  if (CheckCollisionPlayerWallList(player, *wallList))
    printf("hit wall\n");

  if (CountWallList(*wallList) <= 0) {
    printf("test");
    struct WallList wall1 = {
        .wallStart = (Vector2){200, 0},
        .wallEnd = (Vector2){200, 450},
        .next = NULL,
        .previous = NULL,
    };
    struct WallList wall2 = {
        .wallStart = (Vector2){600, 0},
        .wallEnd = (Vector2){600, 450},
        .next = NULL,
        .previous = NULL,
    };
    AddWall(wallList, &wall1);
    AddWall(wallList, &wall2);
  }

  BeginDrawing();

  DrawWallList(*wallList);
  DrawCircle(player->position.x, player->position.y, 50, BLACK);
  DrawText(TextFormat("wall count: %d", CountWallList(*wallList)), 0, 0, 12,
           BLACK);

  EndDrawing();
}

void AddWall(struct WallList **head, struct WallList *newWall) {
  struct WallList *wallList = malloc(sizeof(struct WallList));
  *wallList = *newWall;

  if (*head == NULL) {
    printf("1\n");
    *head = wallList;
    return;
  }
  printf("2\n");

  struct WallList *temp = *head;
  while (temp->next != NULL) {
    temp = temp->next;
  }
  temp->next = wallList;
  wallList->previous = temp;
}

int CountWallList(struct WallList *head) {
  int count = 0;
  while (head != NULL) {
    count++;
    head = head->next;
  }
  return count;
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

bool WallIsOutOfScreen(struct WallList *head) {
  if (head == NULL)
    return false;
  if ((0 > head->wallEnd.x || head->wallEnd.x > SCREEN_WIDTH ||
       0 > head->wallEnd.y || head->wallEnd.y > SCREEN_HEIGHT) &&
      (0 > head->wallStart.x || head->wallStart.x > SCREEN_WIDTH ||
       0 > head->wallStart.y || head->wallStart.y > SCREEN_HEIGHT))
    return true;
  return false;
}

void RemoveWall(struct WallList **head) {
  struct WallList *next = (*head)->next;
  struct WallList *previous = (*head)->previous;
  free(*head);
  *head = NULL;
  if (previous != NULL && next != NULL) {
    previous->next = next;
    next->previous = previous;
  } else if (previous != NULL) {
    previous->next = NULL;
  } else if (next != NULL) {
    next->previous = NULL;
  }
}

void RemoveWallNodeIf(struct WallList **head,
                      bool (*condition)(struct WallList *head)) {
  while (*head != NULL) {
    if (condition(*head))
      RemoveWall(head);
    head = &(*head)->next;
  }
}
