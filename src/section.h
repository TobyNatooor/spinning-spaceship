#include "wall.h"

struct Section {
  struct Section *next;
  struct Section *previous;
  struct WallList *wallList;
};

struct Section *AddSection(struct Section **sections, struct WallList *wallList);
void RemoveSection(struct Section **sections);
void DrawSections(struct Section *sections);
int CountSections(struct Section *sections);
void MoveSectionsDown(struct Section *sections, int offset);
bool SectionIsOutOfScreen(struct Section *section);
void RemoveSectionIfOutOfScreen(struct Section **sections);
void AddWallV(struct Section *section, Vector2 startPoint, Vector2 endPoint);
void AddStraightSection(struct Section **section, Vector2 offset);
void AddCurveLeftSection(struct Section **section, Vector2 offset);
void AddCurveRightSection(struct Section **section, Vector2 offset);
