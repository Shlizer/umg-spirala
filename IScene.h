#pragma once
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_render.h>
#include <string>
#include "GameContext.h"

class IScene {
protected:
    GameContext* Context;

public:
    bool Active = false;

    IScene(GameContext* Context) : Context(Context) {}
    virtual ~IScene() = default;

    virtual string GetName() = 0;
    virtual void HandleEvent(const SDL_Event& e) {};
    virtual void Update(float deltaTime) {};
    virtual void Render() {};

    virtual void Activate() {
        this->Active = true;
    };
    virtual void Deactivate() {
        this->Active = false;
    };
};
