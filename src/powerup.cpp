#include <raylib.h>

#include "powerup.hpp"
#include "powerup_type.hpp"
#include "util.hpp"

#define POWERUP_SPEED_RATIO 3
#define POWERUP_SIZE_RATIO 10
#define POWERUP_COLOR PURPLE

Powerup::Powerup(PowerupType type) {
  Resize(GetWindowSize(), GetWindowSize());
  pos = {(float)GetRandomValue(0, GetWindowWidth() - size.x), -size.y};
  this->type = type;
}

Powerup::~Powerup() {}

void Powerup::Update() {
  Vector2 vel;
  vel.y = speed.y;
  pos += vel * GetFrameTime();
}

void Powerup::Draw() { DrawRectangleV(pos, size, POWERUP_COLOR); }

void Powerup::Resize(Vector2 old, Vector2 nnew) {
  speed = CalculateSpeed(POWERUP_SPEED_RATIO);
  size = CalculateSize(POWERUP_SIZE_RATIO);
  pos = MaintainPosAfterResize(old, nnew, pos);
}

Rectangle Powerup::GetRec() { return {pos.x, pos.y, size.x, size.y}; }

PowerupType Powerup::GetType() { return type; }
