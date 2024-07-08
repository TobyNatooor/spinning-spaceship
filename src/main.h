#include "include/raylib.h"

#define SCREEN_HEIGHT 450
#define SCREEN_WIDTH 800

struct WallList {
  struct WallList *next;
  struct WallList *previous;
  Vector2 wallStart;
  Vector2 wallEnd;
};
struct Section {
  struct WallList *wallList;
};
struct Player {
  Vector2 position;
};
struct LoopArg {
  struct Player *player;
  struct Section *sections;
};

struct LoopArg *Setup(void);
void Loop(void *loopArg);

void AddWall(struct WallList **head, struct WallList *newWall);
void AddWallV(struct WallList **head, Vector2 startPoint, Vector2 endPoint);
int CountWallList(struct WallList *head);
void MoveWallsDown(struct WallList *head, int offset);
bool WallIsOutOfScreen(struct WallList *head);
void RemoveWall(struct WallList **head);
void RemoveSection(struct Section *sections);
void DrawWallList(struct WallList *head);
bool CheckCollisionPlayerWallList(struct Player *player, struct WallList *head);
struct WallList *GetLastWall(struct WallList *head);
void AddStraightSection(struct WallList **head);
void AddCurveLeftSection(struct WallList **head);
void AddCurveRightSection(struct WallList **head);
