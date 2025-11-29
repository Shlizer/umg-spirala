#pragma once
#include <vector>
#include <memory>
#include "SelectionGroup.h"

using namespace std;

class SelectionManager {
private:
    vector<SelectionGroup*> groups;
    SelectionGroup* activeGroup = nullptr;
    bool keyboardMode = true;

public:
    void RegisterGroup(SelectionGroup* group) {
        this->groups.push_back(group);

        if (!this->activeGroup && !group->IsEmpty()) {
            this->SetActiveGroup(group);
        }
    }

    void UnregisterGroup(SelectionGroup* group) {
        auto it = find(this->groups.begin(), this->groups.end(), group);
        if (it != this->groups.end()) {
            if (this->activeGroup == group) {
                this->activeGroup = nullptr;
            }
            this->groups.erase(it);
        }
    }

    void SetActiveGroup(SelectionGroup* group) {
        this->activeGroup = group;
        if (this->activeGroup && this->keyboardMode) {
            this->activeGroup->SelectFirst();
        }
    }

    void HandleKeyboard(const SDL_Event& event) {
        if (!this->activeGroup) return;

        if (event.type == SDL_EVENT_KEY_DOWN) {
            this->keyboardMode = true;

            switch (event.key.key) {
            case SDLK_UP:
                this->activeGroup->NavigateUp();
                break;
            case SDLK_DOWN:
                this->activeGroup->NavigateDown();
                break;
            case SDLK_LEFT:
                this->activeGroup->NavigateLeft();
                break;
            case SDLK_RIGHT:
                this->activeGroup->NavigateRight();
                break;
            case SDLK_TAB:
                if (event.key.mod & SDL_KMOD_SHIFT) {
                    this->activeGroup->NavigatePrev();
                }
                else {
                    this->activeGroup->NavigateNext();
                }
                break;
            case SDLK_RETURN:
            case SDLK_SPACE:
                this->activeGroup->ClickSelected();
                break;
            }
        }
    }

    void HandleMouse(float mx, float my, bool clicked) {
        if (!this->activeGroup) return;

        this->activeGroup->HandleMouse(mx, my);

        if (clicked) {
            this->keyboardMode = false;
            Selectable* hovered = this->activeGroup->GetHovered();
            if (hovered) {
                this->activeGroup->SelectElement(hovered);
                hovered->Click();
            }
        }
    }

    bool IsKeyboardMode() const { return this->keyboardMode; }
};