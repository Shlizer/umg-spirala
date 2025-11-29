#pragma once
#include <SDL3/SDL_render.h>
#include "IEvent.h"

using namespace std;

class Selectable {
protected:
    SDL_FRect rect;
    bool selected = false;
    bool hovered = false;

public:
    IEvent<> onClick;
    IEvent<bool> onSelected;
    IEvent<bool> onHover;

    Selectable(SDL_FRect rect) : rect(rect) {}
    virtual ~Selectable() = default;

    SDL_FRect GetRect() const { return this->rect; }

    void SetSelected(bool value) {
        if (this->selected != value) {
            this->selected = value;
            this->onSelected.Invoke(value);
        }
    }

    void SetHovered(bool value) {
        if (this->hovered != value) {
            this->hovered = value;
            this->onHover.Invoke(value);
        }
    }

    bool IsSelected() const { return this->selected; }
    bool IsHovered() const { return this->hovered; }

    void HandleMouse(float mx, float my) {
        bool wasHovered = this->hovered;
        this->hovered = (mx >= rect.x && mx <= rect.x + rect.w &&
            my >= rect.y && my <= rect.y + rect.h);
        if (this->hovered != wasHovered) {
            this->onHover.Invoke(this->hovered);
        }
    }

    virtual void Click() {
        this->onClick.Invoke();
    }
};