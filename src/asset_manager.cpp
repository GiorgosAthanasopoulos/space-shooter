#include "asset_manager.hpp"
#include <raylib.h>

AssetManager::AssetManager() {
  bg = LoadTexture("assets/background.png");
  meteor = LoadTexture("assets/meteor.png");
  laser = LoadTexture("assets/laser.png");
  spaceship = LoadTexture("assets/ship_2.png");
  shield = LoadTexture("assets/ship_3.png");
  spaceshipShield = LoadTexture("assets/ship_2.png");
  powerupIcon = LoadTexture("assets/powerup_icon.png");
  flamingMeteor = LoadTexture("assets/flaming_meteor.png");
  explosion = LoadTexture("assets/explosion.png");
}

AssetManager::~AssetManager() {
  UnloadTexture(bg);
  UnloadTexture(meteor);
  UnloadTexture(laser);
  UnloadTexture(spaceship);
  UnloadTexture(shield);
  UnloadTexture(spaceshipShield);
  UnloadTexture(powerupIcon);
  UnloadTexture(flamingMeteor);
  UnloadTexture(explosion);
}
