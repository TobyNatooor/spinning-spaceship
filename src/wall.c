#include "wall.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

void AddWall(WallNode **walls, WallNode *newWall) {
  if (*walls == NULL) {
    *walls = newWall;
    return;
  }

  WallNode *last = *walls;
  while (last->next != NULL)
    last = last->next;
  last->next = newWall;

  newWall->prev = last;
}

void AddWallV(WallNode **walls, Vector2 startPoint, Vector2 endPoint) {
  WallNode *newWallNode = malloc(sizeof(WallNode));
  newWallNode->wallStart = startPoint;
  newWallNode->wallEnd = endPoint;
  newWallNode->prev = NULL;
  newWallNode->next = NULL;
  AddWall(walls, newWallNode);
}

void RemoveWall(WallNode **walls) {
  WallNode *prev = (*walls)->prev;
  WallNode *next = (*walls)->next;
  free(*walls);
  *walls = NULL;

  if (next != NULL)
    next->prev = prev;
  if (prev != NULL)
    prev->next = next;
  else
    *walls = next;
}

int CountWalls(WallNode *walls) {
  int count = 0;
  while (walls != NULL) {
    count++;
    walls = walls->next;
  }
  return count;
}

bool WallIsOutOfScreen(WallNode *head) {
  assert(head != NULL);

  if (head->wallEnd.y > SCREEN_HEIGHT && head->wallStart.y > SCREEN_HEIGHT)
    return true;
  return false;
}
