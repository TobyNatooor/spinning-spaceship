#include "include/raylib.h"
#include "section.h"

typedef enum Display {
  StartScreen,
  GameScreen,
} Display;

typedef struct Player {
  Texture2D texture;
  Vector2 *points;
  Vector2 position;
  float rotation;
  float score;
  bool isDead;
} Player;

typedef struct LoopArg {
  Display display;
  Player *player;
  SectionNode *sections;
  bool isPaused;
} LoopArg;

LoopArg *Setup(void);
void Loop(void *loopArg);
void DrawStartScreen(Player *player, SectionNode **sections, Display *display);
void UpdateGame(Player *player, SectionNode **sections, Display *display);
void DrawGameScreen(Player *player, SectionNode **sections, Display *display);

void InitNewGame(Player *player, SectionNode **sections);

void RotatePlayer(Player *player);
void MovePlayer(Player *player);

bool IsButtonClicked(Rectangle button);

// Draw functions
void DrawLines(LineNode *lines);
void DrawSections(SectionNode *sections);
void DrawButton(Rectangle rect, const char *text, int font, Color color,
                float borderWidth, Color borderColor);

// Collision functions
bool IsPlayerCollidingWalls(Player *player, SectionNode *sections);
