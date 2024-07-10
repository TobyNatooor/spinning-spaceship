#include "test.h"
#include "include/raylib.h"
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

  struct WallList *wallList = NULL;
  assert(CountWalls(wallList) == 0);

  struct WallList *wallList1 = malloc(sizeof(struct WallList));
  wallList1->previous = NULL;
  wallList1->next = NULL;
  wallList1->wallStart = (Vector2){1, 1};
  wallList1->wallEnd = (Vector2){1, 1};
  AddWall(&wallList, wallList1);
  assert(CountWalls(wallList) == 1);

  struct WallList *wallList2 = malloc(sizeof(struct WallList));
  wallList2->previous = NULL;
  wallList2->next = NULL;
  wallList2->wallStart = (Vector2){2, 2};
  wallList2->wallEnd = (Vector2){2, 2};
  AddWall(&wallList, wallList2);
  assert(CountWalls(wallList) == 2);

  struct WallList *wallList3 = malloc(sizeof(struct WallList));
  wallList3->previous = NULL;
  wallList3->next = NULL;
  wallList3->wallStart = (Vector2){3, 3};
  wallList3->wallEnd = (Vector2){3, 3};
  AddWall(&wallList, wallList3);
  assert(CountWalls(wallList) == 3);

  printf("added walls succesfully\n");

  assert(wallList->wallStart.x == 1);
  assert(wallList->next->wallStart.x == 2);
  assert(wallList->next->next->wallStart.x == 3);

  printf("order of walls is correct\n");

  RemoveWall(&wallList->next);
  assert(CountWalls(wallList) == 2);
  assert(wallList->wallStart.x == 1);
  assert(wallList->next->wallStart.x == 3);

  RemoveWall(&wallList);
  assert(CountWalls(wallList) == 1);
  assert(wallList->wallStart.x == 3);

  RemoveWall(&wallList);
  assert(CountWalls(wallList) == 0);
  assert(wallList == NULL);

  printf("removed walls succesfully\n");
}

void TestSection() {
  printf("testing sections...\n");

  struct Section *sections = NULL;
  assert(CountSections(sections) == 0);

  struct Section *section1 = malloc(sizeof(struct Section));
  section1->previous = NULL;
  section1->next = NULL;
  section1->wallList = NULL;
  AddSection(&sections, section1);
  assert(CountSections(sections) == 1);
  assert(CountWalls(section1->wallList) == 0);

  struct Section *section2 = malloc(sizeof(struct Section));
  section2->previous = NULL;
  section2->next = NULL;
  section2->wallList = NULL;
  AddSection(&sections, section2);
  assert(CountSections(sections) == 2);

  struct WallList *wallList1 = malloc(sizeof(struct WallList));
  wallList1->previous = NULL;
  wallList1->next = NULL;
  wallList1->wallStart = (Vector2){1, 1};
  wallList1->wallEnd = (Vector2){1, 1};
  AddWall(&section2->wallList, wallList1);
  assert(CountWalls(section2->wallList) == 1);

  struct Section *section3 = malloc(sizeof(struct Section));
  section3->previous = NULL;
  section3->next = NULL;
  section3->wallList = NULL;
  AddSection(&sections, section3);
  assert(CountSections(sections) == 3);
  assert(CountWalls(section3->wallList) == 0);

  printf("added sections succesfully\n");

  RemoveSection(&sections->next->next);
  assert(CountSections(sections) == 2);
  RemoveSection(&sections);
  assert(CountSections(sections) == 1);
  assert(CountWalls(sections->wallList) == 1);
  RemoveSection(&sections);
  assert(CountSections(sections) == 0);

  printf("removed sections succesfully\n");
}
