#include "section.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

void AddSection(struct Section **sections, struct Section *newSection) {
  if (*sections == NULL) {
    *sections = newSection;
    return;
  }

  struct Section *last = *sections;
  while (last->next != NULL)
    last = last->next;

  last->next = newSection;
  newSection->previous = last;
}

void RemoveSection(struct Section **section) {
  struct WallList *wallList = (*section)->wallList;
  while (wallList != NULL)
    RemoveWall(&wallList);

  struct Section *previous = (*section)->previous;
  struct Section *next = (*section)->next;
  free(*section);
  *section = NULL;
  if (next != NULL)
    next->previous = previous;
  if (previous != NULL)
    previous->next = next;
  else
    *section = next;
}

int CountSections(struct Section *sections) {
  int count = 0;
  while (sections != NULL) {
    count++;
    sections = sections->next;
  }
  return count;
}

void RemoveSectionIfOutOfScreen(struct Section **sections) {
  while (*sections != NULL) {
    if (SectionIsOutOfScreen(*sections))
      RemoveSection(sections);
    sections = &(*sections)->next;
  }
}

void MoveSectionsDown(struct Section *sections, int offset) {
  while (sections != NULL) {
    struct WallList *wallList = sections->wallList;
    while (wallList != NULL) {
      wallList->wallStart.y += offset;
      wallList->wallEnd.y += offset;
      wallList = wallList->next;
    }
    sections = sections->next;
  }
}

bool SectionIsOutOfScreen(struct Section *section) {
  assert(section != NULL);

  struct WallList *wallList = section->wallList;
  while (wallList != NULL) {
    if (!WallIsOutOfScreen(wallList))
      return false;
    wallList = wallList->next;
  }
  return true;
}

void AddStraightSection(struct Section **sections, Vector2 offset) {
  struct Section *section = malloc(sizeof(struct Section));
  section->previous = NULL;
  section->next = NULL;
  section->wallList = NULL;
  AddSection(sections, section);
  AddWallV(&section->wallList, (Vector2){300 + offset.x, 0 + offset.y},
           (Vector2){300 + offset.x, SCREEN_HEIGHT + offset.y});
  AddWallV(&section->wallList, (Vector2){500 + offset.x, 0 + offset.y},
           (Vector2){500 + offset.x, SCREEN_HEIGHT + offset.y});
}

void AddCurveLeftSection(struct Section **sections, Vector2 offset) {
  struct Section *section = malloc(sizeof(struct Section));
  section->previous = NULL;
  section->next = NULL;
  section->wallList = NULL;
  AddSection(sections, section);
  AddWallV(&section->wallList, (Vector2){300 + offset.x, 0 + offset.y},
           (Vector2){200 + offset.x, SCREEN_HEIGHT / 2.0 + offset.y});
  AddWallV(&section->wallList, (Vector2){200 + offset.x, SCREEN_HEIGHT / 2.0 + offset.y},
           (Vector2){300 + offset.x, SCREEN_HEIGHT + offset.y});
  AddWallV(&section->wallList, (Vector2){500 + offset.x, 0 + offset.y},
           (Vector2){400 + offset.x, SCREEN_HEIGHT / 2.0 + offset.y});
  AddWallV(&section->wallList, (Vector2){400 + offset.x, SCREEN_HEIGHT / 2.0 + offset.y},
           (Vector2){500 + offset.x, SCREEN_HEIGHT + offset.y});
}

void AddCurveRightSection(struct Section **sections, Vector2 offset) {
  struct Section *section = malloc(sizeof(struct Section));
  section->previous = NULL;
  section->next = NULL;
  section->wallList = NULL;
  AddSection(sections, section);
  AddWallV(&section->wallList, (Vector2){300 + offset.x, 0 + offset.y},
           (Vector2){400 + offset.x, SCREEN_HEIGHT / 2.0 + offset.y});
  AddWallV(&section->wallList, (Vector2){400 + offset.x, SCREEN_HEIGHT / 2.0 + offset.y},
           (Vector2){300 + offset.x, SCREEN_HEIGHT + offset.y});
  AddWallV(&section->wallList, (Vector2){500 + offset.x, 0 + offset.y},
           (Vector2){600 + offset.x, SCREEN_HEIGHT / 2.0 + offset.y});
  AddWallV(&section->wallList, (Vector2){600 + offset.x, SCREEN_HEIGHT / 2.0 + offset.y},
           (Vector2){500 + offset.x, SCREEN_HEIGHT + offset.y});
}
