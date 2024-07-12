#include "wall.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

void AddLine(LineNode **lines, LineNode *newLine) {
  if (*lines == NULL) {
    *lines = newLine;
    return;
  }

  LineNode *last = *lines;
  while (last->next != NULL)
    last = last->next;
  last->next = newLine;

  newLine->prev = last;
}

void AddLineV(LineNode **lines, Vector2 startPoint, Vector2 endPoint) {
  LineNode *newLineNode = malloc(sizeof(LineNode));
  newLineNode->start = startPoint;
  newLineNode->end = endPoint;
  newLineNode->prev = NULL;
  newLineNode->next = NULL;
  AddLine(lines, newLineNode);
}

void RemoveLine(LineNode **lines) {
  LineNode *prev = (*lines)->prev;
  LineNode *next = (*lines)->next;
  free(*lines);
  *lines = NULL;

  if (next != NULL)
    next->prev = prev;
  if (prev != NULL)
    prev->next = next;
  else
    *lines = next;
}

int CountLines(LineNode *head) {
  int count = 0;
  while (head != NULL) {
    count++;
    head = head->next;
  }
  return count;
}

bool LineIsOutOfScreen(LineNode *head) {
  assert(head != NULL);

  if (head->end.y > SCREEN_HEIGHT && head->start.y > SCREEN_HEIGHT)
    return true;
  return false;
}
