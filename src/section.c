#include "section.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

void AddSection(SectionNode **sections, SectionNode *newSection) {
  if (*sections == NULL) {
    *sections = newSection;
    return;
  }

  SectionNode *last = *sections;
  while (last->next != NULL)
    last = last->next;

  last->next = newSection;
  newSection->prev = last;
}

void RemoveSection(SectionNode **section) {
  WallNode *wallNode = (*section)->walls;
  while (wallNode != NULL)
    RemoveWall(&wallNode);

  SectionNode *prev = (*section)->prev;
  SectionNode *next = (*section)->next;
  free(*section);
  *section = NULL;
  if (next != NULL)
    next->prev = prev;
  if (prev != NULL)
    prev->next = next;
  else
    *section = next;
}

int CountSections(SectionNode *sections) {
  int count = 0;
  while (sections != NULL) {
    count++;
    sections = sections->next;
  }
  return count;
}

void RemoveSectionIfOutOfScreen(SectionNode **sections) {
  while (*sections != NULL) {
    if (SectionIsOutOfScreen(*sections))
      RemoveSection(sections);
    sections = &(*sections)->next;
  }
}

void MoveSectionsDown(SectionNode *sections, int offset) {
  while (sections != NULL) {
    WallNode *wallNode = sections->walls;
    while (wallNode != NULL) {
      wallNode->wallStart.y += offset;
      wallNode->wallEnd.y += offset;
      wallNode = wallNode->next;
    }
    sections = sections->next;
  }
}

bool SectionIsOutOfScreen(SectionNode *section) {
  assert(section != NULL);

  WallNode *wallNode = section->walls;
  while (wallNode != NULL) {
    if (!WallIsOutOfScreen(wallNode))
      return false;
    wallNode = wallNode->next;
  }
  return true;
}

void AddStraightSection(SectionNode **sections, Vector2 offset) {
  SectionNode *section = malloc(sizeof(SectionNode));
  section->prev = NULL;
  section->next = NULL;
  section->walls = NULL;
  AddSection(sections, section);
  AddWallV(&section->walls, (Vector2){300 + offset.x, 0 + offset.y},
           (Vector2){300 + offset.x, SCREEN_HEIGHT + offset.y});
  AddWallV(&section->walls, (Vector2){500 + offset.x, 0 + offset.y},
           (Vector2){500 + offset.x, SCREEN_HEIGHT + offset.y});
}

void AddCurveLeftSection(SectionNode **sections, Vector2 offset) {
  SectionNode *section = malloc(sizeof(SectionNode));
  section->prev = NULL;
  section->next = NULL;
  section->walls = NULL;
  AddSection(sections, section);
  AddWallV(&section->walls, (Vector2){300 + offset.x, 0 + offset.y},
           (Vector2){200 + offset.x, SCREEN_HEIGHT / 2.0 + offset.y});
  AddWallV(&section->walls,
           (Vector2){200 + offset.x, SCREEN_HEIGHT / 2.0 + offset.y},
           (Vector2){300 + offset.x, SCREEN_HEIGHT + offset.y});
  AddWallV(&section->walls, (Vector2){500 + offset.x, 0 + offset.y},
           (Vector2){400 + offset.x, SCREEN_HEIGHT / 2.0 + offset.y});
  AddWallV(&section->walls,
           (Vector2){400 + offset.x, SCREEN_HEIGHT / 2.0 + offset.y},
           (Vector2){500 + offset.x, SCREEN_HEIGHT + offset.y});
}

void AddCurveRightSection(SectionNode **sections, Vector2 offset) {
  SectionNode *section = malloc(sizeof(SectionNode));
  section->prev = NULL;
  section->next = NULL;
  section->walls = NULL;
  AddSection(sections, section);
  AddWallV(&section->walls, (Vector2){300 + offset.x, 0 + offset.y},
           (Vector2){400 + offset.x, SCREEN_HEIGHT / 2.0 + offset.y});
  AddWallV(&section->walls,
           (Vector2){400 + offset.x, SCREEN_HEIGHT / 2.0 + offset.y},
           (Vector2){300 + offset.x, SCREEN_HEIGHT + offset.y});
  AddWallV(&section->walls, (Vector2){500 + offset.x, 0 + offset.y},
           (Vector2){600 + offset.x, SCREEN_HEIGHT / 2.0 + offset.y});
  AddWallV(&section->walls,
           (Vector2){600 + offset.x, SCREEN_HEIGHT / 2.0 + offset.y},
           (Vector2){500 + offset.x, SCREEN_HEIGHT + offset.y});
}
