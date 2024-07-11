#include "section.h"

typedef WallNode LineNode;
typedef struct Player {
  Texture2D texture;
  LineNode *collisionLines;
  Vector2 position;
  float radius;
} Player;

typedef struct LoopArg {
  Player *player;
  SectionNode *sections;
} LoopArg;

LoopArg *Setup(void);
void Loop(void *loopArg);

void MovePlayer(Player *player, Vector2 direction);

// Draw functions
void DrawWalls(WallNode *wallList);
void DrawSections(SectionNode *sections);

// Collision functions
bool IsPlayerCollidingWalls(Player *player, SectionNode *sections);
