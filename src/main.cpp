#include "raylib.hpp"
#include "space_shooter.hpp"

int main() {
  Raylib *raylib = new Raylib;
  SpaceShooter *game = new SpaceShooter;

  while (!game->ShouldExit()) {
    game->Update();
    raylib->Update();

    game->Draw();
    raylib->Draw();
  }

  delete game;
  delete raylib;
  return 0;
}
