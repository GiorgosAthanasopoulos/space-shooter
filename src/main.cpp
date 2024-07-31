#include "raylib.hpp"
#include "space_shooter.hpp"
#include <raylib.h>

int main() {
  Raylib *raylib = new Raylib;
  SpaceShooter *game = new SpaceShooter;

  while (!game->ShouldExit()) {
    game->Update();
    raylib->Update();

    BeginDrawing();
    game->Draw();
    raylib->Draw();
    EndDrawing();
  }

  delete game;
  delete raylib;
  return 0;
}
