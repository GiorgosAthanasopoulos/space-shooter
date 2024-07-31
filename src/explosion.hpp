#pragma once

#include <raylib.h>

class Explosion {
public:
  Explosion(Vector2 pos);
  ~Explosion();
  void Update();
  void Draw(Texture2D tex);
  void Resize(Vector2 old, Vector2 nnew);
  float GetLifetime();

private:
  Vector2 pos, maxSize, size;
  float lifetimeTimer;
};
