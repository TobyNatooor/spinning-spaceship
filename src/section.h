#include "wall.h"

typedef struct SectionNode {
  struct SectionNode *next;
  struct SectionNode *prev;
  WallNode *walls;
} SectionNode;

void AddSection(SectionNode **sections, SectionNode *newSection);
void RemoveSection(SectionNode **sections);
int CountSections(SectionNode *sections);
void MoveSectionsDown(SectionNode *sections, int offset);
bool SectionIsOutOfScreen(SectionNode *section);
void RemoveSectionIfOutOfScreen(SectionNode **sections);
void AddStraightSection(SectionNode **sections, Vector2 offset);
void AddCurveLeftSection(SectionNode **sections, Vector2 offset);
void AddCurveRightSection(SectionNode **sections, Vector2 offset);
