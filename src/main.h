#include "section.h"

struct Player {
  Vector2 position;
  float radius;
};

struct LoopArg {
  struct Player *player;
  struct Section *sections;
};

struct LoopArg *Setup(void);
void Loop(void *loopArg);
bool IsPlayerCollidingWalls(struct Player *player, struct Section *sections);
