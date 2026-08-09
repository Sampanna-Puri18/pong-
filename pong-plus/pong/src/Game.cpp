#include "Game.h"
#include <raymath.h>
#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <algorithm>
#include <iterator>
#include <string>

namespace {
// Palette sampled from the supplied art pack so procedural UI (title text,
// board border, HUD) matches the sprite colours.
constexpr Color kBackground = { 6, 8, 18, 255 };
constexpr Color kBoardLine  = { 46, 40, 58, 255 };
constexpr Color kMagenta    = { 198, 30, 149, 255 };
constexpr Color kMagentaDk  = { 133, 24, 104, 255 };
constexpr Color kCoral      = { 215, 121, 82, 255 };
constexpr Color kBlue       = { 82, 124, 215, 255 };
constexpr Color kCream      = { 240, 232, 235, 255 };
constexpr Color kMuted      = { 150, 150, 165, 255 };

std::string AssetPath(const std::string& relative) {
#ifdef ASSETS_DIR
    return std::string(ASSETS_DIR) + "/" + relative;
#else
    return "assets/" + relative;
#endif
}

float RandRangeF(float lo, float hi) {
    return lo + (hi - lo) * ((float)rand() / (float)RAND_MAX);
}
} // namespace

Game::Game(int screenWidth, int screenHeight)
    : screenWidth(screenWidth), screenHeight(screenHeight) {
    srand((unsigned int)time(nullptr));

    InitWindow(screenWidth, screenHeight, "Pong+");
    SetTargetFPS(60);
    SetExitKey(KEY_NULL); // we handle ESC ourselves so it doesn't instantly close mid-game

    LoadAssets();
    ResetMatch();
}

Game::~Game() {
    UnloadAssets();
    CloseWindow();
}

void Game::LoadAssets() {
    Image boardImg = LoadImage(AssetPath("images/board.png").c_str());
    if (boardImg.data) {
        boardTexture = LoadTextureFromImage(boardImg);
        UnloadImage(boardImg);
    }

    player.LoadSprite(AssetPath("images/paddle_player.png").c_str());
    ai.LoadSprite(AssetPath("images/paddle_ai.png").c_str());
    ball.LoadSprites(AssetPath("images/ball.png").c_str(), AssetPath("images/ball_motion.png").c_str());

    int codepoints[128];
    for (int i = 0; i < 128; ++i) codepoints[i] = 32 + i; // basic ASCII range
    titleFont = LoadFontEx(AssetPath("fonts/Teko-VariableFont.ttf").c_str(), 96, codepoints, 128);
    bodyFont  = LoadFontEx(AssetPath("fonts/Teko-VariableFont.ttf").c_str(), 40, codepoints, 128);
    if (titleFont.texture.id != 0) SetTextureFilter(titleFont.texture, TEXTURE_FILTER_BILINEAR);
    if (bodyFont.texture.id != 0) SetTextureFilter(bodyFont.texture, TEXTURE_FILTER_BILINEAR);

    assetsLoaded = true;
}

void Game::UnloadAssets() {
    if (!assetsLoaded) return;
    UnloadTexture(boardTexture);
    player.UnloadSprite();
    ai.UnloadSprite();
    ball.UnloadSprites();
    UnloadFont(titleFont);
    UnloadFont(bodyFont);
    assetsLoaded = false;
}

void Game::ResetMatch() {
    playerScore = 0;
    aiScore = 0;
    powerUps.clear();
    powerUpSpawnTimer = 5.0f;

    player.width = 16; player.height = 110; player.speed = 420;
    player.x = 34; player.y = screenHeight / 2.0f - player.height / 2.0f;
    player.sizeMultiplier = 1.0f; player.speedMultiplier = 1.0f;
    player.sizeEffectTimer = 0.0f; player.speedEffectTimer = 0.0f;

    ai.width = 16; ai.height = 110; ai.speed = 360;
    ai.x = screenWidth - ai.width - 34; ai.y = screenHeight / 2.0f - ai.height / 2.0f;
    ai.sizeMultiplier = 1.0f; ai.speedMultiplier = 1.0f;
    ai.sizeEffectTimer = 0.0f; ai.speedEffectTimer = 0.0f;
    ai.difficulty = difficulty;

    ball.radius = 12;
    ball.baseSpeed = 320;
    ball.maxSpeed = 780;
    serveDirection = (rand() % 2 == 0) ? -1 : 1;
    ball.Reset((float)screenWidth, (float)screenHeight, serveDirection);
}

