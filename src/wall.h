#include "defines.h"
#include "include/raylib.h"

struct WallList {
  struct WallList *next;
  struct WallList *previous;
  Vector2 wallStart;
  Vector2 wallEnd;
};

void AddWall(struct WallList **wallList, struct WallList *newWallList);
void RemoveWall(struct WallList **wallList);
int CountWalls(struct WallList *wallList);
bool WallIsOutOfScreen(struct WallList *head);
