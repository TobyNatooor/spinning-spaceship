#include "test.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
  TestLine();
  TestSection();

  return 0;
}

void TestLine() {
  printf("testing walls...\n");

  LineNode *lines = NULL;
  assert(CountLines(lines) == 0);

  LineNode *line1 = malloc(sizeof(LineNode));
  line1->prev = NULL;
  line1->next = NULL;
  line1->start = (Vector2){1, 1};
  line1->end = (Vector2){1, 1};
  AddLine(&lines, line1);
  assert(CountLines(lines) == 1);

  LineNode *line2 = malloc(sizeof(LineNode));
  line2->prev = NULL;
  line2->next = NULL;
  line2->start = (Vector2){2, 2};
  line2->end = (Vector2){2, 2};
  AddLine(&lines, line2);
  assert(CountLines(lines) == 2);

  LineNode *line3 = malloc(sizeof(LineNode));
  line3->prev = NULL;
  line3->next = NULL;
  line3->start = (Vector2){3, 3};
  line3->end = (Vector2){3, 3};
  AddLine(&lines, line3);
  assert(CountLines(lines) == 3);

  printf("added walls succesfully\n");

  assert(lines->start.x == 1);
  assert(lines->next->start.x == 2);
  assert(lines->next->next->start.x == 3);

  printf("order of walls is correct\n");

  RemoveLine(&lines->next);
  assert(CountLines(lines) == 2);
  assert(lines->start.x == 1);
  assert(lines->next->start.x == 3);

  RemoveLine(&lines);
  assert(CountLines(lines) == 1);
  assert(lines->start.x == 3);

  RemoveLine(&lines);
  assert(CountLines(lines) == 0);
  assert(lines == NULL);

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
  assert(CountLines(section1->walls) == 0);

  SectionNode *section2 = malloc(sizeof(SectionNode));
  section2->prev = NULL;
  section2->next = NULL;
  section2->walls = NULL;
  AddSection(&sections, section2);
  assert(CountSections(sections) == 2);

  LineNode *wallNode1 = malloc(sizeof(LineNode));
  wallNode1->prev = NULL;
  wallNode1->next = NULL;
  wallNode1->start = (Vector2){1, 1};
  wallNode1->end = (Vector2){1, 1};
  AddLine(&section2->walls, wallNode1);
  assert(CountLines(section2->walls) == 1);

  SectionNode *section3 = malloc(sizeof(SectionNode));
  section3->prev = NULL;
  section3->next = NULL;
  section3->walls = NULL;
  AddSection(&sections, section3);
  assert(CountSections(sections) == 3);
  assert(CountLines(section3->walls) == 0);

  printf("added sections succesfully\n");

  RemoveSection(&sections->next->next);
  assert(CountSections(sections) == 2);
  RemoveSection(&sections);
  assert(CountSections(sections) == 1);
  assert(CountLines(sections->walls) == 1);
  RemoveSection(&sections);
  assert(CountSections(sections) == 0);

  printf("removed sections succesfully\n");
}
