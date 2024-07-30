#include <raylib.h>

#include "player.hpp"
#include "util.hpp"

#define PLAYER_COLOR RED
#define PLAYER_SIZE_RATIO 10
#define KEY_MOVE_RIGHT KEY_D
#define KEY_MOVE_LEFT KEY_A
#define PLAYER_SPEED_RATIO 2

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
  size = nnew / PLAYER_SIZE_RATIO;
  speed = nnew / PLAYER_SPEED_RATIO;
  pos = MaintainPosAfterResize(old, nnew, pos);
}

Rectangle Player::GetRec() { return {pos.x, pos.y, size.x, size.y}; }

void Player::Reset() {
  pos = {GetWindowWidth() / 2 - size.x / 2, GetWindowHeight() - 2 * size.y};
}
