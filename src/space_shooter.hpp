#pragma once

#include <raylib.h>
#include <vector>

#include "bullet.hpp"
#include "enemy.hpp"
#include "player.hpp"

class SpaceShooter {
public:
  SpaceShooter();
  ~SpaceShooter();
  bool ShouldExit();
  void Update();
  void Draw();

private:
  Player player;
  Vector2 winSize;
  std::vector<Enemy> enemies;
  float enemySpawnTimer;
  bool lost;
  int score;
  std::vector<Bullet> bullets;
  float bulletSpawnTimer;

  void Resize(Vector2 old, Vector2 nnew);
  void Restart();
};
