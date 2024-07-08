#include "main.h"
#include "include/raylib.h"
#include <stdio.h>
#include <stdlib.h>

#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

int main(void) {
  struct LoopArg *loopArg = Setup();
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Cartpole Visualization");

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

  if (CountSections(*sections) <= 0) {
    switch (rand() % 3) {
    case 0:
      AddStraightSection(sections);
      break;
    case 1:
      AddCurveLeftSection(sections);
      break;
    case 2:
      AddCurveRightSection(sections);
      break;
    }
  }

  BeginDrawing();

  ClearBackground(DARKGRAY);
  DrawSections(*sections);
  DrawCircle(player->position.x, player->position.y, player->radius, WHITE);
  DrawText(TextFormat("section count: %d", CountSections(*sections)), 0, 0, 12,
           WHITE);

  EndDrawing();
}

void FreeSections(struct Section *sections) {
  while (sections != NULL) {
    struct Section *temp = sections->next;
    FreeWallList(sections->wallList);
    free(sections);
    sections = temp;
  }
}

void FreeWallList(struct WallList *wallList) {
  while (wallList != NULL) {
    struct WallList *temp = wallList->next;
    free(wallList);
    wallList = temp;
  }
}

void AddWall(struct WallList **head, struct WallList *newWall) {
  struct WallList *wallList = malloc(sizeof(struct WallList));
  *wallList = *newWall;

  if (*head == NULL) {
    *head = wallList;
    return;
  }

  struct WallList *temp = *head;
  while (temp->next != NULL) {
    temp = temp->next;
  }
  temp->next = wallList;
  wallList->previous = temp;
}

void AddWallV(struct WallList **head, Vector2 startPoint, Vector2 endPoint) {
  struct WallList wall = {
      .wallStart = startPoint,
      .wallEnd = endPoint,
      .next = NULL,
      .previous = NULL,
  };
  AddWall(head, &wall);
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
  if (head == NULL)
    return false;
  if ((0 > head->wallEnd.x || head->wallEnd.x > SCREEN_WIDTH ||
       0 > head->wallEnd.y || head->wallEnd.y > SCREEN_HEIGHT) &&
      (0 > head->wallStart.x || head->wallStart.x > SCREEN_WIDTH ||
       0 > head->wallStart.y || head->wallStart.y > SCREEN_HEIGHT))
    return true;
  return false;
}

bool SectionIsOutOfScreen(struct Section *section) {
  if (section == NULL)
    return false;
  struct WallList *wallList = section->wallList;
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

void RemoveSection(struct Section **section) {
  struct WallList **wallList = &(*section)->wallList;
  while (*wallList != NULL) {
    RemoveWall(wallList);
    wallList = &(*wallList)->next;
  }
  free(*section);
  *section = NULL;
}

void AddSection(struct Section **sections, struct Section *newSection) {
  struct Section *section = malloc(sizeof(struct Section));
  *section = *newSection;

  if (*sections == NULL) {
    *sections = section;
    return;
  }

  struct Section *temp = *sections;
  while (temp->next != NULL) {
    temp = temp->next;
  }
  temp->next = section;
  section->previous = temp;
}

void AddStraightSection(struct Section **section) {
  struct Section newSection = {
      .next = NULL,
      .previous = NULL,
      .wallList = NULL,
  };
  AddWallV(&newSection.wallList, (Vector2){300, 0},
           (Vector2){300, SCREEN_HEIGHT});
  AddWallV(&newSection.wallList, (Vector2){500, 0},
           (Vector2){500, SCREEN_HEIGHT});
  AddSection(section, &newSection);
}

void AddCurveLeftSection(struct Section **section) {
  struct Section newSection = {
      .next = NULL,
      .previous = NULL,
      .wallList = NULL,
  };
  AddWallV(&newSection.wallList, (Vector2){300, 0},
           (Vector2){200, SCREEN_HEIGHT / 2.0});
  AddWallV(&newSection.wallList, (Vector2){200, SCREEN_HEIGHT / 2.0},
           (Vector2){300, SCREEN_HEIGHT});
  AddWallV(&newSection.wallList, (Vector2){500, 0},
           (Vector2){400, SCREEN_HEIGHT / 2.0});
  AddWallV(&newSection.wallList, (Vector2){400, SCREEN_HEIGHT / 2.0},
           (Vector2){500, SCREEN_HEIGHT});
  AddSection(section, &newSection);
}

void AddCurveRightSection(struct Section **section) {
  struct Section newSection = {
      .next = NULL,
      .previous = NULL,
      .wallList = NULL,
  };
  AddWallV(&newSection.wallList, (Vector2){300, 0},
           (Vector2){400, SCREEN_HEIGHT / 2.0});
  AddWallV(&newSection.wallList, (Vector2){400, SCREEN_HEIGHT / 2.0},
           (Vector2){300, SCREEN_HEIGHT});
  AddWallV(&newSection.wallList, (Vector2){500, 0},
           (Vector2){600, SCREEN_HEIGHT / 2.0});
  AddWallV(&newSection.wallList, (Vector2){600, SCREEN_HEIGHT / 2.0},
           (Vector2){500, SCREEN_HEIGHT});
  AddSection(section, &newSection);
}

void RemoveSectionIfOutOfScreen(struct Section **sections) {
  while (*sections != NULL) {
    if (SectionIsOutOfScreen(*sections))
      RemoveSection(sections);
    if (*sections != NULL)
      sections = &(*sections)->next;
  }
}
