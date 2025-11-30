#pragma once
#include <random>
#include <vector>
#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_scancode.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include "Utils.h"
#include "Config.h"

using namespace std;
using namespace UTILS;
using namespace CONFIG;

struct PlayerPosition {
    float x, y, angle;
};

enum class PlayerState {
    WarmingUp,  // Before start - show position
    Playing,
    Dead
};

class Player {
private:
    GameContext* Context;
    PlayerPosition position;
    PlayerState state = PlayerState::WarmingUp;

    Color color;
    SDL_Scancode keyLeft, keyRight;

    vector<SDL_FPoint> trail;
    mt19937 gen;

    SDL_FPoint GetScaledPosition(float scale) const {
        SDL_FPoint p;

        float centerX = this->Context->windowWidth * 0.5f;
        float centerY = this->Context->windowHeight * 0.5f;

        // Player distance from center
        float dx = position.x - centerX;
        float dy = position.y - centerY;

        // Scale from center
        p.x = centerX + dx * scale;
        p.y = centerY + dy * scale;

        return p;
    }

public:
    Player(GameContext* Context, Color color, SDL_Scancode keyLeft, SDL_Scancode keyRight)
        : Context(Context), color(color), keyLeft(keyLeft), keyRight(keyRight) {
        mt19937 gen{ std::random_device{}() };
    }

    Player* SetRandomPosition(float edgeMargin, float playerDistance, const vector<Player*>& others) {
        float minX = SCENE_GAMEPLAY_PADDING_L + edgeMargin;
        float maxX = this->Context->windowWidth - SCENE_GAMEPLAY_PADDING_R - edgeMargin;
        float minY = SCENE_GAMEPLAY_PADDING_T + edgeMargin;
        float maxY = this->Context->windowHeight - SCENE_GAMEPLAY_PADDING_B - edgeMargin;

        uniform_real_distribution<float> distX(minX, maxX);
        uniform_real_distribution<float> distY(minY, maxY);
        uniform_real_distribution<float> distAngle(0, 2 * static_cast<float>(PI));

        int maxAttempts = 100;

        for (int attempt = 0; attempt < maxAttempts; attempt++) {
            float x = distX(gen);
            float y = distY(gen);
            float angle = distAngle(gen);

            bool validPosition = true;
            for (const auto* player : others) {
                float dx = player->GetX() - x;
                float dy = player->GetY() - y;
                float dist = sqrtf(dx * dx + dy * dy);

                if (dist < playerDistance) {
                    validPosition = false;
                    break;
                }
            }

            if (validPosition) {
                position.x = x;
                position.y = y;
                position.angle = angle;
                return this;
            }
        }

        position.x = (minX + maxX) / 2;
        position.y = (minY + maxY) / 2;
        position.angle = distAngle(gen);
        return this;
    }

    void SetAngle(float a) { this->position.angle = a; }

    void HandleInput(const bool* keystate) {
        if (state != PlayerState::Playing) return;
        if (keystate[keyLeft]) this->position.angle -= PLAYER_TURN_SPEED;
        if (keystate[keyRight]) this->position.angle += PLAYER_TURN_SPEED;
    }

    void Update(float deltaTime) {
        if (state != PlayerState::Playing) return;

        this->position.x += cosf(this->position.angle) * PLAYER_SPEED;
        this->position.y += sinf(this->position.angle) * PLAYER_SPEED;

        trail.push_back({ this->position.x, this->position.y });
    }

    void Draw() {
        if (trail.size() < 2) return;
        SDL_SetRenderDrawColor(this->Context->renderer, color.r, color.g, color.b, color.a);

        for (size_t i = 1; i < trail.size(); i++) {
            // Line direction
            float dx = trail[i].x - trail[i - 1].x;
            float dy = trail[i].y - trail[i - 1].y;
            float len = sqrtf(dx * dx + dy * dy);

            if (len < 0.001f) continue; // Ignore if distance is too short

            // Perpendicural (90deg)
            float perpX = -dy / len;
            float perpY = dx / len;

            // Draw line alongside offset
            for (int offset = -PLAYER_THICKNESS / 2; offset <= PLAYER_THICKNESS / 2; offset++) {
                float offsetX = perpX * offset;
                float offsetY = perpY * offset;

                SDL_RenderLine(this->Context->renderer,
                    trail[i - 1].x + offsetX, trail[i - 1].y + offsetY,
                    trail[i].x + offsetX, trail[i].y + offsetY);
            }
        }
    }

    void DrawStart(float scale, float alpha) {
        auto pos = GetScaledPosition(scale);

        // Circle in start position
        SDL_SetRenderDrawColor(this->Context->renderer, color.r, color.g, color.b, static_cast<Uint8>(alpha));

        float radius = 8.0f;
        int segments = 16;

        // Draw circle
        for (int i = 0; i < segments; i++) {
            float angle1 = (2.0f * static_cast<float>(PI) * i) / segments;
            float angle2 = (2.0f * static_cast<float>(PI) * (i + 1)) / segments;

            SDL_RenderLine(this->Context->renderer,
                pos.x + cosf(angle1) * radius,
                pos.y + sinf(angle1) * radius,
                pos.x + cosf(angle2) * radius,
                pos.y + sinf(angle2) * radius);
        }

        // Direction arrow
        float arrowLength = 20.0f;
        float arrowWidth = 8.0f;

        // Arrow point
        float endX = pos.x + cosf(this->position.angle) * arrowLength;
        float endY = pos.y + sinf(this->position.angle) * arrowLength;

        // Arrow main line
        for (int i = -1; i <= 1; i++) {
            SDL_RenderLine(this->Context->renderer,
                pos.x + i, pos.y,
                endX + i, endY);
        }  

        // Arrow side lines
        float arrowAngle = 0.5f; // ~30 deg
        float leftX = endX - cosf(this->position.angle - arrowAngle) * arrowWidth;
        float leftY = endY - sinf(this->position.angle - arrowAngle) * arrowWidth;
        float rightX = endX - cosf(this->position.angle + arrowAngle) * arrowWidth;
        float rightY = endY - sinf(this->position.angle + arrowAngle) * arrowWidth;

        SDL_RenderLine(this->Context->renderer, endX, endY, leftX, leftY);
        SDL_RenderLine(this->Context->renderer, endX, endY, rightX, rightY);
    }

    void Start() {
        this->state = PlayerState::Playing;
    }
    void Kill() {
        this->state = PlayerState::Dead;
    }

    // Getters for collision
    const vector<SDL_FPoint>& GetTrail() const { return trail; }
    int GetLineThickness() const { return PLAYER_THICKNESS; }
    bool IsAlive() const { return state == PlayerState::Playing; }
    float GetX() const { return this->position.x; }
    float GetY() const { return this->position.y; }
};