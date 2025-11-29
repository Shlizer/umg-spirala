#pragma once
#include <vector>
#include <memory>
#include <cmath>
#include "Selectable.h"

using namespace std;

class SelectionGroup {
private:
    vector<Selectable*> elements;
    int selectedIndex = -1;

    // Znajdź najbliższy element w kierunku (spatial navigation)
    int FindNearest(int fromIndex, float dx, float dy) {
        if (fromIndex < 0 || fromIndex >= this->elements.size()) return -1;

        SDL_FRect from = this->elements[fromIndex]->GetRect();
        float fromCenterX = from.x + from.w / 2.0f;
        float fromCenterY = from.y + from.h / 2.0f;

        int bestIndex = -1;
        float bestScore = 1e9f;

        for (int i = 0; i < this->elements.size(); i++) {
            if (i == fromIndex) continue;

            SDL_FRect to = this->elements[i]->GetRect();
            float toCenterX = to.x + to.w / 2.0f;
            float toCenterY = to.y + to.h / 2.0f;

            float diffX = toCenterX - fromCenterX;
            float diffY = toCenterY - fromCenterY;

            bool inDirection = false;
            if (dx > 0 && diffX > 0) inDirection = true;  // RIGHT
            if (dx < 0 && diffX < 0) inDirection = true;  // LEFT
            if (dy > 0 && diffY > 0) inDirection = true;  // DOWN
            if (dy < 0 && diffY < 0) inDirection = true;  // UP

            if (!inDirection) continue;

            float distance = sqrt(diffX * diffX + diffY * diffY);
            float alignment = 0;

            if (dx != 0) {
                alignment = abs(diffY);
            }
            else {
                alignment = abs(diffX);
            }

            float score = distance + alignment * 0.5f;

            if (score < bestScore) {
                bestScore = score;
                bestIndex = i;
            }
        }

        return bestIndex;
    }

public:
    void Add(Selectable* element) {
        this->elements.push_back(element);
    }

    void Remove(Selectable* element) {
        auto it = find(this->elements.begin(), this->elements.end(), element);
        if (it != this->elements.end()) {
            if ((*it)->IsSelected()) {
                (*it)->SetSelected(false);
            }
            this->elements.erase(it);
            if (this->selectedIndex >= this->elements.size()) {
                this->selectedIndex = (int)this->elements.size() - 1;
            }
        }
    }

    void Clear() {
        for (auto elem : this->elements) {
            elem->SetSelected(false);
        }
        this->elements.clear();
        this->selectedIndex = -1;
    }

    void SelectFirst() {
        if (!this->elements.empty()) {
            this->Select(0);
        }
    }

    void Select(int index) {
        if (index < 0 || index >= this->elements.size()) return;

        if (this->selectedIndex >= 0 && this->selectedIndex < this->elements.size()) {
            this->elements[selectedIndex]->SetSelected(false);
        }

        this->selectedIndex = index;
        this->elements[selectedIndex]->SetSelected(true);
    }

    void SelectElement(Selectable* element) {
        for (int i = 0; i < this->elements.size(); i++) {
            if (this->elements[i] == element) {
                this->Select(i);
                return;
            }
        }
    }

    void NavigateUp() {
        if (this->selectedIndex < 0) { this->SelectFirst(); return; }
        int next = this->FindNearest(this->selectedIndex, 0, -1);
        if (next >= 0) this->Select(next);
    }

    void NavigateDown() {
        if (this->selectedIndex < 0) { this->SelectFirst(); return; }
        int next = this->FindNearest(this->selectedIndex, 0, 1);
        if (next >= 0) this->Select(next);
    }

    void NavigateLeft() {
        if (this->selectedIndex < 0) { this->SelectFirst(); return; }
        int next = this->FindNearest(this->selectedIndex, -1, 0);
        if (next >= 0) this->Select(next);
    }

    void NavigateRight() {
        if (this->selectedIndex < 0) { this->SelectFirst(); return; }
        int next = this->FindNearest(this->selectedIndex, 1, 0);
        if (next >= 0) this->Select(next);
    }

    void NavigateNext() {
        if (this->elements.empty()) return;
        if (this->selectedIndex < 0) {
            this->Select(0);
        }
        else {
            this->Select((this->selectedIndex + 1) % this->elements.size());
        }
    }

    void NavigatePrev() {
        if (this->elements.empty()) return;
        if (this->selectedIndex < 0) {
            this->Select((int)this->elements.size() - 1);
        }
        else {
            this->Select((this->selectedIndex - 1 + (int)this->elements.size()) % (int)this->elements.size());
        }
    }

    void ClickSelected() {
        if (this->selectedIndex >= 0 && this->selectedIndex < this->elements.size()) {
            this->elements[this->selectedIndex]->Click();
        }
    }

    void HandleMouse(float mx, float my) {
        for (auto elem : this->elements) {
            elem->HandleMouse(mx, my);
        }
    }

    Selectable* GetHovered() {
        for (auto elem : this->elements) {
            if (elem->IsHovered()) return elem;
        }
        return nullptr;
    }

    bool IsEmpty() const { return this->elements.empty(); }
};
