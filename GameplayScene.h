#pragma once
#include <string>
#include "IScene.h"
#include "MenuScene.h"

class GameplayScene : public IScene {
    bool isHiding = false;

public:
    static const string Name;
    string GetName() override { return Name; }

    void Activate() override {
        this->Active = true;
        this->isHiding = false;
    }

    GameplayScene(GameContext* ctx) : IScene(ctx) {
        this->Context->OnSceneShow.AddListener([this](string newScene) {
            if (newScene != GameplayScene::Name) {
                this->isHiding = true;
            }
            });
    }

    void HandleEvent(const SDL_Event& event) override {
        if (this->isHiding) return;

        if (event.type == SDL_EVENT_KEY_DOWN) {
            if (event.key.key == SDLK_ESCAPE) {
                this->Context->OnSceneShow.Invoke("Menu");
            }
        }
    }

    void Render() override {
    }
};

const string GameplayScene::Name = "Gameplay";
