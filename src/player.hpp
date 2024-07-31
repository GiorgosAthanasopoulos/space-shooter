#pragma once

#include <raylib.h>

class Player {
public:
  Player();
  ~Player();
  void Update();
  void Draw(Texture2D tex);
  void Resize(Vector2 old, Vector2 nnew);
  Rectangle GetRec();
  void Reset();

private:
  Vector2 pos, size, speed;
};
