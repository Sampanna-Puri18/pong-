#include "PowerUp.h"
#include <cmath>

const char* PowerUpLabel(PowerUpType type) {
    switch (type) {
        case PowerUpType::GrowSelf:    return "GROW";
        case PowerUpType::ShrinkRival: return "SHRINK";
        case PowerUpType::SwiftSelf:   return "SPEED";
        case PowerUpType::SlowBall:    return "SLOW";
    }
    return "";
}

Color PowerUpColor(PowerUpType type) {
    switch (type) {
        case PowerUpType::GrowSelf:    return { 108, 214, 148, 255 }; // green
        case PowerUpType::ShrinkRival: return { 235, 111, 111, 255 }; // red
        case PowerUpType::SwiftSelf:   return { 244, 208, 63, 255 };  // yellow
        case PowerUpType::SlowBall:    return { 96, 199, 227, 255 };  // cyan
    }
    return WHITE;
}

void DrawPowerUpIcon(const PowerUp& p) {
    Color c = PowerUpColor(p.type);
    float pulse = 1.0f + 0.08f * sinf(p.pulseTimer * 4.0f);
    float r = p.radius * pulse;

    DrawCircleV(p.position, r + 4.0f, Fade(c, 0.20f));
    DrawCircleV(p.position, r, Fade(c, 0.85f));
    DrawCircleLines((int)p.position.x, (int)p.position.y, r, c);

    float x = p.position.x, y = p.position.y;
    switch (p.type) {
        case PowerUpType::GrowSelf: {
            // Outward-pointing double arrow (grow).
            DrawTriangle({ x - 5, y - 3 }, { x + 5, y - 3 }, { x, y - 10 }, BLACK);
            DrawTriangle({ x, y + 10 }, { x + 5, y + 3 }, { x - 5, y + 3 }, BLACK);
            break;
        }
        case PowerUpType::ShrinkRival: {
            // Inward-pointing double arrow (shrink).
            DrawTriangle({ x - 5, y - 8 }, { x + 5, y - 8 }, { x, y - 1 }, BLACK);
            DrawTriangle({ x, y + 1 }, { x + 5, y + 8 }, { x - 5, y + 8 }, BLACK);
            break;
        }
        case PowerUpType::SwiftSelf: {
            // Lightning bolt.
            Vector2 pts[5] = {
                { x + 2, y - 10 }, { x - 6, y + 1 }, { x - 1, y + 1 },
                { x - 2, y + 10 }, { x + 6, y - 2 }
            };
            DrawTriangle(pts[0], pts[1], pts[2], BLACK);
            DrawTriangle(pts[2], pts[4], pts[0], BLACK);
            DrawTriangle(pts[2], pts[3], pts[4], BLACK);
            break;
        }
        case PowerUpType::SlowBall: {
            // Little clock face.
            DrawCircleLines((int)x, (int)y, 7, BLACK);
            DrawLineEx({ x, y }, { x, y - 5 }, 1.6f, BLACK);
            DrawLineEx({ x, y }, { x + 4, y + 1 }, 1.6f, BLACK);
            break;
        }
    }
}
