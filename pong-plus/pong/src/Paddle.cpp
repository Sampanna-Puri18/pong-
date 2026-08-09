#include "Paddle.h"

void Paddle::LoadSprite(const char* path) {
    texture = LoadTexture(path);
    hasTexture = (texture.id != 0);
    if (hasTexture) SetTextureFilter(texture, TEXTURE_FILTER_BILINEAR);
}

void Paddle::UnloadSprite() {
    if (hasTexture) {
        UnloadTexture(texture);
        hasTexture = false;
    }
}

void Paddle::ApplySizeEffect(float multiplier, float durationSeconds) {
    sizeMultiplier = multiplier;
    sizeEffectTimer = durationSeconds;
}

void Paddle::ApplySpeedEffect(float multiplier, float durationSeconds) {
    speedMultiplier = multiplier;
    speedEffectTimer = durationSeconds;
}

void Paddle::TickEffects(float dt) {
    if (sizeEffectTimer > 0.0f) {
        sizeEffectTimer -= dt;
        if (sizeEffectTimer <= 0.0f) {
            sizeEffectTimer = 0.0f;
            sizeMultiplier = 1.0f;
        }
    }
    if (speedEffectTimer > 0.0f) {
        speedEffectTimer -= dt;
        if (speedEffectTimer <= 0.0f) {
            speedEffectTimer = 0.0f;
            speedMultiplier = 1.0f;
        }
    }
}

Rectangle Paddle::GetRect() const {
    float h = EffectiveHeight();
    // Grow/shrink from the paddle's vertical centre so it feels natural.
    float centerY = y + height * 0.5f;
    return { x, centerY - h * 0.5f, width, h };
}

void Paddle::ClampToScreen(float screenHeight) {
    float h = EffectiveHeight();
    float centerY = y + height * 0.5f;
    if (centerY - h * 0.5f < 0.0f) y = h * 0.5f - height * 0.5f;
    if (centerY + h * 0.5f > screenHeight) y = screenHeight - h * 0.5f - height * 0.5f;
}

void Paddle::Draw(Color tint) const {
    Rectangle dst = GetRect();
    if (hasTexture) {
        Rectangle src = { 0, 0, (float)texture.width, (float)texture.height };
        DrawTexturePro(texture, src, dst, { 0, 0 }, 0.0f, tint);
    } else {
        DrawRectangleRounded(dst, 0.4f, 6, tint);
    }
}
