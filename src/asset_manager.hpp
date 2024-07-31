#pragma once

#include <raylib.h>
class AssetManager {
public:
  Texture2D bg, meteor, laser, spaceship, shield, spaceshipShield, powerupIcon;
  Texture2D flamingMeteor, explosionTex;

  Sound explosion, lost, powerup;
  Music bgm;

  AssetManager();
  ~AssetManager();
};
