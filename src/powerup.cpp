#include <raylib.h>

#include "config.hpp"
#include "powerup.hpp"
#include "powerup_type.hpp"
#include "util.hpp"

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

void Powerup::Draw(Texture2D tex) {
  // DrawRectangleV(pos, size, POWERUP_COLOR);
  Rectangle powerupRec = {pos.x, pos.y, size.x, size.y};
  DrawTexturePro(tex, {0, 0, (float)tex.width, (float)tex.height}, powerupRec,
                 {0, 0}, 0, WHITE);
  if (DEBUG) {
    DrawRectangleLinesEx(powerupRec, DEBUG_COLL_LINE_THICK,
                         DEBUG_COLL_LINE_COLOR);
  }
}

void Powerup::Resize(Vector2 old, Vector2 nnew) {
  speed = CalculateSpeed(POWERUP_SPEED_RATIO);
  size = CalculateSize(POWERUP_SIZE_RATIO);
  pos = MaintainPosAfterResize(old, nnew, pos);
}

Rectangle Powerup::GetRec() { return {pos.x, pos.y, size.x, size.y}; }

PowerupType Powerup::GetType() { return type; }
