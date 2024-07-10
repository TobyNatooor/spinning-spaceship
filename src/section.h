#include "wall.h"

struct Section {
  struct Section *next;
  struct Section *previous;
  struct WallList *wallList;
};

void AddSection(struct Section **sections, struct Section *newSection);
void RemoveSection(struct Section **sections);
int CountSections(struct Section *sections);
void MoveSectionsDown(struct Section *sections, int offset);
bool SectionIsOutOfScreen(struct Section *section);
void RemoveSectionIfOutOfScreen(struct Section **sections);
void AddStraightSection(struct Section **section, Vector2 offset);
void AddCurveLeftSection(struct Section **section, Vector2 offset);
void AddCurveRightSection(struct Section **section, Vector2 offset);
