#include "game.h"

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

void DrawStartScreen(Player *player, SectionNode **sections, Display *display,
                     Texture2D background) {
  Rectangle startButton = {SCREEN_WIDTH * 0.1, 200, SCREEN_WIDTH * 0.8, 100};

  if (IsButtonClicked(startButton)) {
    *display = GameScreen;
    InitNewGame(player, sections);
  }

  BeginDrawing();
  DrawGame(player, sections, display, background);

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

  if (IsPlayerCollidingWalls(player, *sections))
    player->isDead = true;

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

void DrawGameScreen(Player *player, SectionNode **sections, Display *display,
                    Texture2D background) {
  BeginDrawing();
  DrawGame(player, sections, display, background);
  EndDrawing();
}

void DrawGame(Player *player, SectionNode **sections, Display *display,
              Texture2D background) {
  ClearBackground(DARKGRAY);

  int bgHeightX2 = background.height * 2;
  int timeX100 = GetTime() * 100;
  float y1 = ((timeX100 + SCREEN_HEIGHT) % bgHeightX2) - background.height;
  float y2 = ((timeX100) % bgHeightX2) - bgHeightX2 + SCREEN_HEIGHT;
  DrawTexture(background, 0, y1, WHITE);
  DrawTexture(background, 0, y2, WHITE);

  DrawSections(*sections);

  DrawTexturePro(player->texture, (Rectangle){0, 0, 100, 100},
                 (Rectangle){player->position.x, player->position.y, 100, 100},
                 (Vector2){50, 50}, player->rotation * (180 / PI), WHITE);

  if (player->isDead) {
    Rectangle restartButton = {SCREEN_WIDTH * 0.1, SCREEN_HEIGHT * 0.3,
                               SCREEN_WIDTH * 0.8, SCREEN_HEIGHT * 0.1};
    Rectangle mainMenuButton = {SCREEN_WIDTH * 0.1, SCREEN_HEIGHT * 0.45,
                                SCREEN_WIDTH * 0.8, SCREEN_HEIGHT * 0.1};
    DrawButton(restartButton, "Restart Game", 20, DARKGRAY, 2, WHITE);
    DrawButton(mainMenuButton, "Main Menu", 20, DARKGRAY, 2, WHITE);
    if (IsButtonClicked(restartButton))
      InitNewGame(player, sections);
    if (IsButtonClicked(mainMenuButton)) {
      *display = StartScreen;
      InitNewGame(player, sections);
    }

    const char *text = TextFormat("Score: %.2f", player->score);
    int textWidth = MeasureText(text, 30);
    DrawText(text, SCREEN_WIDTH / 2.0 - textWidth / 2.0, 160, 30, WHITE);

  } else if (*display == GameScreen) {
    DrawText(TextFormat("Score: %.2f", player->score), 10, 10, 18, WHITE);
  }
#if defined(DEBUG)
  Vector2 *points = player->points;
  DrawCircle(player->position.x, player->position.y, 2, RED);
  for (int i = 0; i < PLAYER_POINTS; i++)
    DrawLineV(points[i], points[(i + 1) % PLAYER_POINTS], RED);
#endif
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

