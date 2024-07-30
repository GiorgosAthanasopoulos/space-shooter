#include "raylib.hpp"
#include <raylib.h>

#define WIN_FLAGS FLAG_WINDOW_RESIZABLE
#define WIN_W 1280
#define WIN_H 720
#define WIN_TITLE "Space Shooter"
#define WIN_FPS 60
#define EXIT_KEY KEY_NULL
#define WIN_MIN_W 640
#define WIN_MIN_H 480
#define WIN_MAX_W 3840
#define WIN_MAX_H 2160
#define DEBUG true

Raylib::Raylib() {
  SetConfigFlags(WIN_FLAGS);
  InitWindow(WIN_W, WIN_H, WIN_TITLE);
  SetTargetFPS(WIN_FPS);
  SetExitKey(EXIT_KEY);
  SetWindowMinSize(WIN_MIN_W, WIN_MIN_H);
  SetWindowMaxSize(WIN_MAX_W, WIN_MAX_H);
  InitAudioDevice();
}

Raylib::~Raylib() {
  CloseAudioDevice();
  CloseWindow();
}

void Raylib::Update() {}

void Raylib::Draw() {
  if (DEBUG) {
    DrawFPS(0, 0);
  }
}

void Raylib::Resize(Vector2, Vector2) {}
