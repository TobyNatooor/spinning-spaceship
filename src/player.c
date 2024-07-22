#include "player.h"

void MovePlayer(Player *player) {
  Vector2 forwardDirection = {cos(player->rotation - PI / 2),
                              sin(player->rotation - PI / 2)};
  Vector2 rightDirection = {cos(player->rotation), sin(player->rotation)};

  float speed = 1.5;
  Vector2 direction = {0, 0};
  if (IsKeyDown(KEY_A)) {
    direction.x -= rightDirection.x * speed;
    direction.y -= rightDirection.y * speed;
  }
  if (IsKeyDown(KEY_D)) {
    direction.x += rightDirection.x * speed;
    direction.y += rightDirection.y * speed;
  }
  if (IsKeyDown(KEY_W)) {
    direction.x += forwardDirection.x * speed;
    direction.y += forwardDirection.y * speed;
  }
  if (IsKeyDown(KEY_S)) {
    direction.x -= forwardDirection.x * speed;
    direction.y -= forwardDirection.y * speed;
  }

  if (player->position.x + direction.x < 0)
    direction.x = player->position.x - direction.x;
  if (player->position.x + direction.x > SCREEN_WIDTH)
    direction.x = -SCREEN_WIDTH + player->position.x;

  if (player->position.y + direction.y < 0)
    direction.y = player->position.y - direction.y;
  if (player->position.y + direction.y > SCREEN_HEIGHT)
    direction.y = -SCREEN_HEIGHT + player->position.y;

  player->position.x += direction.x;
  player->position.y += direction.y;

  Vector2 *points = player->points;
  for (int i = 0; i < PLAYER_POINTS; i++) {
    points[i].x += direction.x;
    points[i].y += direction.y;
  }
}

bool IsPlayerCollidingWalls(Player *player, SectionNode *sections) {
  while (sections != NULL) {
    LineNode *wallList = sections->walls;
    while (wallList != NULL) {
      Vector2 *points = player->points;
      for (int i = 0; i < PLAYER_POINTS; i++) {
        if (CheckCollisionLines(wallList->start, wallList->end, points[i],
                                points[(i + 1) % PLAYER_POINTS], NULL))
          return true;
      }
      wallList = wallList->next;
    }
    sections = sections->next;
  }
  return false;
}

void RotatePlayer(Player *player) {
  float spinRate = GetFrameTime() + player->score / 10000;
  player->rotation += spinRate;

  Vector2 *points = player->points;
  for (int i = 0; i < PLAYER_POINTS; i++) {
    float adjacent = points[i].x - player->position.x;
    float opposite = points[i].y - player->position.y;
    points[i].x = player->position.x + adjacent * cos(spinRate) -
                  opposite * sin(spinRate);
    points[i].y = player->position.y + adjacent * sin(spinRate) +
                  opposite * cos(spinRate);
  }
}

