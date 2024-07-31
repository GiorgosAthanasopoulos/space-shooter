#include <raylib.h>
#include <sstream>

#include "space_shooter.hpp"
#include "util.hpp"

#define WIN_BG BLACK
#define ENEMY_SPAWN_TIMER 2.0f
#define KEY_RESTART KEY_SPACE
#define SCORE_TEXT_PADDING 5
#define SCORE_TEXT_COLOR WHITE
#define SCORE_INTERVAL_OOB 10
#define SCORE_INTERVAL_KILL 20
#define BULLET_SPAWN_TIMER 0.5f
#define KEY_SHOOT KEY_SPACE
#define FONT_SIZE 100
#define LARGE_FONT_SIZE 1000

SpaceShooter::SpaceShooter() {
  winSize = GetWindowSize();
  enemySpawnTimer = 0;
  lost = false;
  score = 0;
  bulletSpawnTimer = 0;
  bestScore = 0;
}

SpaceShooter::~SpaceShooter() {}

bool SpaceShooter::ShouldExit() { return WindowShouldClose(); }

void SpaceShooter::Update() {
  if (IsWindowResized()) {
    Vector2 nnew = GetWindowSize();
    Resize(winSize, nnew);
    winSize = nnew;
  }

  if (lost) {
    if (IsKeyPressed(KEY_RESTART)) {
      Restart();
    }
    return;
  }

  player.Update();
  for (size_t i = 0; i < enemies.size(); ++i) {
    enemies[i].Update();

    Rectangle enemyRec = enemies[i].GetRec();
    if (enemyRec.y > GetWindowHeight()) {
      enemies.erase(enemies.begin() + i);
      score += SCORE_INTERVAL_OOB;
    }

    if (CheckCollisionRecs(enemyRec, player.GetRec())) {
      lost = true;
    }
  }
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

  float frameTime = GetFrameTime();
  enemySpawnTimer -= frameTime;
  if (enemySpawnTimer <= 0.0f) {
    enemySpawnTimer = ENEMY_SPAWN_TIMER;
    enemies.push_back(Enemy());
  }
  bulletSpawnTimer -= frameTime;
  if (IsKeyPressed(KEY_SHOOT) && bulletSpawnTimer <= 0.0f) {
    bulletSpawnTimer = BULLET_SPAWN_TIMER;
    Rectangle playerRec = player.GetRec();
    bullets.push_back(
        Bullet({playerRec.x, playerRec.y - playerRec.height / 2}, 0));
  }
}

void SpaceShooter::Draw() {
  BeginDrawing();
  ClearBackground(WIN_BG);

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

  if (lost) {
    const char *lostText = "YOU LOST";
    Vector2 lostTextSize = AssertTextFitsInViewport(lostText, LARGE_FONT_SIZE,
                                                    GetWindowSize() / 2);
    DrawText(lostText, GetWindowWidth() / 2 - lostTextSize.x / 2,
             GetWindowHeight() / 2 - lostTextSize.y / 2, lostTextSize.y, RED);

    EndDrawing();
    return;
  }

  player.Draw();
  for (size_t i = 0; i < enemies.size(); ++i) {
    enemies[i].Draw();
  }
  for (size_t i = 0; i < bullets.size(); ++i) {
    bullets[i].Draw();
  }

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
}

void SpaceShooter::Restart() {
  enemies.clear();
  enemySpawnTimer = ENEMY_SPAWN_TIMER;
  lost = false;
  player.Reset();
  if (score > bestScore) {
    bestScore = score;
  }
  score = 0;
  bullets.clear();
}
