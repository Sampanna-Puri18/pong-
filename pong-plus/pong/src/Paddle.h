#pragma once
#include <raylib.h>

// Base class shared by the human-controlled and AI-controlled paddles.
// Holds position/size, the sprite, and the temporary power-up modifiers
// (size + speed multipliers) that decay back to normal over time.
class Paddle {
public:
    float x = 0.0f;
    float y = 0.0f;
    float width = 16.0f;
    float height = 110.0f;
    float speed = 380.0f; // pixels / second

    // --- Power-up state -----------------------------------------------
    float sizeMultiplier = 1.0f;
    float speedMultiplier = 1.0f;
    float sizeEffectTimer = 0.0f;   // seconds remaining
    float speedEffectTimer = 0.0f;  // seconds remaining

    Texture2D texture{};
    bool hasTexture = false;

    void LoadSprite(const char* path);
    void UnloadSprite();

    // Applies a temporary paddle-size change (e.g. grow / shrink power-ups).
    void ApplySizeEffect(float multiplier, float durationSeconds);
    // Applies a temporary movement-speed change.
    void ApplySpeedEffect(float multiplier, float durationSeconds);

    // Advances power-up timers; call once per frame before Update().
    void TickEffects(float dt);

    // Effective (post power-up) paddle height, used for collisions & drawing.
    float EffectiveHeight() const { return height * sizeMultiplier; }
    float EffectiveSpeed() const { return speed * speedMultiplier; }

    Rectangle GetRect() const;
    void ClampToScreen(float screenHeight);
    void Draw(Color tint = WHITE) const;
};
