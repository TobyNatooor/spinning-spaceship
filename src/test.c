#include "test.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
  TestWall();
  TestSection();

  return 0;
}

void TestWall() {
  printf("testing walls...\n");

  WallNode *walls = NULL;
  assert(CountWalls(walls) == 0);

  WallNode *wall1 = malloc(sizeof(WallNode));
  wall1->prev = NULL;
  wall1->next = NULL;
  wall1->wallStart = (Vector2){1, 1};
  wall1->wallEnd = (Vector2){1, 1};
  AddWall(&walls, wall1);
  assert(CountWalls(walls) == 1);

  WallNode *wall2 = malloc(sizeof(WallNode));
  wall2->prev = NULL;
  wall2->next = NULL;
  wall2->wallStart = (Vector2){2, 2};
  wall2->wallEnd = (Vector2){2, 2};
  AddWall(&walls, wall2);
  assert(CountWalls(walls) == 2);

  WallNode *wall3 = malloc(sizeof(WallNode));
  wall3->prev = NULL;
  wall3->next = NULL;
  wall3->wallStart = (Vector2){3, 3};
  wall3->wallEnd = (Vector2){3, 3};
  AddWall(&walls, wall3);
  assert(CountWalls(walls) == 3);

  printf("added walls succesfully\n");

  assert(walls->wallStart.x == 1);
  assert(walls->next->wallStart.x == 2);
  assert(walls->next->next->wallStart.x == 3);

  printf("order of walls is correct\n");

  RemoveWall(&walls->next);
  assert(CountWalls(walls) == 2);
  assert(walls->wallStart.x == 1);
  assert(walls->next->wallStart.x == 3);

  RemoveWall(&walls);
  assert(CountWalls(walls) == 1);
  assert(walls->wallStart.x == 3);

  RemoveWall(&walls);
  assert(CountWalls(walls) == 0);
  assert(walls == NULL);

  printf("removed walls succesfully\n");
}

void TestSection() {
  printf("testing sections...\n");

  SectionNode *sections = NULL;
  assert(CountSections(sections) == 0);

  SectionNode *section1 = malloc(sizeof(SectionNode));
  section1->prev = NULL;
  section1->next = NULL;
  section1->walls = NULL;
  AddSection(&sections, section1);
  assert(CountSections(sections) == 1);
  assert(CountWalls(section1->walls) == 0);

  SectionNode *section2 = malloc(sizeof(SectionNode));
  section2->prev = NULL;
  section2->next = NULL;
  section2->walls = NULL;
  AddSection(&sections, section2);
  assert(CountSections(sections) == 2);

  WallNode *wallNode1 = malloc(sizeof(WallNode));
  wallNode1->prev = NULL;
  wallNode1->next = NULL;
  wallNode1->wallStart = (Vector2){1, 1};
  wallNode1->wallEnd = (Vector2){1, 1};
  AddWall(&section2->walls, wallNode1);
  assert(CountWalls(section2->walls) == 1);

  SectionNode *section3 = malloc(sizeof(SectionNode));
  section3->prev = NULL;
  section3->next = NULL;
  section3->walls = NULL;
  AddSection(&sections, section3);
  assert(CountSections(sections) == 3);
  assert(CountWalls(section3->walls) == 0);

  printf("added sections succesfully\n");

  RemoveSection(&sections->next->next);
  assert(CountSections(sections) == 2);
  RemoveSection(&sections);
  assert(CountSections(sections) == 1);
  assert(CountWalls(sections->walls) == 1);
  RemoveSection(&sections);
  assert(CountSections(sections) == 0);

  printf("removed sections succesfully\n");
}
