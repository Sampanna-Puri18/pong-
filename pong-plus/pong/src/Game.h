#pragma once
#include <raylib.h>
#include <vector>
#include "Ball.h"
#include "PlayerPaddle.h"
#include "AIPaddle.h"
#include "PowerUp.h"

enum class GameState { Title, Serve, Playing, Paused, GameOver };

class Game {
public:
    Game(int screenWidth, int screenHeight);
    ~Game();

    void Run();

private:
    int screenWidth;
    int screenHeight;
    GameState state = GameState::Title;

    Ball ball;
    PlayerPaddle player;
    AIPaddle ai;
    Difficulty difficulty = Difficulty::Normal;

    int playerScore = 0;
    int aiScore = 0;
    const int winningScore = 7;
    Side lastWinner = Side::None;

    int serveDirection = -1;      // who the next serve goes toward
    float serveTimer = 0.0f;
    const float serveDelay = 1.0f;

    std::vector<PowerUp> powerUps;
    float powerUpSpawnTimer = 0.0f;

    // Assets
    Texture2D boardTexture{};
    Font titleFont{};
    Font bodyFont{};
    bool assetsLoaded = false;

    float titleBallAngle = 0.0f;

    void LoadAssets();
    void UnloadAssets();

    void Update(float dt);
    void UpdateTitle(float dt);
    void UpdateServe(float dt);
    void UpdatePlaying(float dt);
    void UpdatePaused(float dt);
    void UpdateGameOver(float dt);

    void HandleBallCollisions();
    void SpawnPowerUp();
    void UpdatePowerUps(float dt);
    void ApplyPowerUp(const PowerUp& p, Side beneficiary);
    void ResolveScore(Side sideThatScored);

    void Draw();
    void DrawBackground();
    void DrawScore();
    void DrawTitleScreen();
    void DrawPauseOverlay();
    void DrawGameOverScreen();
    void DrawHud();

    void ResetMatch();
};
