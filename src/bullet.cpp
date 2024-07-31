#include <cmath>
#include <raylib.h>

#include "bullet.hpp"
#include "config.hpp"
#include "util.hpp"

Bullet::Bullet(Vector2 pos, float angle) {
  this->pos = pos;
  this->angle = angle;
  Resize(GetWindowSize(), GetWindowSize());
}

Bullet::~Bullet() {}

void Bullet::Update() {
  Vector2 vel;
  vel.x = std::sin(angle) * speed.x;
  vel.y = std::cos(angle) * -speed.y;
  pos += vel * GetFrameTime();
}

void Bullet::Draw() { DrawRectangleV(pos, size, BULLET_COLOR); }

void Bullet::Resize(Vector2 old, Vector2 nnew) {
  speed = CalculateSpeed(BULLET_SPEED_RATIO);
  size = CalculateSize(BULLET_SIZE_RATIO);
  pos = MaintainPosAfterResize(old, nnew, pos);
}

Rectangle Bullet::GetRec() { return {pos.x, pos.y, size.x, size.y}; }
