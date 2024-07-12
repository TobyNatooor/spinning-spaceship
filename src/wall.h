#include "defines.h"
#include "include/raylib.h"

typedef struct LineNode {
  struct LineNode *next;
  struct LineNode *prev;
  Vector2 start;
  Vector2 end;
} LineNode;

void AddLine(LineNode **lines, LineNode *newLine);
void AddLineV(LineNode **lines, Vector2 startPoint, Vector2 endPoint);
void RemoveLine(LineNode **lines);
int CountLines(LineNode *head);
bool LineIsOutOfScreen(LineNode *head);