void Game::Run() {
    while (!WindowShouldClose()) {
        float dt = GetFrameTime();
        if (dt > 1.0f / 30.0f) dt = 1.0f / 30.0f; // avoid huge steps on lag spikes
        Update(dt);
        Draw();
    }
}

// ---------------------------------------------------------------------
// Update
// ---------------------------------------------------------------------

void Game::Update(float dt) {
    switch (state) {
        case GameState::Title:    UpdateTitle(dt); break;
        case GameState::Serve:    UpdateServe(dt); break;
        case GameState::Playing:  UpdatePlaying(dt); break;
        case GameState::Paused:   UpdatePaused(dt); break;
        case GameState::GameOver: UpdateGameOver(dt); break;
    }
}

void Game::UpdateTitle(float dt) {
    titleBallAngle += dt * 2.2f;

    if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A)) {
        difficulty = (difficulty == Difficulty::Easy) ? Difficulty::Hard
                   : (difficulty == Difficulty::Normal) ? Difficulty::Easy
                   : Difficulty::Normal;
    }
    if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D)) {
        difficulty = (difficulty == Difficulty::Easy) ? Difficulty::Normal
                   : (difficulty == Difficulty::Normal) ? Difficulty::Hard
                   : Difficulty::Easy;
    }
    if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) {
        ResetMatch();
        state = GameState::Serve;
        serveTimer = serveDelay;
    }
    if (IsKeyPressed(KEY_ESCAPE)) {
        CloseWindow();
    }
}

void Game::UpdateServe(float dt) {
    // Let the paddles stay responsive while we count down, but keep the
    // ball frozen so the player can see where it's about to launch from.
    player.TickEffects(dt);
    ai.TickEffects(dt);
    player.Update(dt, (float)screenHeight);
    ai.Update(dt, ball, (float)screenWidth, (float)screenHeight);

    serveTimer -= dt;
    if (serveTimer <= 0.0f) {
        state = GameState::Playing;
    }
    if (IsKeyPressed(KEY_P) || IsKeyPressed(KEY_ESCAPE)) state = GameState::Paused;
}

void Game::UpdatePlaying(float dt) {
    if (IsKeyPressed(KEY_P) || IsKeyPressed(KEY_ESCAPE)) {
        state = GameState::Paused;
        return;
    }

    player.TickEffects(dt);
    ai.TickEffects(dt);
    ball.TickEffects(dt);

    player.Update(dt, (float)screenHeight);
    ai.Update(dt, ball, (float)screenWidth, (float)screenHeight);
    ball.Update(dt);

    HandleBallCollisions();
    UpdatePowerUps(dt);

    powerUpSpawnTimer -= dt;
    if (powerUpSpawnTimer <= 0.0f && powerUps.size() < 2) {
        SpawnPowerUp();
        powerUpSpawnTimer = 7.0f + RandRangeF(0.0f, 4.0f);
    }

    if (ball.x < -40) {
        ResolveScore(Side::AI);
    } else if (ball.x > screenWidth + 40) {
        ResolveScore(Side::Player);
    }
}

void Game::UpdatePaused(float dt) {
    (void)dt;
    if (IsKeyPressed(KEY_P) || IsKeyPressed(KEY_ENTER)) {
        state = GameState::Playing;
    }
    if (IsKeyPressed(KEY_ESCAPE)) {
        state = GameState::Title;
    }
}

void Game::UpdateGameOver(float dt) {
    (void)dt;
    if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) {
        ResetMatch();
        state = GameState::Serve;
        serveTimer = serveDelay;
    }
    if (IsKeyPressed(KEY_ESCAPE)) {
        state = GameState::Title;
    }
}

// ---------------------------------------------------------------------
// Physics / gameplay
// ---------------------------------------------------------------------

