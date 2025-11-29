#pragma once
#include <string>
#include "Static.h"
#include "IScene.h"
#include "StateAnimator.h"
#include "Utils.h"

class LogoScene : public IScene {
    TTF_Font* font;
    SDL_Surface* textSurface;
    SDL_Texture* textTexture;

    StateAnimator animator;

public:
    static const string Name;
    string GetName() override { return Name; }

    LogoScene(GameContext* ctx) : IScene(ctx) {
        this->font = UTILS::loadFont(CONFIG::FONT_FACE_STYLED, CONFIG::FONT_SIZE_LOGO);
        this->textSurface = TTF_RenderText_Blended(this->font, CONFIG::TITLE, 0, CONFIG::COLOR_LOGO.toColor());
        this->textTexture = SDL_CreateTextureFromSurface(this->Context->renderer, textSurface);

        this->animator.setSmooth(true);
        this->animator.SetState(CONFIG::ANIM_LOGO_INIT);
        this->animator.BlendTo(CONFIG::ANIM_LOGO_MENU_UP, CONFIG::TIME_LOGO_SHOW, [this]() { this->MenuYoyo(); });

        this->Context->OnSceneShow.AddListener([this](string sceneName) {
            if (sceneName == MenuScene::Name) {
                this->animator.BlendTo(CONFIG::ANIM_LOGO_MENU_UP, CONFIG::TIME_LOGO_SWITCH_MENU, [this]() { this->MenuYoyo(); });
            }
            else if (sceneName == GameplayScene::Name) {
                this->animator.BlendTo(CONFIG::ANIM_LOGO_GAMEPLAY, CONFIG::TIME_LOGO_SWITCH_GAMEPLAY);
            }
        });
    }

    ~LogoScene() {
        SDL_DestroySurface(this->textSurface);
        SDL_DestroyTexture(this->textTexture);
        TTF_CloseFont(this->font);
    }

    void MenuYoyo() {
        this->Context->OnSceneShow.Invoke(MenuScene::Name);
        this->animator.YoyoBetween(CONFIG::ANIM_LOGO_MENU_UP, CONFIG::ANIM_LOGO_MENU_DOWN, CONFIG::TIME_LOGO_MENU_YOYO);
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
            state.y,
            scaledW,
            scaledH
        };

        SDL_SetTextureAlphaMod(this->textTexture, (Uint8)state.alpha);
        SDL_RenderTexture(this->Context->renderer, this->textTexture, NULL, &rect);
    }
};
const string LogoScene::Name = "Logo";