#include "section.h"

typedef struct Player {
  Vector2 position;
  float radius;
} Player;

typedef struct LoopArg {
  Player *player;
  SectionNode *sections;
} LoopArg;

LoopArg *Setup(void);
void Loop(void *loopArg);

// Draw functions
void DrawWalls(WallNode *wallList);
void DrawSections(SectionNode *sections);

// Collision functions
bool IsPlayerCollidingWalls(Player *player, SectionNode *sections);
