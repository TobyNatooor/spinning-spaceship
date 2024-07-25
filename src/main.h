#include "game.h"
#include "include/raylib.h"

typedef struct LoopArg {
  Texture2D background;
  Display display;
  Player *player;
  SectionNode *sections;
  bool isPaused;
} LoopArg;

LoopArg *Setup(void);
void Loop(void *loopArg);
void InitNewGame(Player *player, SectionNode **sections);
void UpdateGame(Player *player, SectionNode **sections);
