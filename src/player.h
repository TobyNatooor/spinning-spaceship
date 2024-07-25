#include "section.h"
#include <math.h>
#include <stdlib.h>


typedef struct Player {
  Texture2D texture;
  Vector2 *points;
  Vector2 position;
  float rotation;
  float score;
  bool isDead;
} Player;

bool IsPlayerCollidingWalls(Player *player, SectionNode *sections);
void RotatePlayer(Player *player);
void MovePlayer(Player *player);
