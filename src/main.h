#include "include/raylib.h"
#include "section.h"

typedef enum Display {
  StartScreen,
  GameScreen,
} Display;

typedef struct Player {
  Texture2D texture;
  LineNode *collisionLines;
  Vector2 position;
  bool isDead;
} Player;

typedef struct LoopArg {
  Display display;
  Player *player;
  SectionNode *sections;
} LoopArg;

LoopArg *Setup(void);
void Loop(void *loopArg);
void LoopStart(Display *display);
void LoopGame(Player *player, SectionNode **sections);

void InitNewGame(Player *player, SectionNode **sections);

void MovePlayer(Player *player, Vector2 direction);

bool IsButtonClicked(Rectangle button);

// Draw functions
void DrawLines(LineNode *lines);
void DrawSections(SectionNode *sections);
void DrawButton(Rectangle rect, const char *text, int font, Color color,
                float borderWidth, Color borderColor);

// Collision functions
bool IsPlayerCollidingWalls(Player *player, SectionNode *sections);
