#include "asset_manager.hpp"
#include "config.hpp"
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
  explosionTex = LoadTexture("assets/explosion.png");

  explosion = LoadSound("assets/explosion.mp3");
  lost = LoadSound("assets/lost.mp3");
  powerup = LoadSound("assets/powerup.mp3");
  bgm = LoadMusicStream("assets/bgm.mp3");

  SetSoundVolume(explosion, SOUND_VOLUME);
  SetSoundVolume(powerup, SOUND_VOLUME);
  SetSoundVolume(lost, SOUND_VOLUME);
  SetMusicVolume(bgm, MUSIC_VOLUME);
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
  UnloadTexture(explosionTex);

  UnloadSound(explosion);
  UnloadSound(powerup);
  UnloadSound(lost);
  UnloadMusicStream(bgm);
}
