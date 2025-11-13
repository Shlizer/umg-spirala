#pragma once
#include "Static.h"
#include "GameContext.h"
#include "IScene.h"
#include "BackgroundScene.h"
#include "MenuScene.h"
#include "GameplayScene.h"
#include "LogoScene.h"
#include "FPSCounterScene.h"

class Game {
    vector<unique_ptr<IScene>> scenes;

private:
    IScene* FindScene(const string& name) {
        for (auto& scene : scenes) {
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
            for (auto& scene : scenes) {
                if (scene->Active) {
                    scene->HandleEvent(event);
                }
            }
        }
    }

    void Update(float deltaTime) {
        for (auto& scene : scenes) {
            if (scene->Active) {
                scene->Update(deltaTime);
            }
        }
        this->Context->taskManager->Update(deltaTime);
    }

    void Render() {
        SDL_SetRenderDrawColor(this->Context->renderer, 0, 0, 0, 255);
        SDL_RenderClear(this->Context->renderer);

        for (auto& scene : scenes) {
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

        Context->OnSceneShow.AddListener([this](string sceneName) {
            SDL_Log("Show scene: %s", sceneName.c_str());
            FindScene(sceneName)->Activate();
        });

        scenes.push_back(make_unique<BackgroundScene>(this->Context));
        scenes.push_back(make_unique<MenuScene>(this->Context));
        scenes.push_back(make_unique<GameplayScene>(this->Context));
        scenes.push_back(make_unique<LogoScene>(this->Context));
        scenes.push_back(make_unique<FPSCounterScene>(this->Context));

        FindScene(BackgroundScene::Name)->Activate();
        FindScene(LogoScene::Name)->Activate();
        FindScene(FPSCounterScene::Name)->Activate();
    }

    void Run() {
        lastFrameTime = SDL_GetTicks();

        while (Context->isRunning) {
            Uint64 frameStart = SDL_GetTicks();
            float deltaTime = (frameStart - lastFrameTime) / 1000.0f;
            lastFrameTime = frameStart;

            HandleInput();
            Update(deltaTime);
            Render();
        }
    }
};
