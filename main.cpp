#include "Ball.h"
#include "PlayerPaddle.h"
#include "AIPaddle.h"
#include <raylib.h>

int main() {
    const int SCREEN_WIDTH = 800;
    const int SCREEN_HEIGHT = 600;

    
    Ball ball;
    ball.x = 100;
    ball.y = 100;
    ball.speed_x = 5;
    ball.speed_y = 5;
    ball.radius = 15;

    
    PlayerPaddle player;
    player.width = 20;
    player.height = 80;
    player.x = 10;
    player.y = SCREEN_HEIGHT / 2 - player.height / 2;
    player.speed = 6;

    
    AIPaddle ai;
    ai.width = 20;
    ai.height = 80;
    ai.x = SCREEN_WIDTH - ai.width - 10; 
    ai.y = SCREEN_HEIGHT / 2 - ai.height / 2; 
    ai.speed = 6;

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Pong+");
    SetTargetFPS(60);

    while (WindowShouldClose() == false) {
        ball.x += ball.speed_x;
        ball.y += ball.speed_y;

        if (ball.x + ball.radius >= SCREEN_WIDTH || ball.x - ball.radius <= 0) {
            ball.speed_x *= -1;
        }

        if (ball.y + ball.radius >= SCREEN_HEIGHT || ball.y - ball.radius <= 0) {
            ball.speed_y *= -1;
        }

        BeginDrawing();
            ClearBackground(BLACK);
            DrawCircle(ball.x, ball.y, ball.radius, WHITE);
            
            player.Draw();
            ai.Draw(); 
            
            DrawLine(SCREEN_WIDTH / 2, 0, SCREEN_WIDTH / 2, SCREEN_HEIGHT, GRAY);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}