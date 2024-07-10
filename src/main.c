#include "main.h"
#include "include/raylib.h"
#include <stdio.h>
#include <stdlib.h>

#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

int main(void) {
  struct LoopArg *loopArg = Setup();
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Spinning Spaceship");

#if defined(PLATFORM_WEB)
  emscripten_set_main_loop_arg(Loop, loopArg, 0, 1);
#else
  SetTargetFPS(60);

  while (!WindowShouldClose()) {
    Loop(loopArg);
  }
#endif

  CloseWindow();

  free(loopArg->player);
  while (loopArg->sections != NULL) {
    RemoveSection(&loopArg->sections);
    loopArg->sections = loopArg->sections->next;
  }
  free(loopArg);

  return 0;
}

struct LoopArg *Setup(void) {
  struct Player *player = malloc(sizeof(struct Player));
  player->position.x = SCREEN_WIDTH / 2.0;
  player->position.y = SCREEN_HEIGHT / 2.0;
  player->radius = 50;

  struct LoopArg *loopArg = malloc(sizeof(struct LoopArg));
  loopArg->player = player;
  loopArg->sections = NULL;
  AddStraightSection(&loopArg->sections, (Vector2){0, 0});

  return loopArg;
}

void Loop(void *loopArg_) {
  struct LoopArg *arg = loopArg_;
  struct Player *player = arg->player;
  struct Section **sections = &arg->sections;

  float frameTime = GetFrameTime();

  if (IsKeyDown(KEY_A))
    player->position.x -= 100 * frameTime;
  if (IsKeyDown(KEY_D))
    player->position.x += 100 * frameTime;
  if (IsKeyDown(KEY_W))
    player->position.y -= 100 * frameTime;
  if (IsKeyDown(KEY_S))
    player->position.y += 100 * frameTime;

  MoveSectionsDown(*sections, 200 * frameTime);

  RemoveSectionIfOutOfScreen(sections);

  if (IsPlayerCollidingWalls(player, *sections))
    printf("hit wall\n");

  if (CountSections(*sections) <= 1) {
    switch (rand() % 3) {
    case 0:
      AddStraightSection(sections, (Vector2){0, -SCREEN_HEIGHT});
      break;
    case 1:
      AddCurveLeftSection(sections, (Vector2){0, -SCREEN_HEIGHT});
      break;
    case 2:
      AddCurveRightSection(sections, (Vector2){0, -SCREEN_HEIGHT});
      break;
    }
  }

  BeginDrawing();

  ClearBackground(DARKGRAY);
  DrawSections(*sections);
  DrawCircle(player->position.x, player->position.y, player->radius, WHITE);
  DrawText(TextFormat("section count: %d\nwall count: %d",
                      CountSections(*sections),
                      CountWalls((*sections)->wallList)),
           0, 0, 12, WHITE);

  EndDrawing();
}

void AddWallV(struct Section *section, Vector2 startPoint, Vector2 endPoint) {
  struct WallList *wallList = malloc(sizeof(struct WallList));
  wallList->wallStart = startPoint;
  wallList->wallEnd = endPoint;
  wallList->previous = NULL;
  wallList->next = NULL;

  if (section->wallList == NULL) {
    section->wallList = wallList;
    printf("test\n");
    return;
  } else {

    printf("123\n");
  }

  struct WallList *last = section->wallList;
  while (last->next != NULL)
    last = last->next;

  last->next = wallList;
  wallList->previous = last;
}

int CountWalls(struct WallList *wallList) {
  int count = 0;
  while (wallList != NULL) {
    count++;
    wallList = wallList->next;
  }
  return count;
}

int CountSections(struct Section *sections) {
  int count = 0;
  while (sections != NULL) {
    count++;
    sections = sections->next;
  }
  return count;
}

void MoveSectionsDown(struct Section *sections, int offset) {
  while (sections != NULL) {
    struct WallList *wallList = sections->wallList;
    while (wallList != NULL) {
      wallList->wallStart.y += offset;
      wallList->wallEnd.y += offset;
      wallList = wallList->next;
    }
    sections = sections->next;
  }
}

void DrawSections(struct Section *sections) {
  while (sections != NULL) {
    struct WallList *wallList = sections->wallList;
    while (wallList != NULL) {
      DrawLineV(wallList->wallStart, wallList->wallEnd, WHITE);
      wallList = wallList->next;
    }
    sections = sections->next;
  }
}

