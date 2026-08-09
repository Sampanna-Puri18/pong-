#include "Ball.h"
#include <raymath.h>
#include <cstdlib>
#include <cmath>

void Ball::LoadSprites(const char* ballPath, const char* trailPath) {
    texture = LoadTexture(ballPath);
    hasTexture = (texture.id != 0);
    if (hasTexture) SetTextureFilter(texture, TEXTURE_FILTER_BILINEAR);

    trailTexture = LoadTexture(trailPath);
    hasTrailTexture = (trailTexture.id != 0);
    if (hasTrailTexture) SetTextureFilter(trailTexture, TEXTURE_FILTER_BILINEAR);
}

void Ball::UnloadSprites() {
    if (hasTexture) { UnloadTexture(texture); hasTexture = false; }
    if (hasTrailTexture) { UnloadTexture(trailTexture); hasTrailTexture = false; }
}

void Ball::Reset(float screenWidth, float screenHeight, int direction) {
    x = screenWidth * 0.5f;
    y = screenHeight * 0.5f;
    speedMultiplier = 1.0f;
    speedEffectTimer = 0.0f;
    lastHitBy = Side::None;
    trail.clear();

    // Random-ish serve angle so every round doesn't start identically.
    float angle = ((float)(rand() % 60) - 30.0f) * DEG2RAD; // +/-30 degrees
    float dir = (direction >= 0) ? 1.0f : -1.0f;
    speedX = cosf(angle) * baseSpeed * dir;
    speedY = sinf(angle) * baseSpeed;
    if (fabsf(speedY) < 60.0f) speedY = (speedY < 0 ? -60.0f : 60.0f);
}

void Ball::ApplySpeedEffect(float multiplier, float durationSeconds) {
    speedMultiplier = multiplier;
    speedEffectTimer = durationSeconds;
}

void Ball::TickEffects(float dt) {
    if (speedEffectTimer > 0.0f) {
        speedEffectTimer -= dt;
        if (speedEffectTimer <= 0.0f) {
            speedEffectTimer = 0.0f;
            speedMultiplier = 1.0f;
        }
    }
}

void Ball::Update(float dt) {
    trail.push_back({ x, y });
    while ((int)trail.size() > kTrailLength) trail.pop_front();

    x += speedX * speedMultiplier * dt;
    y += speedY * speedMultiplier * dt;
}

void Ball::Draw() const {
    // Fading trail, oldest first, drawn behind the ball.
    if (hasTrailTexture) {
        float angle = atan2f(speedY, speedX) * RAD2DEG;
        int n = (int)trail.size();
        for (int i = 0; i < n; ++i) {
            float t = (float)(i + 1) / (float)(n + 1); // 0..1, closer to ball = higher
            unsigned char alpha = (unsigned char)(t * 140.0f);
            float scale = 0.5f + 0.5f * t;
            Vector2 p = trail[i];
            Rectangle src = { 0, 0, (float)trailTexture.width, (float)trailTexture.height };
            float size = radius * 2.6f * scale;
            Rectangle dst = { p.x, p.y, size, size };
            Vector2 origin = { size * 0.5f, size * 0.5f };
            DrawTexturePro(trailTexture, src, dst, origin, angle, Fade(WHITE, alpha / 255.0f));
        }
    }

    if (hasTexture) {
        Rectangle src = { 0, 0, (float)texture.width, (float)texture.height };
        float size = radius * 2.0f;
        Rectangle dst = { x, y, size, size };
        Vector2 origin = { size * 0.5f, size * 0.5f };
        DrawTexturePro(texture, src, dst, origin, 0.0f, WHITE);
    } else {
        DrawCircle((int)x, (int)y, radius, WHITE);
    }
}
