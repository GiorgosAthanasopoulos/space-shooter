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
  vel.x = std::sin(DEG2RAD * angle) * speed.x;
  vel.y = std::cos(DEG2RAD * angle) * -speed.y;
  pos += vel * GetFrameTime();
}

void Bullet::Draw(Texture2D tex) {
  // DrawRectangleV(pos, size, BULLET_COLOR);
  Rectangle bulletRec = {pos.x, pos.y, size.x, size.y};
  DrawTexturePro(tex, {0, 0, (float)tex.width, (float)tex.height}, bulletRec,
                 {0, 0}, angle, WHITE);
  if (DEBUG) {
    DrawCircleLinesV({pos.x + size.x / 2, pos.y + size.y / 2}, size.x / 2,
                     DEBUG_COLL_LINE_COLOR);
  }
}

void Bullet::Resize(Vector2 old, Vector2 nnew) {
  speed = CalculateSpeed(BULLET_SPEED_RATIO);
  size = CalculateSize(BULLET_SIZE_RATIO);
  pos = MaintainPosAfterResize(old, nnew, pos);
}

Rectangle Bullet::GetRec() {
  return {pos.x + size.x / 2, pos.y + size.y / 2, size.x, size.y};
}
