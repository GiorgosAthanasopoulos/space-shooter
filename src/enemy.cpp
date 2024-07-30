#include <raylib.h>

#include "enemy.hpp"
#include "util.hpp"

#define ENEMY_COLOR GRAY
#define ENEMY_SIZE_RATIO 10
#define ENEMY_SPEED_RATIO 3

Enemy::Enemy() {
  Resize(GetWindowSize(), GetWindowSize());
  pos.y = -size.y;
  pos.x = GetRandomValue(0, GetWindowWidth() - size.x);
}

Enemy::~Enemy() {}

void Enemy::Update() {
  Vector2 vel;
  vel.y = speed.y;
  pos += vel * GetFrameTime();
}

void Enemy::Draw() { DrawRectangleV(pos, size, ENEMY_COLOR); }

void Enemy::Resize(Vector2 old, Vector2 nnew) {
  size = nnew / ENEMY_SIZE_RATIO;
  speed = nnew / ENEMY_SPEED_RATIO;
  pos = MaintainPosAfterResize(old, nnew, pos);
}

Rectangle Enemy::GetRec() { return {pos.x, pos.y, size.x, size.y}; }
