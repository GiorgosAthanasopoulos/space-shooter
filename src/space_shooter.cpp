#include <raylib.h>
#include <sstream>

#include "powerup_type.hpp"
#include "space_shooter.hpp"
#include "util.hpp"

#define WIN_BG BLACK
#define KEY_RESTART KEY_SPACE
#define SCORE_TEXT_PADDING 5
#define SCORE_TEXT_COLOR WHITE
#define SCORE_INTERVAL_OOB 10
#define SCORE_INTERVAL_KILL 20
#define KEY_SHOOT KEY_SPACE
#define FONT_SIZE 100
#define LARGE_FONT_SIZE 1000

#define POWERUP_SPAWN_TIMER 10.0f
#define POWERUP_ACTIVE_TIMER POWERUP_SPAWN_TIMER * 3 / 4
#define ENEMY_SPAWN_TIMER 2.0f
#define BULLET_SPAWN_TIMER 0.5f
#define SHIELD_COLOR RAYWHITE
#define POWERUP_INFO_DRAW_COLOR RED

SpaceShooter::SpaceShooter() {
  winSize = GetWindowSize();
  bestScore = 0;
  score = 0;
  Restart();
}

SpaceShooter::~SpaceShooter() {}

bool SpaceShooter::ShouldExit() { return WindowShouldClose(); }

void SpaceShooter::Update() {
  HandleResize();
  if (lost) {
    HandleLossUpdate();
    return;
  }
  player.Update();
  HandleEnemyUpdate();
  HandleBulletUpdate();
  HandlePowerupUpdate();
}

void SpaceShooter::Draw() {
  BeginDrawing();
  ClearBackground(WIN_BG);

  DrawScores();

  if (lost) {
    DrawLoss();
    EndDrawing();
    return;
  }

  DrawPowerups();
  DrawEntities();

  EndDrawing();
}

void SpaceShooter::Resize(Vector2 old, Vector2 nnew) {
  player.Resize(old, nnew);
  for (size_t i = 0; i < enemies.size(); ++i) {
    enemies[i].Resize(old, nnew);
  }
  for (size_t i = 0; i < bullets.size(); ++i) {
    bullets[i].Resize(old, nnew);
  }
  for (size_t i = 0; i < powerups.size(); ++i) {
    powerups[i].Resize(old, nnew);
  }
}

void SpaceShooter::Restart() {
  lost = false;
  if (score > bestScore) {
    bestScore = score;
  }
  score = 0;

  player.Reset();

  enemies.clear();
  enemySpawnTimer = 0;

  bullets.clear();
  bulletSpawnTimer = 0;

  powerups.clear();
  powerupSpawnTimer = POWERUP_SPAWN_TIMER;

  shield = false;
  shieldTimer = 0;
  tripleShot = false;
  tripleShotTimer = 0;
}

void SpaceShooter::HandleResize() {
  if (IsWindowResized()) {
    Vector2 nnew = GetWindowSize();
    Resize(winSize, nnew);
    winSize = nnew;
  }
}

void SpaceShooter::HandleLossUpdate() {
  if (IsKeyPressed(KEY_RESTART)) {
    Restart();
  }
}

void SpaceShooter::HandleEnemyUpdate() {
  for (size_t i = 0; i < enemies.size(); ++i) {
    enemies[i].Update();

    Rectangle enemyRec = enemies[i].GetRec();
    if (enemyRec.y > GetWindowHeight()) {
      enemies.erase(enemies.begin() + i);
      score += SCORE_INTERVAL_OOB;
    }

    if (CheckCollisionRecs(enemyRec, player.GetRec())) {
      if (shield) {
        shield = false;
        enemies.erase(enemies.begin() + i);
      } else {
        lost = true;
      }
    }
  }

  enemySpawnTimer -= GetFrameTime();
  if (enemySpawnTimer <= 0.0f) {
    enemySpawnTimer = ENEMY_SPAWN_TIMER;
    enemies.push_back(Enemy());
  }
}

void SpaceShooter::HandleBulletUpdate() {
  for (size_t i = 0; i < bullets.size(); ++i) {
    bullets[i].Update();

    Rectangle bulletRec = bullets[i].GetRec();
    if (bulletRec.y < 0) {
      bullets.erase(bullets.begin() + i);
    }

    for (size_t j = 0; j < enemies.size(); ++j) {
      Rectangle enemyRec = enemies[j].GetRec();
      if (CheckCollisionRecs(bulletRec, enemyRec)) {
        bullets.erase(bullets.begin() + i);
        enemies.erase(enemies.begin() + j);
        score += SCORE_INTERVAL_KILL;
      }
    }
  }

  bulletSpawnTimer -= GetFrameTime();
  if (IsKeyPressed(KEY_SHOOT) && bulletSpawnTimer <= 0.0f) {
    bulletSpawnTimer = BULLET_SPAWN_TIMER;
    Rectangle playerRec = player.GetRec();
    float bulletSpawnY = playerRec.y - playerRec.height / 2;
    if (tripleShot) {
      bullets.push_back(Bullet(
          {playerRec.x - playerRec.width / 2, bulletSpawnY}, DEG2RAD * 315));
      bullets.push_back(Bullet(
          {playerRec.x + playerRec.width / 2, bulletSpawnY}, DEG2RAD * 45));
    }
    bullets.push_back(Bullet({playerRec.x, bulletSpawnY}, DEG2RAD * 0));
  }
}

