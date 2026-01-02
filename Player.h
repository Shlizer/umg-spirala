#pragma once
#include <random>
#include <SDL3/SDL.h>
#include "Utils.h"
#include "Config.h"
#include "CollisionSystem.h"

using namespace std;
using namespace UTILS;
using namespace CONFIG;

struct PlayerPosition {
    float x, y, angle;
};

enum class PlayerState {
    WarmingUp,
    Playing,
    Dead
};

class Player {
private:
    GameContext* Context;
    PlayerPosition position;
    PlayerPosition prevPosition;
    PlayerState state = PlayerState::WarmingUp;
    PlayerInfo info;
    bool hasPrevPosition = false;

    int playerId = -1;
    int lastPixelX = -1;
    int lastPixelY = -1;
    int pixelMoveCount = 0;

    float trailTimer = 0.0f;
    bool isDrawing = true;

    SDL_FPoint GetScaledPosition(float scale) const {
        float centerX = this->Context->windowWidth * 0.5f;
        float centerY = this->Context->windowHeight * 0.5f;
        float dx = position.x - centerX;
        float dy = position.y - centerY;
        return { centerX + dx * scale, centerY + dy * scale };
    }

public:
    Player(GameContext* Context, PlayerInfo info, int id) : Context(Context), info(info), playerId(id) {}

    Player* SetRandomPosition(float edgeMargin, float playerDistance, const vector<Player*>& others) {
        mt19937 gen{ random_device{}() };
        float minX = SCENE_GAMEPLAY_PADDING_L + edgeMargin;
        float maxX = this->Context->windowWidth - SCENE_GAMEPLAY_PADDING_R - edgeMargin;
        float minY = SCENE_GAMEPLAY_PADDING_T + edgeMargin;
        float maxY = this->Context->windowHeight - SCENE_GAMEPLAY_PADDING_B - edgeMargin;

        uniform_real_distribution<float> distX(minX, maxX);
        uniform_real_distribution<float> distY(minY, maxY);
        uniform_real_distribution<float> distAngle(0, 2 * static_cast<float>(PI));

        for (int attempt = 0; attempt < 100; attempt++) {
            float x = distX(gen);
            float y = distY(gen);

            bool validPosition = true;
            for (const auto* player : others) {
                float dx = player->GetX() - x;
                float dy = player->GetY() - y;
                if (sqrtf(dx * dx + dy * dy) < playerDistance) {
                    validPosition = false;
                    break;
                }
            }

            if (validPosition) {
                position.x = x;
                position.y = y;
                position.angle = distAngle(gen);
                return this;
            }
        }

        position.x = (minX + maxX) / 2;
        position.y = (minY + maxY) / 2;
        position.angle = distAngle(gen);
        return this;
    }

    void HandleInput(const bool* keystate) {
        if (state != PlayerState::Playing) return;
        if (keystate[info.keyLeft]) this->position.angle -= PLAYER_TURN_SPEED;
        if (keystate[info.keyRight]) this->position.angle += PLAYER_TURN_SPEED;
    }

    void Update(float deltaTime) {
        if (state != PlayerState::Playing) return;

        this->position.x += cosf(this->position.angle) * PLAYER_SPEED;
        this->position.y += sinf(this->position.angle) * PLAYER_SPEED;

        this->trailTimer += deltaTime;
        if (this->isDrawing && this->trailTimer >= TRAIL_GAP_INTERVAL) {
            this->isDrawing = false;
            this->trailTimer = 0.0f;
        }
        else if (!this->isDrawing && this->trailTimer >= TRAIL_GAP_DURATION) {
            this->isDrawing = true;
            this->trailTimer = 0.0f;
        }

        this->hasPrevPosition = true;
    }

    bool CheckCollisionAndDraw(CollisionSystem& collision) {
        if (!this->hasPrevPosition) return false;

        int currPixelX = (int)this->position.x;
        int currPixelY = (int)this->position.y;

        if (currPixelX == lastPixelX && currPixelY == lastPixelY) {
            return false;
        }

        float fromX = (lastPixelX >= 0) ? (float)lastPixelX : this->position.x;
        float fromY = (lastPixelY >= 0) ? (float)lastPixelY : this->position.y;

        SDL_Color color = info.color.toColor();
        float collisionX, collisionY;

        bool skipCollision = (pixelMoveCount < PLAYER_THICKNESS * 2);

        bool hit = collision.CheckAndDrawLine(
            fromX, fromY,
            this->position.x, this->position.y,
            PLAYER_THICKNESS,
            color,
            skipCollision,
            !this->isDrawing,
            &collisionX, &collisionY
        );

        lastPixelX = currPixelX;
        lastPixelY = currPixelY;
        pixelMoveCount++;

        if (hit) {
            this->position.x = collisionX;
            this->position.y = collisionY;
        }

        return hit;
    }

    void DrawStart(float scale, float alpha) {
        auto pos = GetScaledPosition(scale);
        SDL_SetRenderDrawColor(this->Context->renderer, info.color.r, info.color.g, info.color.b, static_cast<Uint8>(alpha));

        float radius = 8.0f;
        int segments = 16;
        for (int i = 0; i < segments; i++) {
            float angle1 = (2.0f * static_cast<float>(PI) * i) / segments;
            float angle2 = (2.0f * static_cast<float>(PI) * (i + 1)) / segments;
            SDL_RenderLine(this->Context->renderer,
                pos.x + cosf(angle1) * radius, pos.y + sinf(angle1) * radius,
                pos.x + cosf(angle2) * radius, pos.y + sinf(angle2) * radius);
        }

        float arrowLength = 20.0f;
        float arrowWidth = 8.0f;
        float endX = pos.x + cosf(this->position.angle) * arrowLength;
        float endY = pos.y + sinf(this->position.angle) * arrowLength;

        for (int i = -1; i <= 1; i++) {
            SDL_RenderLine(this->Context->renderer, pos.x + i, pos.y, endX + i, endY);
        }

        float arrowAngle = 0.5f;
        SDL_RenderLine(this->Context->renderer, endX, endY,
            endX - cosf(this->position.angle - arrowAngle) * arrowWidth,
            endY - sinf(this->position.angle - arrowAngle) * arrowWidth);
        SDL_RenderLine(this->Context->renderer, endX, endY,
            endX - cosf(this->position.angle + arrowAngle) * arrowWidth,
            endY - sinf(this->position.angle + arrowAngle) * arrowWidth);
    }

    void Start() {
        this->state = PlayerState::Playing;
        this->prevPosition = this->position;
        this->hasPrevPosition = true;
        this->lastPixelX = (int)this->position.x;
        this->lastPixelY = (int)this->position.y;
        this->pixelMoveCount = 0;
        this->trailTimer = 0.0f;
        this->isDrawing = true;
    }

    void Kill() {
        this->state = PlayerState::Dead;

        KillEvent event;
        event.victimId = this->playerId;
        this->Context->killLog.push_back(event);
        this->Context->aliveCount--;
    }

    bool IsAlive() const { return state == PlayerState::Playing; }
    float GetX() const { return this->position.x; }
    float GetY() const { return this->position.y; }
    int GetId() const { return this->playerId; }
    SDL_Color GetColor() const { return info.color.toColor(); }
};