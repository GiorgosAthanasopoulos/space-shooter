#pragma once

#include <raylib.h>

class Bullet {
public:
  Bullet(Vector2 pos, float angle);
  ~Bullet();
  void Update();
  void Draw(Texture2D tex);
  void Resize(Vector2 old, Vector2 nnew);
  Rectangle GetRec();

private:
  Vector2 pos, size, speed;
  float angle;
};
