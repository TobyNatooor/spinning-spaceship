#include "include/raylib.h"

#define SCREEN_HEIGHT 450
#define SCREEN_WIDTH 800

struct WallList {
  struct WallList *next;
  Vector2 wallStart;
  Vector2 wallEnd;
};
struct Player {
  Vector2 position;
};
struct LoopArg {
  struct Player *player;
  struct WallList *wallList;
};

struct LoopArg *Setup(void);
void Loop(void *loopArg);

struct WallList* AddWall(struct WallList *head, struct WallList *newWall);
void MoveWallsDown(struct WallList *head, int offset);
void DrawWallList(struct WallList *head);
bool CheckCollisionPlayerWallList(struct Player *player, struct WallList *head);
