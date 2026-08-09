#include "PlayerPaddle.h"
#include <raylib.h>

void PlayerPaddle::Draw() {
    DrawRectangle(x, y, width, height, RED);
}
void PlayerPaddle::Update() {
    if (IsKeyDown(KEY_W) && y > 0) {
        y -= speed;
    }
    if (IsKeyDown(KEY_S) && y + height < 600) {
        y += speed;
    }
}
