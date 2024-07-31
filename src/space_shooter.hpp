#pragma once

#include <raylib.h>
#include <vector>

#include "bullet.hpp"
#include "enemy.hpp"
#include "player.hpp"
#include "powerup.hpp"

class SpaceShooter {
public:
  SpaceShooter();
  ~SpaceShooter();
  bool ShouldExit();
  void Update();
  void Draw();

private:
  Vector2 winSize;

  bool lost;
  int score;
  int bestScore;

  Player player;
  bool shield, tripleShot;
  float shieldTimer, tripleShotTimer;

  std::vector<Enemy> enemies;
  float enemySpawnTimer;

  std::vector<Bullet> bullets;
  float bulletSpawnTimer;

  std::vector<Powerup> powerups;
  float powerupSpawnTimer;

  void Resize(Vector2 old, Vector2 nnew);
  void Restart();

  void HandleResize();
  void HandleLossUpdate();
  void HandleEnemyUpdate();
  void HandleBulletUpdate();
  void HandlePowerupUpdate();

  void DrawScores();
  void DrawLoss();
  void DrawPowerups();
  void DrawEntities();
};
