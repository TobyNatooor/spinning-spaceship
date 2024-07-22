#include "player.h"

typedef enum Display {
  StartScreen,
  GameScreen,
} Display;

// Input functions
bool IsButtonClicked(Rectangle button);

// Draw functions
void DrawStartScreen(Player *player, SectionNode **sections, Display *display,
                     Texture2D background);
void DrawGameScreen(Player *player, SectionNode **sections, Display *display,
                    Texture2D background);
void DrawGame(Player *player, SectionNode **sections, Display *display,
              Texture2D background);
void DrawLines(LineNode *lines);
void DrawSections(SectionNode *sections);
void DrawButton(Rectangle rect, const char *text, int font, Color color,
                float borderWidth, Color borderColor);
