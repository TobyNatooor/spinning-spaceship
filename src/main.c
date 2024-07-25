#include "main.h"

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
  free(loopArg->player->points);
  free(loopArg->player);
  while (loopArg->sections != NULL)
    RemoveSection(&loopArg->sections);
  free(loopArg);

  return 0;
}

LoopArg *Setup(void) {
  LoopArg *loopArg = malloc(sizeof(LoopArg));

  Player *player = malloc(sizeof(Player));
  Image playerImage = LoadImage("resources/spaceship.png");
  ImageResize(&playerImage, 100, 100);
  player->texture = LoadTextureFromImage(playerImage);
  UnloadImage(playerImage);
  player->position = (Vector2){0, 0};
  player->points = malloc(PLAYER_POINTS * sizeof(Vector2));
  loopArg->player = player;

  Image background = LoadImage("resources/starry_night.png");
  loopArg->background = LoadTextureFromImage(background);
  UnloadImage(background);

  loopArg->sections = NULL;
  loopArg->isPaused = false;
  loopArg->display = StartScreen;

  InitNewGame(player, &loopArg->sections);

  return loopArg;
}

void Loop(void *loopArg_) {
  LoopArg *arg = loopArg_;
  Display *display = &arg->display;
  Player *player = arg->player;
  SectionNode **sections = &arg->sections;
  Texture2D background = arg->background;

  // if (IsKeyPressed(KEY_SPACE))
  //   arg->isPaused = !arg->isPaused;
  if (IsKeyPressed(KEY_R) && player->isDead) {
    *display = GameScreen;
    InitNewGame(player, sections);
  }

  switch (*display) {
  case StartScreen:
    DrawStartScreen(player, sections, display, background);
    break;
  case GameScreen:
    if (!arg->isPaused)
      UpdateGame(player, sections);
    DrawGameScreen(player, sections, display, background);
    break;
  case EndScreen:
    DrawEndScreen(player, sections, display, background);
    break;
  }
}
