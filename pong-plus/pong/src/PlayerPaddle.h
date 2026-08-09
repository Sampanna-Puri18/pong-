#pragma once
#include "Paddle.h"

// Human-controlled paddle. Supports both W/S and Up/Down arrow keys so it
// works regardless of keyboard layout, plus optional mouse Y control.
class PlayerPaddle : public Paddle {
public:
    void Update(float dt, float screenHeight);
};
