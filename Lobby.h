#pragma once
#include <string>
#include "TaskCounter.h"
#include "IScene.h"
#include "MenuScene.h"
#include "Utils.h"

using namespace std;

class LobbyScene : public IScene {
    bool isHiding = false;
    float const TIME_SHOW = 2.0f;
    int const PADDING_X = 70;
    int const PADDING_TOP = 90;
    int const PADDING_BOT = 50;
    const float FONT_SIZE = 120;
    SDL_Color colorBg = { 0,0,0,100 };
    SDL_Color colorBorder = { 255,255,255,255 };

    StateAnimator animator;
    AnimState ANIM_CLOSED = { 0,	0.2f,	0,	0 };
    AnimState ANIM_OPENED = { 180,	1.0f,	0,	0 };

public:
    static const string Name;
    string GetName() override { return Name; }

    LobbyScene(GameContext* ctx) : IScene(ctx) {
        this->animator.setSmooth(true);
        this->animator.SetState(this->ANIM_CLOSED);
        this->Context->OnSceneShow.AddListener([this](string newScene) {
            if (newScene != LobbyScene::Name) {
                this->isHiding = true;
                this->animator.BlendTo(this->ANIM_CLOSED, 1.0f, [this]() {
                    this->Active = false;
                    });
            }
        });
    }

    void Activate() override {
        this->Active = true;
        this->isHiding = false;

        this->animator.BlendTo(this->ANIM_OPENED, 2.0f);

        for (int i = 0; i < 5; i++) {
            char buf[3];
            sprintf_s(buf, "%d", 5 - i);
            auto task = make_unique<TaskCounter>(this->Context, buf, 1.0f, i);
            this->Context->taskManager->AddTask(move(task));
        }
        auto task = make_unique<TaskCounter>(this->Context, "Start!", 1.0f, 5);
        this->Context->taskManager->AddTask(move(task));
    }

    void HandleEvent(const SDL_Event& event) override {
        if (this->isHiding) return;

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
        AnimState state = this->animator.GetCurrent();

        // Oblicz oryginalny rozmiar (bez scale)
        float originalW = this->Context->windowWidth - PADDING_X * 2;
        float originalH = this->Context->windowHeight - PADDING_TOP - PADDING_BOT;

        // Centrum oryginału
        float centerX = PADDING_X + originalW / 2.0f;
        float centerY = PADDING_TOP + originalH / 2.0f;

        // Przeskalowany rozmiar
        float scaledW = originalW * state.scale;
        float scaledH = originalH * state.scale;

        // Pozycja tak, aby centrum się zgadzało
        SDL_FRect rect = {
            centerX - scaledW / 2.0f,  // od środka w lewo
            centerY - scaledH / 2.0f,  // od środka w górę
            scaledW,
            scaledH
        };
        //SDL_FRect rect = {
            //PADDING_X * state.scale, PADDING_TOP* state.scale,
            //(this->Context->windowWidth - PADDING_X * 2) * state.scale,
            //(this->Context->windowHeight - PADDING_TOP - PADDING_BOT) * state.scale
        //};

        UTILS::setColor(this->Context->renderer, colorBg, state.alpha);
        SDL_RenderFillRect(this->Context->renderer, &rect);
        UTILS::setColor(this->Context->renderer, colorBorder, state.alpha);
        SDL_RenderRect(this->Context->renderer, &rect);

    }
};

const string LobbyScene::Name = "Lobby";
