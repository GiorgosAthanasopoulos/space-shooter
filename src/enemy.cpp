#include <raylib.h>

#include "config.hpp"
#include "enemy.hpp"
#include "util.hpp"

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
  speed = CalculateSpeed(ENEMY_SPEED_RATIO);
  size = CalculateSize(ENEMY_SIZE_RATIO);
  pos = MaintainPosAfterResize(old, nnew, pos);
}

Rectangle Enemy::GetRec() { return {pos.x, pos.y, size.x, size.y}; }
