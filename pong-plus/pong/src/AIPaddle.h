#pragma once
#include "Paddle.h"
#include "Ball.h"

enum class Difficulty { Easy, Normal, Hard };

// AI opponent. The previous version never moved at all (no Update()
// existed). This one tracks the ball with a human-like reaction delay,
// a bit of aim error, and a capped movement speed so it can be beaten,
// with the delay/error/speed all scaled by difficulty.
class AIPaddle : public Paddle {
public:
    Difficulty difficulty = Difficulty::Normal;

    void Update(float dt, const Ball& ball, float screenWidth, float screenHeight);

private:
    float targetY = 0.0f;
    float reactionTimer = 0.0f;
    float currentError = 0.0f;
};
