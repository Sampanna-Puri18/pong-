#pragma once
#include <raylib.h>
#include <deque>

enum class Side { None, Player, AI };

// The ball keeps a short history of past positions so a fading motion
// trail can be drawn behind it using the BallMotion sprite.
class Ball {
public:
    float x = 0.0f;
    float y = 0.0f;
    float speedX = 0.0f;
    float speedY = 0.0f;
    float radius = 12.0f;
    float baseSpeed = 320.0f;
    float maxSpeed = 780.0f;
    float speedMultiplier = 1.0f;   // power-up modifier (e.g. "slow ball")
    float speedEffectTimer = 0.0f;

    Side lastHitBy = Side::None;    // who last touched the ball (drives power-up ownership)

    Texture2D texture{};
    bool hasTexture = false;
    Texture2D trailTexture{};
    bool hasTrailTexture = false;

    std::deque<Vector2> trail;
    static constexpr int kTrailLength = 6;

    void LoadSprites(const char* ballPath, const char* trailPath);
    void UnloadSprites();

    // Places the ball centred on screen and serves it toward `direction`
    // (-1 serves toward the player/left, +1 serves toward the AI/right).
    void Reset(float screenWidth, float screenHeight, int direction);

    void ApplySpeedEffect(float multiplier, float durationSeconds);
    void TickEffects(float dt);

    void Update(float dt);
    void Draw() const;

    Rectangle GetRect() const {
        return { x - radius, y - radius, radius * 2.0f, radius * 2.0f };
    }
};
