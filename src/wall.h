#include "defines.h"
#include "include/raylib.h"

struct WallList {
  struct WallList *next;
  struct WallList *previous;
  Vector2 wallStart;
  Vector2 wallEnd;
};

// void AddWall(struct WallList **head, struct WallList *newWall);
void RemoveWall(struct WallList **head);
int CountWalls(struct WallList *wallList);
bool WallIsOutOfScreen(struct WallList *head);
