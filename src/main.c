#include "main.h"
#include "include/raylib.h"
#include <stdio.h>
#include <stdlib.h>

#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

int main(void) {
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Spinning Spaceship");

  LoopArg *loopArg = Setup();

#if defined(PLATFORM_WEB)
  emscripten_set_main_loop_arg(Loop, loopArg, 0, 1);
#else
  SetTargetFPS(60);

  while (!WindowShouldClose()) {
    Loop(loopArg);
  }
#endif

  CloseWindow();

  UnloadTexture(loopArg->player->texture);
  free(loopArg->player);
  while (loopArg->sections != NULL)
    RemoveSection(&loopArg->sections);
  free(loopArg);

  return 0;
}

LoopArg *Setup(void) {
  Player *player = malloc(sizeof(Player));
  Image playerImage = LoadImage("resources/spaceship.png");
  ImageResize(&playerImage, 100, 100);
  player->texture = LoadTextureFromImage(playerImage);
  player->position.x = SCREEN_WIDTH / 2.0;
  player->position.y = SCREEN_HEIGHT / 2.0;
  player->radius = 20;
  player->collisionLines = NULL;
  AddWallV(&player->collisionLines,
           (Vector2){player->position.x - 30, player->position.y + 50},
           (Vector2){player->position.x, player->position.y - 50});
  AddWallV(&player->collisionLines,
           (Vector2){player->position.x, player->position.y - 50},
           (Vector2){player->position.x + 30, player->position.y + 50});
  AddWallV(&player->collisionLines,
           (Vector2){player->position.x + 30, player->position.y + 50},
           (Vector2){player->position.x - 30, player->position.y + 50});

  LoopArg *loopArg = malloc(sizeof(LoopArg));
  loopArg->player = player;
  loopArg->sections = NULL;
  AddStraightSection(&loopArg->sections);

  loopArg->display = StartScreen;

  return loopArg;
}

void Loop(void *loopArg_) {
  LoopArg *arg = loopArg_;
  Display *display = &arg->display;
  Player *player = arg->player;
  SectionNode **sections = &arg->sections;

  switch (*display) {
  case StartScreen:
    LoopStart(display);
    break;
  case GameScreen:
    LoopGame(player, sections);
    break;
  }
}

void LoopStart(Display *display) {
  float tenthWidth = SCREEN_WIDTH / 10.0;
  Rectangle startButton = {tenthWidth, 200, tenthWidth * 8, 100};

  if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
    Vector2 mousePosition = GetMousePosition();
    if (CheckCollisionPointRec(mousePosition, startButton))
      *display = GameScreen;
  }

  BeginDrawing();

  ClearBackground(DARKGRAY);

  int titleTextWidth = MeasureText("Spinning Spaceship", 40);
  DrawText("Spinning Spaceship", SCREEN_WIDTH / 2 - titleTextWidth / 2, 100, 40, WHITE);


  // Start button
  DrawRectangleRec(startButton, WHITE);
  DrawRectangle(startButton.x + 5, startButton.y + 5, startButton.width - 10,
                startButton.height - 10, DARKGRAY);
  int startButtonTextWidth =MeasureText("Start Game", 20);
  DrawText("Start Game", tenthWidth * 5 - startButtonTextWidth / 2.0, 240, 20, WHITE);

  EndDrawing();
}

void LoopGame(Player *player, SectionNode **sections) {
  float frameTime = GetFrameTime();

  Vector2 direction = {0, 0};
  if (IsKeyDown(KEY_A))
    direction.x -= 200 * frameTime;
  if (IsKeyDown(KEY_D))
    direction.x += 200 * frameTime;
  if (IsKeyDown(KEY_W))
    direction.y -= 200 * frameTime;
  if (IsKeyDown(KEY_S))
    direction.y += 200 * frameTime;

  MovePlayer(player, direction);

  MoveSections(*sections, (Vector2){0, 500 * frameTime});

  RemoveSectionIfOutOfScreen(sections);

  if (IsPlayerCollidingWalls(player, *sections))
    printf("hit wall\n");

  if (CountSections(*sections) <= 2) {
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
  DrawTexture(player->texture, player->position.x - player->texture.width / 2.0,
              player->position.y - player->texture.height / 2.0, WHITE);

  EndDrawing();
}

void MovePlayer(Player *player, Vector2 direction) {
  player->position.x += direction.x;
  player->position.y += direction.y;

  LineNode *collisionLines = player->collisionLines;
  while (collisionLines != NULL) {
    collisionLines->wallStart.x += direction.x;
    collisionLines->wallStart.y += direction.y;
    collisionLines->wallEnd.x += direction.x;
    collisionLines->wallEnd.y += direction.y;
    collisionLines = collisionLines->next;
  }
}

bool IsPlayerCollidingWalls(Player *player, SectionNode *sections) {
  while (sections != NULL) {
    WallNode *wallList = sections->walls;
    while (wallList != NULL) {
      LineNode *collisionLines = player->collisionLines;
      while (collisionLines != NULL) {
        if (CheckCollisionLines(wallList->wallStart, wallList->wallEnd,
                                collisionLines->wallStart,
                                collisionLines->wallEnd, NULL))
          return true;
        collisionLines = collisionLines->next;
      }
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
