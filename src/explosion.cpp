#include <raylib.h>

#include "config.hpp"
#include "explosion.hpp"
#include "util.hpp"

Explosion::Explosion(Vector2 pos) {
  this->pos = pos;
  lifetimeTimer = EXPLOSION_LIFETIME;
  size = {0, 0};
  Resize(GetWindowSize(), GetWindowSize());
}

Explosion::~Explosion() {}

void Explosion::Update() {
  lifetimeTimer -= GetFrameTime();
  if (lifetimeTimer < EXPLOSION_LIFETIME / 2) {
    size -= EXPLOSION_DECAY_SIZE_INTERVAL;
    if (size < 0) {
      size = {0, 0};
    } else {
      pos += EXPLOSION_DECAY_SIZE_INTERVAL;
    }
  } else {
    size += EXPLOSION_BLOOM_SIZE_INTERVAL;
    if (size > maxSize) {
      size = maxSize;
    } else {
      pos -= EXPLOSION_BLOOM_SIZE_INTERVAL;
    }
    // bloom
  }
}

void Explosion::Draw(Texture2D tex) {
  Rectangle explosionRec = {pos.x, pos.y, size.x, size.y};
  DrawTexturePro(tex, {0, 0, (float)tex.width, (float)tex.height}, explosionRec,
                 {0, 0}, 0, WHITE);
  if (DEBUG) {
    DrawRectangleLinesEx(explosionRec, DEBUG_COLL_LINE_THICK,
                         DEBUG_COLL_LINE_COLOR);
  }
}

void Explosion::Resize(Vector2 old, Vector2 nnew) {
  maxSize = CalculateSize(EXPLOSION_SIZE_RATIO);
  pos = MaintainPosAfterResize(old, nnew, pos);
}

float Explosion::GetLifetime() { return lifetimeTimer; }