void Game::HandleBallCollisions() {
    // Top / bottom walls.
    if (ball.y - ball.radius <= 0.0f) {
        ball.y = ball.radius;
        ball.speedY = fabsf(ball.speedY);
    } else if (ball.y + ball.radius >= screenHeight) {
        ball.y = screenHeight - ball.radius;
        ball.speedY = -fabsf(ball.speedY);
    }

    auto Deflect = [&](Paddle& paddle, bool isPlayerSide) {
        Rectangle rect = paddle.GetRect();
        Rectangle bRect = ball.GetRect();
        if (!CheckCollisionRecs(rect, bRect)) return false;

        // Where the ball hit the paddle, from -1 (top edge) to 1 (bottom edge).
        float paddleCenter = rect.y + rect.height * 0.5f;
        float relative = (ball.y - paddleCenter) / (rect.height * 0.5f);
        relative = Clamp(relative, -1.0f, 1.0f);

        float speed = Vector2Length({ ball.speedX, ball.speedY });
        speed *= 1.045f; // small escalation each hit keeps rallies exciting
        speed = std::min(speed, ball.maxSpeed);

        float maxBounceAngle = 65.0f * DEG2RAD;
        float angle = relative * maxBounceAngle;
        float dir = isPlayerSide ? 1.0f : -1.0f;

        ball.speedX = cosf(angle) * speed * dir;
        ball.speedY = sinf(angle) * speed;

        // Push the ball just outside the paddle so it can't get stuck inside it.
        ball.x = isPlayerSide ? (rect.x + rect.width + ball.radius + 0.5f)
                               : (rect.x - ball.radius - 0.5f);

        ball.lastHitBy = isPlayerSide ? Side::Player : Side::AI;
        return true;
    };

    if (ball.speedX < 0) Deflect(player, true);
    else if (ball.speedX > 0) Deflect(ai, false);
}

void Game::SpawnPowerUp() {
    PowerUp p;
    int roll = rand() % 4;
    p.type = (PowerUpType)roll;
    float margin = 140.0f;
    p.position.x = margin + (float)(rand() % (int)(screenWidth - 2 * margin));
    p.position.y = 60.0f + (float)(rand() % (int)(screenHeight - 120.0f));
    p.radius = 16.0f;
    p.lifeTime = 9.0f;
    powerUps.push_back(p);
}

void Game::UpdatePowerUps(float dt) {
    for (auto& p : powerUps) {
        p.pulseTimer += dt;
        p.lifeTime -= dt;
    }

    Rectangle bRect = ball.GetRect();
    for (auto it = powerUps.begin(); it != powerUps.end();) {
        bool consumed = false;
        if (it->lifeTime <= 0.0f) {
            consumed = true;
        } else if (CheckCollisionRecs(it->GetRect(), bRect)) {
            if (ball.lastHitBy != Side::None) {
                ApplyPowerUp(*it, ball.lastHitBy);
            }
            consumed = true;
        }
        it = consumed ? powerUps.erase(it) : std::next(it);
    }
}

void Game::ApplyPowerUp(const PowerUp& p, Side beneficiary) {
    Paddle& self = (beneficiary == Side::Player) ? static_cast<Paddle&>(player) : static_cast<Paddle&>(ai);
    Paddle& rival = (beneficiary == Side::Player) ? static_cast<Paddle&>(ai) : static_cast<Paddle&>(player);

    switch (p.type) {
        case PowerUpType::GrowSelf:    self.ApplySizeEffect(1.55f, 8.0f); break;
        case PowerUpType::ShrinkRival: rival.ApplySizeEffect(0.6f, 8.0f); break;
        case PowerUpType::SwiftSelf:   self.ApplySpeedEffect(1.6f, 8.0f); break;
        case PowerUpType::SlowBall:    ball.ApplySpeedEffect(0.6f, 5.0f); break;
    }
}

