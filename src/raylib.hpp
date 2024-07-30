#pragma once

#include <raylib.h>

class Raylib {
public:
  Raylib();
  ~Raylib();
  void Update();
  void Draw();
  void Resize(Vector2 old, Vector2 nnew);
};
