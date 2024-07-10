#include "test.h"
#include "include/raylib.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
  WallTest();

  return 0;
}

void WallTest() {
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

void SectionTest() {
}
