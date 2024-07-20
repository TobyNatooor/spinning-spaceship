#include "main.h"
#include "include/raylib.h"
#include <math.h>
#include <stdbool.h>
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
  player->position = (Vector2){0, 0};
  player->points = malloc(PLAYER_POINTS * sizeof(Vector2));

  LoopArg *loopArg = malloc(sizeof(LoopArg));
  loopArg->player = player;
  loopArg->sections = NULL;
  loopArg->isPaused = false;

  loopArg->display = StartScreen;

  return loopArg;
}

void Loop(void *loopArg_) {
  LoopArg *arg = loopArg_;
  Display *display = &arg->display;
  Player *player = arg->player;
  SectionNode **sections = &arg->sections;

  if (IsKeyPressed(KEY_SPACE)) {
    arg->isPaused = !arg->isPaused;
    printf("bool: %d\n", arg->isPaused);
  }

  switch (*display) {
  case StartScreen:
    DrawStartScreen(player, sections, display);
    break;
  case GameScreen:
    if (!arg->isPaused)
      UpdateGame(player, sections);
    DrawGameScreen(player, sections, display);
    break;
  }
}

void InitNewGame(Player *player, SectionNode **sections) {
  player->position.x = SCREEN_WIDTH / 2.0 + player->texture.width / 2.0 -
                       player->texture.width / 2.0;
  player->position.y = SCREEN_HEIGHT / 1.3 + player->texture.height / 2.0;
  player->rotation = 0;
  player->isDead = false;
  player->score = 0;

  player->points[0] = (Vector2){player->position.x, player->position.y - 50};
  player->points[1] =
      (Vector2){player->position.x - 15, player->position.y - 30};
  player->points[2] =
      (Vector2){player->position.x - 18, player->position.y + 15};
  player->points[3] =
      (Vector2){player->position.x - 28, player->position.y + 28};
  player->points[4] =
      (Vector2){player->position.x - 27, player->position.y + 50};
  player->points[5] =
      (Vector2){player->position.x - 12, player->position.y + 35};
  player->points[6] = (Vector2){player->position.x, player->position.y + 50};
  player->points[7] =
      (Vector2){player->position.x + 12, player->position.y + 35};
  player->points[8] =
      (Vector2){player->position.x + 27, player->position.y + 50};
  player->points[9] =
      (Vector2){player->position.x + 28, player->position.y + 28};
  player->points[10] =
      (Vector2){player->position.x + 18, player->position.y + 15};
  player->points[11] =
      (Vector2){player->position.x + 15, player->position.y - 30};

  while (*sections != NULL)
    RemoveSection(sections);
  AddStraightSection(sections);
}

void DrawStartScreen(Player *player, SectionNode **sections, Display *display) {
  Rectangle startButton = {SCREEN_WIDTH * 0.1, 200, SCREEN_WIDTH * 0.8, 100};
  if (IsButtonClicked(startButton)) {
    *display = GameScreen;
    InitNewGame(player, sections);
  }

  BeginDrawing();

  ClearBackground(DARKGRAY);

  int titleTextWidth = MeasureText("Spinning Spaceship", 40);
  DrawText("Spinning Spaceship", SCREEN_WIDTH / 2 - titleTextWidth / 2, 100, 40,
           WHITE);
  DrawButton(startButton, "Start Game", 20, DARKGRAY, 5, WHITE);

  EndDrawing();
}

void UpdateGame(Player *player, SectionNode **sections) {
  if (player->isDead)
    return;

  float frameTime = GetFrameTime();

  player->score += frameTime;

  RotatePlayer(player);

  MovePlayer(player);

  MoveSections(*sections, (Vector2){0, 500 * frameTime});

  RemoveSectionIfOutOfScreen(sections);

  if (IsPlayerCollidingWalls(player, *sections)) {
    player->isDead = true;
    printf("player hit wall\n");
  }

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
}

void DrawGameScreen(Player *player, SectionNode **sections, Display *display) {
  if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
    Vector2 mousePosition = GetMousePosition();
    printf("player->position.x %f, player->position.y %f\n",
           mousePosition.x - player->position.x,
           mousePosition.y - player->position.y);
  }
  BeginDrawing();

  ClearBackground(DARKGRAY);
  DrawSections(*sections);
  DrawTexturePro(player->texture, (Rectangle){0, 0, 100, 100},
                 (Rectangle){player->position.x, player->position.y, 100, 100},
                 (Vector2){50, 50}, player->rotation * (180 / PI), WHITE);

  if (player->isDead) {
    Rectangle restartButton = {SCREEN_WIDTH * 0.1, SCREEN_HEIGHT * 0.3,
                               SCREEN_WIDTH * 0.8, SCREEN_HEIGHT * 0.1};
    DrawButton(restartButton, "Restart Game", 20, DARKGRAY, 2, WHITE);
    if (IsButtonClicked(restartButton)) {
      InitNewGame(player, sections);
      printf("restart button clicked\n");
    }
    Rectangle mainMenuButton = {SCREEN_WIDTH * 0.1, SCREEN_HEIGHT * 0.45,
                                SCREEN_WIDTH * 0.8, SCREEN_HEIGHT * 0.1};
    DrawButton(mainMenuButton, "Main Menu", 20, DARKGRAY, 2, WHITE);
    if (IsButtonClicked(mainMenuButton)) {
      *display = StartScreen;
    }
    const char *text = TextFormat("Score: %.2f", player->score);
    int textWidth = MeasureText(text, 30);
    DrawText(text, SCREEN_WIDTH / 2.0 - textWidth / 2.0, 160, 30, WHITE);
  } else {
    DrawText(TextFormat("Score: %.2f", player->score), 10, 10, 18, WHITE);
  }
