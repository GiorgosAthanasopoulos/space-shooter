#include <raylib.h>
#include <sstream>

#include "config.hpp"
#include "powerup_type.hpp"
#include "space_shooter.hpp"
#include "util.hpp"

// FIX: bullet hitbox when angled
// TODO: audio
// TODO: explosion

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
  HandleExplosionUpdate();
}

void SpaceShooter::Draw() {
  ClearBackground(WIN_BG);
  DrawTexturePro(am.bg, {0, 0, (float)am.bg.width, (float)am.bg.height},
                 {0, 0, winSize.x, winSize.y}, {0, 0}, 0, WHITE);

  DrawScores();

  if (lost) {
    DrawLoss();
    EndDrawing();
    return;
  }

  DrawPowerups();
  DrawEntities();
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
  powerupSpawnTimer =
      GetRandomValue(POWERUP_SPAWN_TIMER_MIN, POWERUP_SPAWN_TIMER_MAX);

  explosions.clear();

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
    enemySpawnTimer =
        GetRandomValue((float)ENEMY_SPAWN_TIMER_MIN, ENEMY_SPAWN_TIMER_MAX);
    enemies.push_back(
        Enemy(GetRandomValue(0, 1) == 1 ? am.flamingMeteor : am.meteor));
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
        Rectangle collRec = GetCollisionRec(bulletRec, enemyRec);
        explosions.push_back(Explosion({collRec.x, collRec.y}));
      }
    }
  }

  bulletSpawnTimer -= GetFrameTime();
  if (IsKeyPressed(KEY_SHOOT) && bulletSpawnTimer <= 0.0f) {
    bulletSpawnTimer = BULLET_SPAWN_TIMER;
    Rectangle playerRec = player.GetRec();
    float bulletSpawnY = playerRec.y - playerRec.height / 2;
    if (tripleShot) {
      bullets.push_back(
          Bullet({playerRec.x - playerRec.width / 2, bulletSpawnY}, 315));
      bullets.push_back(
          Bullet({playerRec.x + playerRec.width / 2, bulletSpawnY}, 45));
    }
    bullets.push_back(Bullet({playerRec.x, bulletSpawnY}, 0));
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
        shieldTimer = (float)POWERUP_ACTIVE_TIMER;
        break;
      case TRIPLE_SHOT:
        tripleShot = true;
        tripleShotTimer = (float)POWERUP_ACTIVE_TIMER;
        break;
      }
    }
  }

  float frameTime = GetFrameTime();
  powerupSpawnTimer -= frameTime;
  if (powerupSpawnTimer <= 0.0f) {
    powerupSpawnTimer =
        GetRandomValue(POWERUP_SPAWN_TIMER_MIN, POWERUP_SPAWN_TIMER_MAX);
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
               playerRec.y + playerRec.height / 2, playerRec.width,
               SHIELD_COLOR);
  }

  const char *tripleShotText = "Triple Shot Active";
  Vector2 tripleShotTextSize =
      AssertTextFitsInViewport(tripleShotText, FONT_SIZE, winSize / 5);

  const char *shieldText = "Shield Active";
  float shieldTextSizeY = MeasureText(shieldText, tripleShotTextSize.y);

  if (shield) {
    DrawText(shieldText, SCORE_TEXT_PADDING,
             winSize.y - shieldTextSizeY - SCORE_TEXT_PADDING,
             tripleShotTextSize.y, POWERUP_INFO_DRAW_COLOR);
  }
  if (tripleShot) {
    DrawText(tripleShotText, SCORE_TEXT_PADDING,
             winSize.y - SCORE_TEXT_PADDING * 2 - shieldTextSizeY,
             tripleShotTextSize.y, POWERUP_INFO_DRAW_COLOR);
  }
}

void SpaceShooter::DrawEntities() {
  player.Draw(shield ? am.shield : am.spaceship);
  for (size_t i = 0; i < enemies.size(); ++i) {
    enemies[i].Draw();
  }
  for (size_t i = 0; i < bullets.size(); ++i) {
    bullets[i].Draw(am.laser);
  }
  for (size_t i = 0; i < powerups.size(); ++i) {
    PowerupType type = powerups[i].GetType();
    powerups[i].Draw(type == SHIELD ? am.shield : am.powerupIcon);
  }
  for (size_t i = 0; i < explosions.size(); ++i) {
    explosions[i].Draw(am.explosion);
  }
}

void SpaceShooter::HandleExplosionUpdate() {
  for (size_t i = 0; i < explosions.size(); ++i) {
    explosions[i].Update();

    if (explosions[i].GetLifetime() <= 0.0f) {
      explosions.erase(explosions.begin() + i);
    }
  }
}
