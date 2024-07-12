#include "line.h"

typedef struct SectionNode {
  struct SectionNode *next;
  struct SectionNode *prev;
  LineNode *walls;
} SectionNode;

void AddSection(SectionNode **sections, SectionNode *newSection);
void AddSectionFrom(SectionNode **sections, LineNode *walls);
void RemoveSection(SectionNode **sections);
int CountSections(SectionNode *sections);
void MoveSection(SectionNode *section, Vector2 offset);
void MoveSections(SectionNode *sections, Vector2 offset);
bool SectionIsOutOfScreen(SectionNode *section);
void RemoveSectionIfOutOfScreen(SectionNode **sections);
void AddStraightSection(SectionNode **sections);
void AddCurveLeftSection(SectionNode **sections);
void AddCurveRightSection(SectionNode **sections);
float GetSectionTopY(SectionNode *section);
