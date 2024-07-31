#include <raylib.h>

#include "config.hpp"
#include "enemy.hpp"
#include "util.hpp"

Enemy::Enemy(Texture2D tex) {
  Resize(GetWindowSize(), GetWindowSize());
  pos.y = -size.y;
  pos.x = GetRandomValue(size.x, GetWindowWidth() - size.x);
  this->tex = tex;
}

Enemy::~Enemy() {}

void Enemy::Update() {
  Vector2 vel;
  vel.y = speed.y;
  pos += vel * GetFrameTime();
}

void Enemy::Draw() {
  // DrawRectangleV(pos, size, ENEMY_COLOR);
  Rectangle enemyRec = {pos.x, pos.y, size.x, size.y};
  DrawTexturePro(tex, {0, 0, (float)tex.width, (float)tex.height}, enemyRec,
                 {0, 0}, 0, WHITE);
  if (DEBUG) {
    DrawCircleLinesV({pos.x + size.x / 2, pos.y + size.y / 2}, size.x / 2,
                     DEBUG_COLL_LINE_COLOR);
  }
}

void Enemy::Resize(Vector2 old, Vector2 nnew) {
  speed = CalculateSpeed(ENEMY_SPEED_RATIO);
  size = CalculateSize(ENEMY_SIZE_RATIO);
  pos = MaintainPosAfterResize(old, nnew, pos);
}

Rectangle Enemy::GetRec() {
  return {pos.x + size.x / 2, pos.y + size.y / 2, size.x, size.y};
}
