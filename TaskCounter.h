#pragma once
#include <algorithm>
#include <SDL3_ttf/SDL_ttf.h>
#include <string>
#include <functional>
#include "ITask.h"
#include "GameContext.h"
#include "Utils.h"

class TaskCounter : public ITask {
    const float FONT_SIZE = 120;
    SDL_Color color = { 255,255,255,255 };

    bool animationStarted = false;
    float duration;
    float elapsed = 0;
    const char* text;

    GameContext* Context;
    TTF_Font* font;
    SDL_Texture* textTexture;

    StateAnimator animator;

public:
    TaskCounter(GameContext* Context, const char* text, float duration, float delay = 0) : Context(Context), duration(duration), text(text), ITask(delay) {
        this->font = UTILS::loadFont(CONFIG::FONT_FACE_STYLED, FONT_SIZE);
        this->animator.setSmooth(true);
        this->animator.SetState(CONFIG::ANIM_COUNTDOWN_START);

        SDL_Surface* surface = TTF_RenderText_Blended(this->font, text, 0, this->color);
        this->textTexture = SDL_CreateTextureFromSurface(Context->renderer, surface);
        SDL_DestroySurface(surface);
    }

    ~TaskCounter() {
        SDL_DestroyTexture(this->textTexture);
        TTF_CloseFont(this->font);
    }

    bool UpdateDelayed(float deltaTime) override {
        if (!this->animationStarted) {
            this->animator.BlendTo(CONFIG::ANIM_COUNTDOWN_END, 1.0f);
            this->animationStarted = true;
        }
        this->animator.Update(deltaTime);

        elapsed += deltaTime;
        return elapsed >= duration;
    }

    void Render() override {
        if (!this->animationStarted) return;

        AnimState state = this->animator.GetCurrent();

        float w, h;
        SDL_GetTextureSize(this->textTexture, &w, &h);

        float scaledW = w * state.scale;
        float scaledH = h * state.scale;

        SDL_FRect rect = {
            (this->Context->windowWidth - scaledW) / 2.0f + state.x,
            (this->Context->windowHeight - scaledH) / 2.0f + state.y,
            scaledW,
            scaledH
        };

        SDL_SetTextureAlphaMod(this->textTexture, (Uint8)state.alpha);
        SDL_RenderTexture(this->Context->renderer, this->textTexture, NULL, &rect);
    }
};