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
  struct Section *next;
  struct Section *previous;
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

// Wall-related functions
void AddWall(struct WallList **head, struct WallList *newWall);
void AddWallV(struct WallList **head, Vector2 startPoint, Vector2 endPoint);
void MoveSectionsDown(struct Section *sections, int offset);
bool WallIsOutOfScreen(struct WallList *head);
void RemoveWall(struct WallList **head);
struct WallList *GetLastWall(struct WallList *head);

// Section-related functions
void AddStraightSection(struct Section **section);
void AddCurveLeftSection(struct Section **section);
void AddCurveRightSection(struct Section **section);
void AddSection(struct Section **sections, struct Section *newSection);
void RemoveSection(struct Section **sections);
void DrawSections(struct Section *sections);
int CountSections(struct Section *sections);
bool SectionIsOutOfScreen(struct Section *section);
void RemoveSectionIfOutOfScreen(struct Section **sections);

// Collision-related functions
bool CheckCollisionPlayerWallList(struct Player *player, struct WallList *head);
