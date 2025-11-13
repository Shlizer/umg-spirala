#pragma once
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_render.h>
#include <string>
#include "Static.h"
#include "GameContext.h"

class IScene {
protected:
    GameContext* Context;

    void setColor(SDL_Color c, float alpha = NULL) {
        SDL_SetRenderDrawBlendMode(this->Context->renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(this->Context->renderer, c.r, c.g, c.b, alpha != NULL ? alpha : c.a);
    }
public:
    bool Active = false;

    IScene(GameContext* Context) {
        this->Context = Context;
    }
    ~IScene() = default;
    virtual string GetName() = 0;

    virtual void Activate() {
        this->Active = true;
    };
    virtual void HandleEvent(const SDL_Event& e) {};
    virtual void Update(float deltaTime) {};
    virtual void Render() {};
};
