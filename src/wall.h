#include "defines.h"
#include "include/raylib.h"

typedef struct WallNode {
  struct WallNode *next;
  struct WallNode *prev;
  Vector2 wallStart;
  Vector2 wallEnd;
} WallNode;

void AddWall(WallNode **walls, WallNode *newWall);
void AddWallV(WallNode **walls, Vector2 startPoint, Vector2 endPoint);
void RemoveWall(WallNode **walls);
int CountWalls(WallNode *head);
bool WallIsOutOfScreen(WallNode *head);