bool IsPlayerCollidingWalls(struct Player *player, struct Section *sections) {
  while (sections != NULL) {
    struct WallList *wallList = sections->wallList;
    while (wallList != NULL) {
      if (CheckCollisionCircleLine(player->position, player->radius,
                                   wallList->wallStart, wallList->wallEnd))
        return true;
      wallList = wallList->next;
    }
    sections = sections->next;
  }
  return false;
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

bool SectionIsOutOfScreen(struct Section *section) {
  if (section == NULL) {
    printf("got NULL as an argument in %s\n", __func__);
    return false;
  }

  struct WallList *wallList = section->wallList;
  if (wallList == NULL)
    printf("wallList is NULL in %s\n", __func__);
  while (wallList != NULL) {
    if (!WallIsOutOfScreen(wallList))
      return false;
    wallList = wallList->next;
  }
  return true;
}

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

// struct Section *GetFirstSection(struct Section *sections) {
//   while (sections->previous != NULL)
//     sections = sections->previous;
//   return sections;
// }

void RemoveSection(struct Section **section) {
  struct WallList *wallList = (*section)->wallList;
  while (wallList != NULL) {
    struct WallList *temp = wallList->next;
    free(wallList);
    wallList = temp;
  }

  struct Section *next = (*section)->next;
  struct Section *previous = (*section)->previous;
  free(*section);
  *section = NULL;
  // previous->next = next;
  // next->previous = previous;
  if (previous != NULL && next != NULL) {
    previous->next = next;
    next->previous = previous;
    *section = previous;
  } else if (previous != NULL) {
    previous->next = NULL;
    *section = previous;
  } else if (next != NULL) {
    next->previous = NULL;
    *section = next;
  }
}

struct Section *AddSection(struct Section **sections,
                           struct WallList *wallList) {
  struct Section *section = malloc(sizeof(struct Section));
  section->wallList = wallList;
  section->previous = NULL;
  section->next = NULL;

  if (*sections == NULL) {
    *sections = section;
    return section;
  }

  struct Section *last = *sections;
  while (last->next != NULL)
    last = last->next;
  last->next = section;
  section->previous = last;
  return section;
}

void RemoveSectionIfOutOfScreen(struct Section **sections) {
  while (*sections != NULL) {
    struct Section **temp = &(*sections)->next;
    if (SectionIsOutOfScreen(*sections)) {
      printf("section is out of screen\n");
      RemoveSection(sections);
    }
    sections = temp;
  }
}

void AddStraightSection(struct Section **sections, Vector2 offset) {
  struct Section *section = AddSection(sections, NULL);
  AddWallV(section, (Vector2){300 + offset.x, 0 + offset.y},
           (Vector2){300 + offset.x, SCREEN_HEIGHT + offset.y});
  AddWallV(section, (Vector2){500 + offset.x, 0 + offset.y},
           (Vector2){500 + offset.x, SCREEN_HEIGHT + offset.y});
}

void AddCurveLeftSection(struct Section **sections, Vector2 offset) {
  struct Section *section = AddSection(sections, NULL);
  AddWallV(section, (Vector2){300 + offset.x, 0 + offset.y},
           (Vector2){200 + offset.x, SCREEN_HEIGHT / 2.0 + offset.y});
  AddWallV(section, (Vector2){200 + offset.x, SCREEN_HEIGHT / 2.0 + offset.y},
           (Vector2){300 + offset.x, SCREEN_HEIGHT + offset.y});
  AddWallV(section, (Vector2){500 + offset.x, 0 + offset.y},
           (Vector2){400 + offset.x, SCREEN_HEIGHT / 2.0 + offset.y});
  AddWallV(section, (Vector2){400 + offset.x, SCREEN_HEIGHT / 2.0 + offset.y},
           (Vector2){500 + offset.x, SCREEN_HEIGHT + offset.y});
}

void AddCurveRightSection(struct Section **sections, Vector2 offset) {
  struct Section *section = AddSection(sections, NULL);
  AddWallV(section, (Vector2){300 + offset.x, 0 + offset.y},
           (Vector2){400 + offset.x, SCREEN_HEIGHT / 2.0 + offset.y});
  AddWallV(section, (Vector2){400 + offset.x, SCREEN_HEIGHT / 2.0 + offset.y},
           (Vector2){300 + offset.x, SCREEN_HEIGHT + offset.y});
  AddWallV(section, (Vector2){500 + offset.x, 0 + offset.y},
           (Vector2){600 + offset.x, SCREEN_HEIGHT / 2.0 + offset.y});
  AddWallV(section, (Vector2){600 + offset.x, SCREEN_HEIGHT / 2.0 + offset.y},
           (Vector2){500 + offset.x, SCREEN_HEIGHT + offset.y});
}
