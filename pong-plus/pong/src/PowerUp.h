#pragma once
#include <raylib.h>
#include "Ball.h"

enum class PowerUpType {
    GrowSelf,     // beneficiary's paddle grows
    ShrinkRival,  // opponent's paddle shrinks
    SwiftSelf,    // beneficiary's paddle moves faster
    SlowBall      // ball slows down for everyone (helps the defender react)
};

// A collectible that floats in the play field. Whoever's ball hit (the
// last paddle to touch the ball) benefits when the ball passes through it,
// which rewards active, aggressive play rather than luck.
struct PowerUp {
    PowerUpType type;
    Vector2 position{};
    float radius = 16.0f;
    float lifeTime = 9.0f;     // seconds before it despawns if uncollected
    float pulseTimer = 0.0f;   // purely visual

    Rectangle GetRect() const {
        return { position.x - radius, position.y - radius, radius * 2.0f, radius * 2.0f };
    }
};

const char* PowerUpLabel(PowerUpType type);
Color PowerUpColor(PowerUpType type);
void DrawPowerUpIcon(const PowerUp& p);