void Game::ResolveScore(Side sideThatScored) {
    if (sideThatScored == Side::Player) {
        playerScore++;
        serveDirection = 1; // serve toward AI (the side that lost the point)
    } else {
        aiScore++;
        serveDirection = -1; // serve toward player
    }

    powerUps.clear();

    if (playerScore >= winningScore || aiScore >= winningScore) {
        lastWinner = (playerScore > aiScore) ? Side::Player : Side::AI;
        state = GameState::GameOver;
        return;
    }

    ball.Reset((float)screenWidth, (float)screenHeight, serveDirection);
    state = GameState::Serve;
    serveTimer = serveDelay;
}

// ---------------------------------------------------------------------
// Draw
// ---------------------------------------------------------------------

void Game::Draw() {
    BeginDrawing();
    ClearBackground(kBackground);

    DrawBackground();

    if (state == GameState::Title) {
        DrawTitleScreen();
    } else {
        // Center dashed line.
        for (float yy = 10; yy < screenHeight; yy += 26) {
            DrawRectangle(screenWidth / 2 - 2, (int)yy, 4, 14, kBoardLine);
        }

        for (auto& p : powerUps) DrawPowerUpIcon(p);

        player.Draw(kCoral);
        ai.Draw(kBlue);
        ball.Draw();

        DrawScore();
        DrawHud();

        if (state == GameState::Paused) DrawPauseOverlay();
        if (state == GameState::GameOver) DrawGameOverScreen();
    }

    EndDrawing();
}

void Game::DrawBackground() {
    // Soft vignette-style border using the board art's sampled palette,
    // procedurally scaled so it always matches the current resolution.
    DrawRectangleRounded({ 6, 6, (float)screenWidth - 12, (float)screenHeight - 12 }, 0.035f, 16, Fade(kCream, 0.06f));
    DrawRectangleRoundedLines({ 6, 6, (float)screenWidth - 12, (float)screenHeight - 12 }, 0.035f, 16, Fade(kCream, 0.35f));
}

void Game::DrawScore() {
    std::string ps = std::to_string(playerScore);
    std::string as = std::to_string(aiScore);

    DrawRectangleRounded({ (float)screenWidth / 2.0f - 160, 14, 320, 46 }, 0.5f, 12, Fade(kMagentaDk, 0.9f));
    DrawTextEx(titleFont, ps.c_str(), { (float)screenWidth / 2.0f - 120, 6 }, 52, 1, kCream);
    DrawTextEx(titleFont, as.c_str(), { (float)screenWidth / 2.0f + 90, 6 }, 52, 1, kCream);
    DrawTextEx(bodyFont, "VS", { (float)screenWidth / 2.0f - 22, 20 }, 28, 1, Fade(kCream, 0.7f));
}

void Game::DrawHud() {
    const char* diffLabel = (difficulty == Difficulty::Easy) ? "EASY" : (difficulty == Difficulty::Hard) ? "HARD" : "NORMAL";
    std::string hud = std::string("DIFFICULTY: ") + diffLabel + "   |   P: PAUSE";
    DrawTextEx(bodyFont, hud.c_str(), { 16, (float)screenHeight - 30 }, 20, 1, Fade(kMuted, 0.85f));
}

