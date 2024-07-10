#include "section.h"
#include <stdio.h>
#include <stdlib.h>

struct Section *AddSection(struct Section **sections,
                           struct WallList *wallList) {
  struct Section *section = malloc(sizeof(struct Section));
  section->wallList = wallList;
  section->previous = NULL;
  section->next = NULL;

  if (*sections == NULL) {
    *sections = section;
    return section;
  }

  struct Section *last = *sections;
  while (last->next != NULL)
    last = last->next;
  last->next = section;
  section->previous = last;
  return section;
}

void RemoveSection(struct Section **section) {
  struct WallList *wallList = (*section)->wallList;
  while (wallList != NULL) {
    struct WallList *temp = wallList->next;
    free(wallList);
    wallList = temp;
  }

  struct Section *next = (*section)->next;
  struct Section *previous = (*section)->previous;
  free(*section);
  *section = NULL;
  // previous->next = next;
  // next->previous = previous;
  if (previous != NULL && next != NULL) {
    previous->next = next;
    next->previous = previous;
    *section = previous;
  } else if (previous != NULL) {
    previous->next = NULL;
    *section = previous;
  } else if (next != NULL) {
    next->previous = NULL;
    *section = next;
  }
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
    struct Section **temp = &(*sections)->next;
    if (SectionIsOutOfScreen(*sections)) {
      printf("section is out of screen\n");
      RemoveSection(sections);
    }
    sections = temp;
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

void AddWallV(struct Section *section, Vector2 startPoint, Vector2 endPoint) {
  struct WallList *wallList = malloc(sizeof(struct WallList));
  wallList->wallStart = startPoint;
  wallList->wallEnd = endPoint;
  wallList->previous = NULL;
  wallList->next = NULL;

  if (section->wallList == NULL) {
    section->wallList = wallList;
    printf("test\n");
    return;
  } else {

    printf("123\n");
  }

  struct WallList *last = section->wallList;
  while (last->next != NULL)
    last = last->next;

  last->next = wallList;
  wallList->previous = last;
}

bool SectionIsOutOfScreen(struct Section *section) {
  if (section == NULL) {
    printf("got NULL as an argument in %s\n", __func__);
    return false;
  }

  struct WallList *wallList = section->wallList;
  if (wallList == NULL)
    printf("wallList is NULL in %s\n", __func__);
  while (wallList != NULL) {
    if (!WallIsOutOfScreen(wallList))
      return false;
    wallList = wallList->next;
  }
  return true;
}

void AddStraightSection(struct Section **sections, Vector2 offset) {
  struct Section *section = AddSection(sections, NULL);
  AddWallV(section, (Vector2){300 + offset.x, 0 + offset.y},
           (Vector2){300 + offset.x, SCREEN_HEIGHT + offset.y});
  AddWallV(section, (Vector2){500 + offset.x, 0 + offset.y},
           (Vector2){500 + offset.x, SCREEN_HEIGHT + offset.y});
}

void AddCurveLeftSection(struct Section **sections, Vector2 offset) {
  struct Section *section = AddSection(sections, NULL);
  AddWallV(section, (Vector2){300 + offset.x, 0 + offset.y},
           (Vector2){200 + offset.x, SCREEN_HEIGHT / 2.0 + offset.y});
  AddWallV(section, (Vector2){200 + offset.x, SCREEN_HEIGHT / 2.0 + offset.y},
           (Vector2){300 + offset.x, SCREEN_HEIGHT + offset.y});
  AddWallV(section, (Vector2){500 + offset.x, 0 + offset.y},
           (Vector2){400 + offset.x, SCREEN_HEIGHT / 2.0 + offset.y});
  AddWallV(section, (Vector2){400 + offset.x, SCREEN_HEIGHT / 2.0 + offset.y},
           (Vector2){500 + offset.x, SCREEN_HEIGHT + offset.y});
}

void AddCurveRightSection(struct Section **sections, Vector2 offset) {
  struct Section *section = AddSection(sections, NULL);
  AddWallV(section, (Vector2){300 + offset.x, 0 + offset.y},
           (Vector2){400 + offset.x, SCREEN_HEIGHT / 2.0 + offset.y});
  AddWallV(section, (Vector2){400 + offset.x, SCREEN_HEIGHT / 2.0 + offset.y},
           (Vector2){300 + offset.x, SCREEN_HEIGHT + offset.y});
  AddWallV(section, (Vector2){500 + offset.x, 0 + offset.y},
           (Vector2){600 + offset.x, SCREEN_HEIGHT / 2.0 + offset.y});
  AddWallV(section, (Vector2){600 + offset.x, SCREEN_HEIGHT / 2.0 + offset.y},
           (Vector2){500 + offset.x, SCREEN_HEIGHT + offset.y});
}
