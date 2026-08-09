#include "PlayerPaddle.h"
#include <raylib.h>

void PlayerPaddle::Draw() {
    DrawRectangle(x, y, width, height, RED);
}
