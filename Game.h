#pragma once
#include "GameContext.h"
#include "IScene.h"
#include "BackgroundScene.h"
#include "GameplayScene.h"
#include "KillLogScene.h"
#include "FPSCounterScene.h"

class Game {
    vector<unique_ptr<IScene>> scenes;

private:
    IScene* FindScene(const string& name) {
        for (auto& scene : this->scenes) {
            if (scene->GetName() == name) return scene.get();
        }
        return nullptr;
    }

    void HandleInput() {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                this->Context->isRunning = false;
            }
            for (auto& scene : this->scenes) {
                if (scene->Active) {
                    scene->HandleEvent(event);
                }
            }
        }
    }

    void Update(float deltaTime) {
        for (auto& scene : this->scenes) {
            if (scene->Active) {
                scene->Update(deltaTime);
            }
        }
        this->Context->taskManager->Update(deltaTime);
    }

    void Render() {
        SDL_SetRenderDrawColor(this->Context->renderer, 0, 0, 0, 255);
        SDL_RenderClear(this->Context->renderer);

        for (auto& scene : this->scenes) {
            if (scene->Active) {
                scene->Render();
            }
        }

        this->Context->taskManager->Render();
        SDL_RenderPresent(this->Context->renderer);
    }

public:
    Uint64 lastFrameTime = 0;
    GameContext* Context;

    Game(GameContext* gameContext) {
        this->Context = gameContext;

        this->scenes.push_back(make_unique<BackgroundScene>(this->Context));
        this->scenes.push_back(make_unique<GameplayScene>(this->Context));
        this->scenes.push_back(make_unique<KillLogScene>(this->Context));
        this->scenes.push_back(make_unique<FPSCounterScene>(this->Context));

        this->FindScene(BackgroundScene::Name)->Activate();
        this->FindScene(GameplayScene::Name)->Activate();
        this->FindScene(KillLogScene::Name)->Activate();
        this->FindScene(FPSCounterScene::Name)->Activate();
    }

    void Run() {
        lastFrameTime = SDL_GetTicks();

        while (Context->isRunning) {
            Uint64 frameStart = SDL_GetTicks();
            float deltaTime = (frameStart - lastFrameTime) / 1000.0f;
            lastFrameTime = frameStart;

            this->HandleInput();
            this->Update(deltaTime);
            this->Render();
        }
    }
};
