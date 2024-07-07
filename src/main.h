#include "include/raylib.h"

#define SCREEN_HEIGHT 450
#define SCREEN_WIDTH 800

struct WallList {
  struct WallList *next;
  struct WallList *previous;
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

void AddWall(struct WallList **head, struct WallList *newWall);
int CountWallList(struct WallList *head);
void MoveWallsDown(struct WallList *head, int offset);
bool WallIsOutOfScreen(struct WallList *head);
void RemoveWall(struct WallList **head);
void RemoveWallNodeIf(struct WallList **head, bool (*condition)(struct WallList *head));
void DrawWallList(struct WallList *head);
bool CheckCollisionPlayerWallList(struct Player *player, struct WallList *head);
