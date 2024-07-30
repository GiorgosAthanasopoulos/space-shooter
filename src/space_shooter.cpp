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

SpaceShooter::SpaceShooter() {
  winSize = GetWindowSize();
  enemySpawnTimer = 0;
  lost = false;
  score = 0;
  bulletSpawnTimer = 0;
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
      Rectangle enemyRec = enemies[i].GetRec();
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

  std::ostringstream scoreTextStream;
  scoreTextStream << "Score: " << score;
  std::string scoreTextString = scoreTextStream.str();
  const char *scoreText = scoreTextString.data();
  Vector2 scoreTextSize =
      AssertTextFitsInViewport(scoreText, 100, GetWindowSize() / 6);
  DrawText(scoreText, winSize.x - scoreTextSize.x - SCORE_TEXT_PADDING,
           SCORE_TEXT_PADDING, scoreTextSize.y, SCORE_TEXT_COLOR);

  if (lost) {
    const char *lostText = "YOU LOST";
    Vector2 lostTextSize =
        AssertTextFitsInViewport(lostText, 1000, GetWindowSize() / 2);
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
  score = 0;
  bullets.clear();
}
