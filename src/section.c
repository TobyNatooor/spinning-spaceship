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
  LineNode *wallNode = (*section)->walls;
  while (wallNode != NULL)
    RemoveLine(&wallNode);

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

bool SectionIsOutOfScreen(SectionNode *section) {
  assert(section != NULL);

  LineNode *wallNode = section->walls;
  while (wallNode != NULL) {
    if (!LineIsOutOfScreen(wallNode))
      return false;
    wallNode = wallNode->next;
  }
  return true;
}

void MoveSection(SectionNode *section, Vector2 offset) {
  LineNode *wall = section->walls;
  while (wall != NULL) {
    wall->start.x += offset.x;
    wall->start.y += offset.y;
    wall->end.x += offset.x;
    wall->end.y += offset.y;
    wall = wall->next;
  }
}

void MoveSections(SectionNode *sections, Vector2 offset) {
  while (sections != NULL) {
    MoveSection(sections, offset);
    sections = sections->next;
  }
}

float GetSectionTopY(SectionNode *section) {
  if (section == NULL)
    return 0;
  LineNode *wall = section->walls;
  float topY = wall->start.y;
  while (wall != NULL) {
    if (wall->start.y < topY)
      topY = wall->start.y;
    if (wall->end.y < topY)
      topY = wall->end.y;
    wall = wall->next;
  }
  return topY;
}

void AddSectionFrom(SectionNode **sections, LineNode *walls) {
  SectionNode *section = malloc(sizeof(SectionNode));
  section->prev = NULL;
  section->next = NULL;
  section->walls = walls;

  float topY = GetSectionTopY(*sections);
  MoveSection(section,
              (Vector2){0, topY - SCREEN_HEIGHT * CountSections(*sections)});
  AddSection(sections, section);
}

void AddStraightSection(SectionNode **sections) {
  LineNode *walls = NULL;
  AddLineV(&walls, (Vector2){SCREEN_WIDTH * 0.3, 0},
           (Vector2){SCREEN_WIDTH * 0.3, SCREEN_HEIGHT});
  AddLineV(&walls, (Vector2){SCREEN_WIDTH * 0.7, 0},
           (Vector2){SCREEN_WIDTH * 0.7, SCREEN_HEIGHT});
  AddSectionFrom(sections, walls);
}

void AddCurveLeftSection(SectionNode **sections) {
  LineNode *walls = NULL;
  AddLineV(&walls, (Vector2){SCREEN_WIDTH * 0.3, 0},
           (Vector2){SCREEN_WIDTH * 0.1, SCREEN_HEIGHT / 2.0});
  AddLineV(&walls, (Vector2){SCREEN_WIDTH * 0.1, SCREEN_HEIGHT / 2.0},
           (Vector2){SCREEN_WIDTH * 0.3, SCREEN_HEIGHT});
  AddLineV(&walls, (Vector2){SCREEN_WIDTH * 0.7, 0},
           (Vector2){SCREEN_WIDTH * 0.5, SCREEN_HEIGHT / 2.0});
  AddLineV(&walls, (Vector2){SCREEN_WIDTH * 0.5, SCREEN_HEIGHT / 2.0},
           (Vector2){SCREEN_WIDTH * 0.7, SCREEN_HEIGHT});
  AddSectionFrom(sections, walls);
}

void AddCurveRightSection(SectionNode **sections) {
  LineNode *walls = NULL;
  AddLineV(&walls, (Vector2){SCREEN_WIDTH * 0.3, 0},
           (Vector2){SCREEN_WIDTH * 0.5, SCREEN_HEIGHT / 2.0});
  AddLineV(&walls, (Vector2){SCREEN_WIDTH * 0.5, SCREEN_HEIGHT / 2.0},
           (Vector2){SCREEN_WIDTH * 0.3, SCREEN_HEIGHT});
  AddLineV(&walls, (Vector2){SCREEN_WIDTH * 0.7, 0},
           (Vector2){SCREEN_WIDTH * 0.9, SCREEN_HEIGHT / 2.0});
  AddLineV(&walls, (Vector2){SCREEN_WIDTH * 0.9, SCREEN_HEIGHT / 2.0},
           (Vector2){SCREEN_WIDTH * 0.7, SCREEN_HEIGHT});
  AddSectionFrom(sections, walls);
}
