#pragma once
#include <SDL3_ttf/SDL_ttf.h>
#include "ITask.h"
#include "GameContext.h"
#include "Config.h"
#include "Utils.h"

class TaskEndGame : public ITask {
    GameContext* Context;
    TTF_Font* font;
    SDL_Texture* textTexture;
    float w, h;

public:
    TaskEndGame(GameContext* Context, const char* text) : Context(Context), ITask(0) {
        this->font = UTILS::loadFont(CONFIG::FONT_FACE_STYLED, CONFIG::FONT_SIZE_COUNTER);
        SDL_Surface* surface = TTF_RenderText_Blended(this->font, text, 0, { 255,255,255,255 });
        this->textTexture = SDL_CreateTextureFromSurface(Context->renderer, surface);
        this->w = (float)surface->w;
        this->h = (float)surface->h;
        SDL_DestroySurface(surface);
    }

    ~TaskEndGame() {
        SDL_DestroyTexture(this->textTexture);
        TTF_CloseFont(this->font);
    }

    bool UpdateDelayed(float deltaTime) override {
        return false;
    }

    void Render() override {
        SDL_FRect rect = {
            (this->Context->windowWidth - w) / 2.0f,
            (this->Context->windowHeight - h) / 2.0f,
            w, h
        };
        SDL_RenderTexture(this->Context->renderer, this->textTexture, nullptr, &rect);
    }
};
