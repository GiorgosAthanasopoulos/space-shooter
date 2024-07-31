#include <raylib.h>

#include "config.hpp"
#include "player.hpp"
#include "util.hpp"

Player::Player() {
  Resize(GetWindowSize(), GetWindowSize());
  Reset();
}

Player::~Player() {}

void Player::Update() {
  Vector2 vel = {0, 0};

  if (IsKeyDown(KEY_MOVE_LEFT)) {
    vel.x = -speed.x;
  } else if (IsKeyDown(KEY_MOVE_RIGHT)) {
    vel.x = speed.x;
  }

  pos += vel * GetFrameTime();

  WorldBoundaryCheckRec(&pos, size);
}

void Player::Draw() { DrawRectangleV(pos, size, PLAYER_COLOR); }

void Player::Resize(Vector2 old, Vector2 nnew) {
  size = CalculateSize(PLAYER_SIZE_RATIO);
  speed = CalculateSpeed(PLAYER_SPEED_RATIO);
  pos = MaintainPosAfterResize(old, nnew, pos);
}

Rectangle Player::GetRec() { return {pos.x, pos.y, size.x, size.y}; }

void Player::Reset() {
  pos = {GetWindowWidth() / 2 - size.x / 2, GetWindowHeight() - 2 * size.y};
}
