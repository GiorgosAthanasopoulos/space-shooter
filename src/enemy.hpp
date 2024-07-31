#pragma once

#include <raylib.h>

class Enemy {
public:
  Enemy(Texture2D tex);
  ~Enemy();
  void Update();
  void Draw();
  void Resize(Vector2 old, Vector2 nnew);
  Rectangle GetRec();

private:
  Vector2 pos, size, speed;
  Texture2D tex;
};
