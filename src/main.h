#include "include/raylib.h"

#define SCREEN_HEIGHT 450
#define SCREEN_WIDTH 800

struct Player {
  Vector2 position;
};
struct LoopArg {
  struct Player *player;
};

struct LoopArg *Setup(void);
void Loop(void *loopArg);
