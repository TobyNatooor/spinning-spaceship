#include "wall.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

void AddWall(struct WallList **wallList, struct WallList *newWallList) {
  if (*wallList == NULL) {
    *wallList = newWallList;
    return;
  }

  struct WallList *last = *wallList;
  while (last->next != NULL)
    last = last->next;
  last->next = newWallList;

  newWallList->previous = last;
}

void AddWallV(struct WallList **wallList, Vector2 startPoint, Vector2 endPoint) {
  struct WallList *newWallList = malloc(sizeof(struct WallList));
  newWallList->wallStart = startPoint;
  newWallList->wallEnd = endPoint;
  newWallList->previous = NULL;
  newWallList->next = NULL;
  AddWall(wallList, newWallList);
}

void RemoveWall(struct WallList **wallList) {
  struct WallList *previous = (*wallList)->previous;
  struct WallList *next = (*wallList)->next;
  free(*wallList);
  *wallList = NULL;

  if (next != NULL)
    next->previous = previous;
  if (previous != NULL)
    previous->next = next;
  else
    *wallList = next;
}

int CountWalls(struct WallList *wallList) {
  int count = 0;
  while (wallList != NULL) {
    count++;
    wallList = wallList->next;
  }
  return count;
}

bool WallIsOutOfScreen(struct WallList *head) {
  assert(head != NULL);

  if ((0 > head->wallEnd.x || head->wallEnd.x > SCREEN_WIDTH ||
       0 > head->wallEnd.y || head->wallEnd.y > SCREEN_HEIGHT) &&
      (0 > head->wallStart.x || head->wallStart.x > SCREEN_WIDTH ||
       0 > head->wallStart.y || head->wallStart.y > SCREEN_HEIGHT))
    return true;
  return false;
}
