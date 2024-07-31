#pragma once

#include "powerup_type.hpp"
#include <raylib.h>

class Powerup {
public:
  Powerup(PowerupType type);
  ~Powerup();
  void Update();
  void Draw(Texture2D tex);
  void Resize(Vector2 old, Vector2 nnew);
  Rectangle GetRec();
  PowerupType GetType();

private:
  Vector2 pos, size, speed;
  PowerupType type;
};
