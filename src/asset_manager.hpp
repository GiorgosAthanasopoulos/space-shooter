#pragma once

#include <raylib.h>
class AssetManager {
public:
  Texture2D bg, meteor, laser, spaceship, shield, spaceshipShield, powerupIcon;
  Texture2D flamingMeteor, explosion;

  AssetManager();
  ~AssetManager();
};
