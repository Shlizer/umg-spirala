#pragma once
#include <string>
#include "IScene.h"
#include "StateAnimator.h"

class LogoScene : public IScene {
    TTF_Font* font;
    SDL_Surface* textSurface;
    SDL_Texture* textTexture;

    StateAnimator animator;

    AnimState initState = { 0, 0.4f, 0, -100 };
    AnimState menuUpState = { 255, 1.0f, 0, -210 };
    AnimState menuDownState = { 255, 1.0f, 0, -190 };
    AnimState playingState = { 255, 0.4f, 0, -250 };

public:
    static const string Name;
    string GetName() override { return this->Name; }

    LogoScene(GameContext* ctx) : IScene(ctx) {
        this->font = TTF_OpenFont(CONFIG::FONT_FACE_STYLED, CONFIG::FONT_SIZE_LOGO);
        this->textSurface = TTF_RenderText_Blended(this->font, CONFIG::TITLE, 0, CONFIG::COLOR_LOGO.toColor());
        this->textTexture = SDL_CreateTextureFromSurface(this->Context->renderer, textSurface);

        // Zacznij od ukrytego
        this->animator.SetState(this->initState);
        this->animator.BlendTo(this->menuUpState, 1.2f, [this]() { this->MenuYoyo(); });

        // Słuchaj zmian scen
        this->Context->OnSceneShow.AddListener([this](string sceneName) {
            if (sceneName == "Menu") {
                animator.BlendTo(this->menuUpState, 0.8f, [this]() { this->MenuYoyo(); });
            }
            else if (sceneName == "Playing") {
                this->animator.BlendTo(this->playingState, 0.8f);
            }
        });
    }

    ~LogoScene() {
        SDL_DestroySurface(this->textSurface);
        SDL_DestroyTexture(this->textTexture);
        TTF_CloseFont(this->font);
    }

    void MenuYoyo() {
        this->animator.YoyoBetween(this->menuUpState, this->menuDownState, 1.5f);
    }

    void HandleEvent(const SDL_Event& event) override {
        if (event.type == SDL_EVENT_KEY_DOWN) {
            if (event.key.key == SDLK_ESCAPE) {
                this->Context->OnSceneShow.Invoke("Menu");
            }
        }
    }

    void Update(float deltaTime) override {
        this->animator.Update(deltaTime);
    }

    void Render() override {
        if (!this->textTexture) return;

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
const string LogoScene::Name = "Logo";