#if defined(DEBUG)
  Vector2 *points = player->points;
  DrawCircle(player->position.x, player->position.y, 2, RED);
  for (int i = 0; i < PLAYER_POINTS; i++)
    DrawLineV(points[i], points[(i + 1) % PLAYER_POINTS], RED);
#endif
  EndDrawing();
}

void RotatePlayer(Player *player) {
  float spinRate = GetFrameTime() + player->score / 10000;

  Vector2 *points = player->points;
  for (int i = 0; i < PLAYER_POINTS; i++) {
    float adjacent = points[i].x - player->position.x;
    float opposite = points[i].y - player->position.y;
    points[i].x = player->position.x + adjacent * cos(spinRate) -
                  opposite * sin(spinRate);
    points[i].y = player->position.y + adjacent * sin(spinRate) +
                  opposite * cos(spinRate);
  }
  player->rotation += spinRate;
}

void MovePlayer(Player *player) {
  Vector2 forwardDirection = {cos(player->rotation - PI / 2),
                              sin(player->rotation - PI / 2)};
  Vector2 rightDirection = {cos(player->rotation), sin(player->rotation)};

  float speed = 1.5;
  Vector2 direction = {0, 0};
  if (IsKeyDown(KEY_A)) {
    direction.x -= rightDirection.x * speed;
    direction.y -= rightDirection.y * speed;
  }
  if (IsKeyDown(KEY_D)) {
    direction.x += rightDirection.x * speed;
    direction.y += rightDirection.y * speed;
  }
  if (IsKeyDown(KEY_W)) {
    direction.x += forwardDirection.x * speed;
    direction.y += forwardDirection.y * speed;
  }
  if (IsKeyDown(KEY_S)) {
    direction.x -= forwardDirection.x * speed;
    direction.y -= forwardDirection.y * speed;
  }
  if (player->position.x + direction.x < 0)
    direction.x = player->position.x - direction.x;
  if (player->position.x + direction.x > SCREEN_WIDTH)
    direction.x = -SCREEN_WIDTH + player->position.x;

  if (player->position.y + direction.y < 0)
    direction.y = player->position.y - direction.y;
  if (player->position.y + direction.y > SCREEN_HEIGHT)
    direction.y = -SCREEN_HEIGHT + player->position.y;

  player->position.x += direction.x;
  player->position.y += direction.y;

  Vector2 *points = player->points;
  for (int i = 0; i < PLAYER_POINTS; i++) {
    points[i].x += direction.x;
    points[i].y += direction.y;
  }
}

bool IsPlayerCollidingWalls(Player *player, SectionNode *sections) {
  while (sections != NULL) {
    LineNode *wallList = sections->walls;
    while (wallList != NULL) {
      Vector2 *points = player->points;
      for (int i = 0; i < PLAYER_POINTS; i++) {
        if (CheckCollisionLines(wallList->start, wallList->end, points[i],
                                points[(i + 1) % PLAYER_POINTS], NULL))
          return true;
      }
      wallList = wallList->next;
    }
    sections = sections->next;
  }
  return false;
}

void DrawLines(LineNode *lines) {
  while (lines != NULL) {
    DrawLineV(lines->start, lines->end, WHITE);
    lines = lines->next;
  }
}

void DrawSections(SectionNode *sections) {
  while (sections != NULL) {
    DrawLines(sections->walls);
    sections = sections->next;
  }
}

void DrawButton(Rectangle rect, const char *text, int font, Color color,
                float borderWidth, Color borderColor) {
  DrawRectangleRec(rect, borderColor);
  DrawRectangle(rect.x + borderWidth, rect.y + borderWidth,
                rect.width - borderWidth * 2, rect.height - borderWidth * 2,
                color);
  int rectTextWidth = MeasureText(text, font);
  DrawText(text, rect.x + rect.width / 2 - rectTextWidth / 2.0,
           rect.y + rect.height / 2 - (float)font / 2, 20, WHITE);
}

bool IsButtonClicked(Rectangle button) {
  if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
    Vector2 mousePosition = GetMousePosition();
    if (CheckCollisionPointRec(mousePosition, button))
      return true;
  }
  return false;
}
