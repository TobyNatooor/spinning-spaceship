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
  printf("added new section, total: %d\n", CountSections(*sections));
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
  printf("removed section\n");
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

  WallNode *wallNode = section->walls;
  while (wallNode != NULL) {
    if (!WallIsOutOfScreen(wallNode))
      return false;
    wallNode = wallNode->next;
  }
  return true;
}

void MoveSection(SectionNode *section, Vector2 offset) {
  WallNode *wall = section->walls;
  while (wall != NULL) {
    wall->wallStart.x += offset.x;
    wall->wallStart.y += offset.y;
    wall->wallEnd.x += offset.x;
    wall->wallEnd.y += offset.y;
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
  WallNode *wall = section->walls;
  float topY = wall->wallStart.y;
  while (wall != NULL) {
    if (wall->wallStart.y < topY)
      topY = wall->wallStart.y;
    if (wall->wallEnd.y < topY)
      topY = wall->wallEnd.y;
    wall = wall->next;
  }
  return topY;
}

void AddSectionFrom(SectionNode **sections, WallNode *walls) {
  SectionNode *section = malloc(sizeof(SectionNode));
  section->prev = NULL;
  section->next = NULL;
  section->walls = walls;

  float topY = GetSectionTopY(*sections);
  MoveSection(section, (Vector2){0, topY - SCREEN_HEIGHT * CountSections(*sections)});
  AddSection(sections, section);
}

void AddStraightSection(SectionNode **sections) {
  WallNode *walls = NULL;
  AddWallV(&walls, (Vector2){300, 0}, (Vector2){300, SCREEN_HEIGHT});
  AddWallV(&walls, (Vector2){500, 0}, (Vector2){500, SCREEN_HEIGHT});
  AddSectionFrom(sections, walls);
}

void AddCurveLeftSection(SectionNode **sections) {
  WallNode *walls = NULL;
  AddWallV(&walls, (Vector2){300, 0}, (Vector2){200, SCREEN_HEIGHT / 2.0});
  AddWallV(&walls, (Vector2){200, SCREEN_HEIGHT / 2.0},
           (Vector2){300, SCREEN_HEIGHT});
  AddWallV(&walls, (Vector2){500, 0}, (Vector2){400, SCREEN_HEIGHT / 2.0});
  AddWallV(&walls, (Vector2){400, SCREEN_HEIGHT / 2.0},
           (Vector2){500, SCREEN_HEIGHT});
  AddSectionFrom(sections, walls);
}

void AddCurveRightSection(SectionNode **sections) {
  WallNode *walls = NULL;
  AddWallV(&walls, (Vector2){300, 0}, (Vector2){400, SCREEN_HEIGHT / 2.0});
  AddWallV(&walls, (Vector2){400, SCREEN_HEIGHT / 2.0},
           (Vector2){300, SCREEN_HEIGHT});
  AddWallV(&walls, (Vector2){500, 0}, (Vector2){600, SCREEN_HEIGHT / 2.0});
  AddWallV(&walls, (Vector2){600, SCREEN_HEIGHT / 2.0},
           (Vector2){500, SCREEN_HEIGHT});
  AddSectionFrom(sections, walls);
}
