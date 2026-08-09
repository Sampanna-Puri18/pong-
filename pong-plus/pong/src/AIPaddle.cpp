#include "AIPaddle.h"
#include <cstdlib>

namespace {
struct Tuning {
    float reactionInterval; // how often (s) the AI is allowed to "look" and re-aim
    float errorMargin;      // max random aim error, in pixels
    float speedScale;       // multiplier applied to the paddle's base speed
};

Tuning GetTuning(Difficulty d) {
    switch (d) {
        case Difficulty::Easy:   return { 0.35f, 70.0f, 0.72f };
        case Difficulty::Hard:   return { 0.08f, 12.0f, 1.05f };
        case Difficulty::Normal:
        default:                 return { 0.18f, 36.0f, 0.88f };
    }
}

float RandRange(float lo, float hi) {
    return lo + (hi - lo) * ((float)rand() / (float)RAND_MAX);
}
} // namespace

void AIPaddle::Update(float dt, const Ball& ball, float screenWidth, float screenHeight) {
    Tuning tune = GetTuning(difficulty);

    // Only "notice" the ball's position periodically to simulate reaction
    // time, instead of tracking it with pixel-perfect precision every frame.
    reactionTimer -= dt;
    if (reactionTimer <= 0.0f) {
        reactionTimer = tune.reactionInterval;
        currentError = RandRange(-tune.errorMargin, tune.errorMargin);

        if (ball.speedX > 0.0f) {
            // Ball heading toward the AI: aim for where it actually is (plus error).
            targetY = ball.y + currentError;
        } else {
            // Ball heading away: drift back toward the vertical centre,
            // which feels far more natural than standing frozen.
            targetY = screenHeight * 0.5f + currentError * 0.5f;
        }
    }

    float paddleCenter = y + height * 0.5f;
    float diff = targetY - paddleCenter;
    float maxStep = EffectiveSpeed() * tune.speedScale * dt;

    if (diff > maxStep) diff = maxStep;
    else if (diff < -maxStep) diff = -maxStep;

    // Small dead-zone so the paddle doesn't jitter once it's on target.
    if (diff > 1.0f || diff < -1.0f) {
        y += diff;
    }

    ClampToScreen(screenHeight);
}
