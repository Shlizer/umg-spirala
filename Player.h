#pragma once
#include <random>
#include <vector>
#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_scancode.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include "Utils.h"
#include "Static.h"

using namespace std;
using namespace UTILS;
using namespace CONFIG;

struct PlayerPosition {
    float x, y, angle;
};

enum class PlayerState {
    WarmingUp,  // Przed startem, pokazywana strzałka
    Playing,    // Aktywna gra
    Dead        // Po kolizji
};

class Player {
private:
    GameContext* Context;
    PlayerPosition position;
    PlayerState state = PlayerState::WarmingUp;

    float prevX, prevY;
    float speed = 0.8f;
    float turnSpeed = 0.05f;
    Color color;
    SDL_Scancode keyLeft, keyRight;

    vector<SDL_FPoint> trail;
    mt19937 gen;
    int lineThickness = 5;

    SDL_FPoint GetScaledPosition(float scale) const {
        SDL_FPoint p;

        float centerX = this->Context->windowWidth * 0.5f;
        float centerY = this->Context->windowHeight * 0.5f;

        // Odległość gracza od środka okna
        float dx = position.x - centerX;
        float dy = position.y - centerY;

        // Skalowanie względem środka
        p.x = centerX + dx * scale;
        p.y = centerY + dy * scale;

        return p;
    }

public:
    Player(GameContext* Context, Color color, SDL_Scancode keyLeft, SDL_Scancode keyRight)
        : Context(Context), color(color), keyLeft(keyLeft), keyRight(keyRight) {
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
                prevX = position.x;
                prevY = position.y;
                return this;
            }
        }

        position.x = (minX + maxX) / 2;
        position.y = (minY + maxY) / 2;
        position.angle = distAngle(gen);
        prevX = position.x;
        prevY = position.y;
        return this;
    }

    void SetAngle(float a) { this->position.angle = a; }

    void HandleInput(const bool* keystate) {
        if (state != PlayerState::Playing) return;
        if (keystate[keyLeft]) this->position.angle -= turnSpeed;
        if (keystate[keyRight]) this->position.angle += turnSpeed;
    }

    void Update(float deltaTime) {
        if (state != PlayerState::Playing) return;

        prevX = this->position.x;
        prevY = this->position.y;

        this->position.x += cosf(this->position.angle) * speed;
        this->position.y += sinf(this->position.angle) * speed;

        trail.push_back({ this->position.x, this->position.y });
    }

    void Draw() {
        if (trail.size() < 2) return;
        SDL_SetRenderDrawColor(this->Context->renderer, color.r, color.g, color.b, color.a);

        for (size_t i = 1; i < trail.size(); i++) {
            // Oblicz kierunek linii
            float dx = trail[i].x - trail[i - 1].x;
            float dy = trail[i].y - trail[i - 1].y;
            float len = sqrtf(dx * dx + dy * dy);

            if (len < 0.001f) continue; // Pomiń jeśli punkty są w tym samym miejscu

            // Wektor prostopadły (obrócony o 90°)
            float perpX = -dy / len;
            float perpY = dx / len;

            // Rysuj linie przesunięte wzdłuż wektora prostopadłego
            for (int offset = -lineThickness / 2; offset <= lineThickness / 2; offset++) {
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

        // Okrąg oznaczający pozycję startową
        SDL_SetRenderDrawColor(this->Context->renderer, color.r, color.g, color.b, static_cast<Uint8>(alpha));

        float radius = 8.0f;
        int segments = 16;

        // Rysuj wypełniony okrąg
        for (int i = 0; i < segments; i++) {
            float angle1 = (2.0f * static_cast<float>(PI) * i) / segments;
            float angle2 = (2.0f * static_cast<float>(PI) * (i + 1)) / segments;

            SDL_RenderLine(this->Context->renderer,
                pos.x + cosf(angle1) * radius,
                pos.y + sinf(angle1) * radius,
                pos.x + cosf(angle2) * radius,
                pos.y + sinf(angle2) * radius);
        }

        // Strzałka wskazująca kierunek
        float arrowLength = 20.0f;
        float arrowWidth = 8.0f;

        // Punkt końcowy strzałki (w kierunku angle)
        float endX = pos.x + cosf(this->position.angle) * arrowLength;
        float endY = pos.y + sinf(this->position.angle) * arrowLength;

        // Linia główna strzałki
        for (int i = -1; i <= 1; i++) {
            SDL_RenderLine(this->Context->renderer,
                pos.x + i, pos.y,
                endX + i, endY);
        }  

        // Groty strzałki (dwa boki trójkąta)
        float arrowAngle = 0.5f; // ~30 stopni
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

    // Gettery dla kolizji
    const vector<SDL_FPoint>& GetTrail() const { return trail; }
    int GetLineThickness() const { return lineThickness; }
    bool IsAlive() const { return state == PlayerState::Playing; }
    float GetX() const { return this->position.x; }
    float GetY() const { return this->position.y; }
};