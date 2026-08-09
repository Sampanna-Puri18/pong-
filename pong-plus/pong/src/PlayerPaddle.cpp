#include "PlayerPaddle.h"

void PlayerPaddle::Update(float dt, float screenHeight) {
    float move = 0.0f;
    if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP))   move -= 1.0f;
    if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) move += 1.0f;

    y += move * EffectiveSpeed() * dt;

    // Let the mouse take over smoothly if the player moves it vertically.
    float mouseDelta = GetMouseDelta().y;
    if (mouseDelta != 0.0f) {
        y += mouseDelta;
    }

    ClampToScreen(screenHeight);
}
