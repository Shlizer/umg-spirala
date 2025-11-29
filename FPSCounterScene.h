#pragma once
#include <SDL3_ttf/SDL_ttf.h>
#include "IScene.h"
#include "Config.h"
#include "Utils.h"

using namespace CONFIG;
using namespace UTILS;

class FPSCounterScene : public IScene {
    TTF_Font* font;
    SDL_Surface* textSurface;
    SDL_Texture* textTexture;

    char buffer[32] = "";
    SDL_Color color = { 255,255,0,120 };

    Uint64 lastTime = 0;
    int frameCount = 0;
    float fps = 0.0f;
    float updateInterval = 0.5f;
    bool visible = false;

    void UpdateTexture() {
        if (this->textTexture) SDL_DestroyTexture(this->textTexture);
        if (this->textSurface) SDL_DestroySurface(this->textSurface);

        SDL_snprintf(this->buffer, sizeof(this->buffer), "FPS: %.1f", this->fps);
        this->textSurface = TTF_RenderText_Solid(this->font, this->buffer, 0, this->color);
        this->textTexture = SDL_CreateTextureFromSurface(this->Context->renderer, this->textSurface);
    }

public:
    static const string Name;
    string GetName() override { return Name; }

    FPSCounterScene(GameContext* ctx) : IScene(ctx) {
        this->font = loadFont(FONT_FACE_LATO_REGULAR, FONT_SIZE_DEBUG_FPS);
        this->lastTime = SDL_GetTicks();
    }

    ~FPSCounterScene() {
        SDL_DestroySurface(this->textSurface);
        SDL_DestroyTexture(this->textTexture);
        TTF_CloseFont(this->font);
    }

    void HandleEvent(const SDL_Event& event) override {
        if (event.type == SDL_EVENT_KEY_DOWN) {
            if (event.key.key == KEY_TOGGLE_FPS) {
                this->visible = !this->visible;

                if (this->visible) {
                    this->lastTime = SDL_GetTicks();
                    this->frameCount = 0;
                }
            }
        }
    }

    void Update(float deltaTime) override {
        if (!this->visible) return;

        this->frameCount++;
        Uint64 now = SDL_GetTicks();
        float elapsed = (now - this->lastTime) / 1000.0f;

        if (elapsed >= this->updateInterval) {
            float newFps = this->frameCount / elapsed;

            if (newFps != this->fps) {
                this->fps = newFps;
                UpdateTexture();
            }

            this->frameCount = 0;
            this->lastTime = now;
        }
    }

    void Render() override {
        if (!this->textTexture || !this->visible) return;
        SDL_FRect rect = { 10, 10, (float)this->textSurface->w, (float)this->textSurface->h };
        SDL_RenderTexture(this->Context->renderer, this->textTexture, nullptr, &rect);
    }
};

const string FPSCounterScene::Name = "FPSCounter";
