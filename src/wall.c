#include "wall.h"
#include <stdio.h>
#include <stdlib.h>

void RemoveWall(struct WallList **head) {
  struct WallList *next = (*head)->next;
  struct WallList *previous = (*head)->previous;
  free(*head);
  *head = NULL;
  if (previous != NULL && next != NULL) {
    previous->next = next;
    next->previous = previous;
  } else if (previous != NULL) {
    previous->next = NULL;
  } else if (next != NULL) {
    next->previous = NULL;
  }
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
  if (head == NULL) {
    printf("got NULL as an argument in %s\n", __func__);
    return false;
  }
  printf("x: %.f, y: %.f\n", head->wallStart.x, head->wallStart.y);

  if ((0 > head->wallEnd.x || head->wallEnd.x > SCREEN_WIDTH ||
       0 > head->wallEnd.y || head->wallEnd.y > SCREEN_HEIGHT) &&
      (0 > head->wallStart.x || head->wallStart.x > SCREEN_WIDTH ||
       0 > head->wallStart.y || head->wallStart.y > SCREEN_HEIGHT))
    return true;
  return false;
}