void Game::DrawTitleScreen() {
    // Small decorative bouncing ball behind the title.
    float bx = screenWidth / 2.0f + cosf(titleBallAngle) * 220.0f;
    float by = 150.0f + sinf(titleBallAngle * 1.7f) * 18.0f;
    DrawCircle((int)bx, (int)by, 10, Fade(kMagenta, 0.5f));

    const char* title = "PONG +";
    Vector2 titleSize = MeasureTextEx(titleFont, title, 96, 2);
    DrawTextEx(titleFont, title, { screenWidth / 2.0f - titleSize.x / 2.0f, 70 }, 96, 2, kCream);

    const char* subtitle = "now with an AI that actually moves, and power-ups";
    Vector2 subSize = MeasureTextEx(bodyFont, subtitle, 24, 1);
    DrawTextEx(bodyFont, subtitle, { screenWidth / 2.0f - subSize.x / 2.0f, 190 }, 24, 1, kMuted);

    // Little paddle glyphs either side of the title for flavour.
    DrawRectangleRounded({ screenWidth / 2.0f - 260, 90, 14, 70 }, 0.4f, 6, kCoral);
    DrawRectangleRounded({ screenWidth / 2.0f + 246, 90, 14, 70 }, 0.4f, 6, kBlue);

    const char* diffLabel = (difficulty == Difficulty::Easy) ? "EASY" : (difficulty == Difficulty::Hard) ? "HARD" : "NORMAL";
    std::string diffLine = std::string("< ") + diffLabel + " >";
    Vector2 diffSize = MeasureTextEx(titleFont, diffLine.c_str(), 44, 1);
    DrawTextEx(bodyFont, "DIFFICULTY", { screenWidth / 2.0f - 60, 300 }, 20, 1, kMuted);
    DrawTextEx(titleFont, diffLine.c_str(), { screenWidth / 2.0f - diffSize.x / 2.0f, 320 }, 44, 1, kMagenta);

    const char* prompt = "PRESS ENTER TO START";
    Vector2 promptSize = MeasureTextEx(titleFont, prompt, 40, 1);
    float alpha = 0.55f + 0.45f * fabsf(sinf(titleBallAngle * 1.3f));
    DrawTextEx(titleFont, prompt, { screenWidth / 2.0f - promptSize.x / 2.0f, 400 }, 40, 1, Fade(kCream, alpha));

    const char* controls = "W/S OR ARROW KEYS TO MOVE   -   ESC TO QUIT";
    Vector2 ctrlSize = MeasureTextEx(bodyFont, controls, 20, 1);
    DrawTextEx(bodyFont, controls, { screenWidth / 2.0f - ctrlSize.x / 2.0f, 460 }, 20, 1, kMuted);

    const char* powerupHint = "Hit a glowing power-up with the ball to trigger its effect";
    Vector2 hintSize = MeasureTextEx(bodyFont, powerupHint, 18, 1);
    DrawTextEx(bodyFont, powerupHint, { screenWidth / 2.0f - hintSize.x / 2.0f, 490 }, 18, 1, Fade(kMuted, 0.8f));
}

void Game::DrawPauseOverlay() {
    DrawRectangle(0, 0, screenWidth, screenHeight, Fade(BLACK, 0.55f));
    const char* text = "PAUSED";
    Vector2 size = MeasureTextEx(titleFont, text, 64, 1);
    DrawTextEx(titleFont, text, { screenWidth / 2.0f - size.x / 2.0f, screenHeight / 2.0f - 60 }, 64, 1, kCream);
    const char* hint = "P TO RESUME   -   ESC FOR TITLE";
    Vector2 hSize = MeasureTextEx(bodyFont, hint, 22, 1);
    DrawTextEx(bodyFont, hint, { screenWidth / 2.0f - hSize.x / 2.0f, screenHeight / 2.0f + 10 }, 22, 1, kMuted);
}

void Game::DrawGameOverScreen() {
    DrawRectangle(0, 0, screenWidth, screenHeight, Fade(BLACK, 0.6f));
    const char* text = (lastWinner == Side::Player) ? "YOU WIN!" : "AI WINS";
    Color c = (lastWinner == Side::Player) ? kCoral : kBlue;
    Vector2 size = MeasureTextEx(titleFont, text, 80, 1);
    DrawTextEx(titleFont, text, { screenWidth / 2.0f - size.x / 2.0f, screenHeight / 2.0f - 90 }, 80, 1, c);

    std::string scoreLine = std::to_string(playerScore) + " - " + std::to_string(aiScore);
    Vector2 sSize = MeasureTextEx(titleFont, scoreLine.c_str(), 44, 1);
    DrawTextEx(titleFont, scoreLine.c_str(), { screenWidth / 2.0f - sSize.x / 2.0f, screenHeight / 2.0f - 5 }, 44, 1, kCream);

    const char* hint = "ENTER TO REMATCH   -   ESC FOR TITLE";
    Vector2 hSize = MeasureTextEx(bodyFont, hint, 22, 1);
    DrawTextEx(bodyFont, hint, { screenWidth / 2.0f - hSize.x / 2.0f, screenHeight / 2.0f + 60 }, 22, 1, kMuted);
}
