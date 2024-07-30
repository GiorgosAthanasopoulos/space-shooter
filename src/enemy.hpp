#pragma once

#include <raylib.h>

class Enemy {
public:
  Enemy();
  ~Enemy();
  void Update();
  void Draw();
  void Resize(Vector2 old, Vector2 nnew);
  Rectangle GetRec();

private:
  Vector2 pos, size, speed;
};
