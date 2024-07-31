#pragma once

#include <raylib.h>
#include <vector>

#include "asset_manager.hpp"
#include "bullet.hpp"
#include "enemy.hpp"
#include "explosion.hpp"
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

  AssetManager am;

  std::vector<Explosion> explosions;

  void Resize(Vector2 old, Vector2 nnew);
  void Restart();

  void HandleResize();
  void HandleLossUpdate();
  void HandleEnemyUpdate();
  void HandleBulletUpdate();
  void HandlePowerupUpdate();
  void HandleExplosionUpdate();

  void DrawScores();
  void DrawLoss();
  void DrawPowerups();
  void DrawEntities();
};