void SpaceShooter::HandlePowerupUpdate() {
  for (size_t i = 0; i < powerups.size(); ++i) {
    powerups[i].Update();

    Rectangle powerupRec = powerups[i].GetRec();
    if (powerupRec.y > winSize.y) {
      powerups.erase(powerups.begin() + i);
    }

    if (CheckCollisionRecs(player.GetRec(), powerupRec)) {
      powerups.erase(powerups.begin() + i);
      switch (powerups[i].GetType()) {
      case SHIELD:
        shield = true;
        shieldTimer = POWERUP_ACTIVE_TIMER;
        break;
      case TRIPLE_SHOT:
        tripleShot = true;
        tripleShotTimer = POWERUP_ACTIVE_TIMER;
        break;
      }
    }
  }

  float frameTime = GetFrameTime();
  powerupSpawnTimer -= frameTime;
  if (powerupSpawnTimer <= 0.0f) {
    powerupSpawnTimer = POWERUP_SPAWN_TIMER;
    PowerupType type;
    int selection = GetRandomValue(0, 1);
    switch (selection) {
    case 0:
      type = SHIELD;
      break;
    case 1:
      type = TRIPLE_SHOT;
      break;
    }
    powerups.push_back(Powerup(type));
  }
  shieldTimer -= frameTime;
  if (shieldTimer <= 0.0f) {
    shield = false;
  }
  tripleShotTimer -= frameTime;
  if (tripleShotTimer <= 0.0f) {
    tripleShot = false;
  }
}

void SpaceShooter::DrawScores() {
  std::ostringstream scrStrm;
  scrStrm << "Score: " << score;
  std::string scrStr = scrStrm.str();
  const char *scrTxt = scrStr.data();

  std::ostringstream bstScrStrm;
  bstScrStrm << "Best Score: " << bestScore;
  std::string bstScrStr = bstScrStrm.str();
  const char *bstScrTxt = bstScrStr.data();
  Vector2 bstScrTxtSz =
      AssertTextFitsInViewport(bstScrTxt, FONT_SIZE, winSize / 6);

  float scrTxtSzX = MeasureText(scrTxt, bstScrTxtSz.y);

  DrawText(scrTxt, winSize.x - scrTxtSzX - SCORE_TEXT_PADDING,
           SCORE_TEXT_PADDING, bstScrTxtSz.y, SCORE_TEXT_COLOR);
  DrawText(bstScrTxt, winSize.x - bstScrTxtSz.x - SCORE_TEXT_PADDING,
           SCORE_TEXT_PADDING * 2 + bstScrTxtSz.y, bstScrTxtSz.y,
           SCORE_TEXT_COLOR);
}

void SpaceShooter::DrawLoss() {
  const char *lostText = "YOU LOST";
  Vector2 lostTextSize =
      AssertTextFitsInViewport(lostText, LARGE_FONT_SIZE, winSize / 2);
  DrawText(lostText, GetWindowWidth() / 2 - lostTextSize.x / 2,
           GetWindowHeight() / 2 - lostTextSize.y / 2, lostTextSize.y, RED);
}

void SpaceShooter::DrawPowerups() {
  if (shield) {
    Rectangle playerRec = player.GetRec();
    DrawCircle(playerRec.x + playerRec.width / 2,
               playerRec.y + playerRec.height / 2, 2 * playerRec.width,
               SHIELD_COLOR);
  }
  // FIX: from this point on smth blocks the thread...
  const char *tripleShotText = "TRIPLE SHOT ACTIVE";
  Vector2 tripleShotTextSize =
      AssertTextFitsInViewport(tripleShotText, FONT_SIZE, winSize / 6);

  const char *shieldText = "SHIELD ACTIVE";
  float shieldTextSizeY = MeasureText(shieldText, tripleShotTextSize.y);

  if (shield) {
    DrawText(shieldText, SCORE_TEXT_PADDING, winSize.y - SCORE_TEXT_PADDING,
             tripleShotTextSize.y, POWERUP_INFO_DRAW_COLOR);
  }
  if (tripleShot) {
    DrawText(tripleShotText, SCORE_TEXT_PADDING,
             winSize.y - SCORE_TEXT_PADDING * 2 - shieldTextSizeY,
             tripleShotTextSize.y, POWERUP_INFO_DRAW_COLOR);
  }
}

void SpaceShooter::DrawEntities() {
  player.Draw();
  for (size_t i = 0; i < enemies.size(); ++i) {
    enemies[i].Draw();
  }
  for (size_t i = 0; i < bullets.size(); ++i) {
    bullets[i].Draw();
  }
  for (size_t i = 0; i < powerups.size(); ++i) {
    powerups[i].Draw();
  }
}
