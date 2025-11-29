#pragma once
#include <SDL3/SDL_render.h>
#include "IScene.h"
#include "Static.h"
#include "Utils.h"

using namespace std;
using namespace CONFIG;
using namespace UTILS;

class BackgroundScene : public IScene {
    float time = 0.0f;

private:
    Color GetColorGradient(float y) {
        Uint8 r = static_cast<Uint8>(COLOR_BACKGROUND_R_ADD + y * COLOR_BACKGROUND_R_MULT);
        Uint8 g = static_cast<Uint8>(COLOR_BACKGROUND_G_ADD + y * COLOR_BACKGROUND_G_MULT);
        Uint8 b = static_cast<Uint8>(COLOR_BACKGROUND_B_ADD + y * COLOR_BACKGROUND_B_MULT);
        return Color{ r, g, b, 255 };
    };

public:
    static const string Name;
    string GetName() override { return Name; }

    BackgroundScene(GameContext* ctx) : IScene(ctx) {}

    void Update(float deltaTime) override {
        this->time += deltaTime / TIME_BACKGROUND_CHANGE_SPEED;
    }

    void Render() override {
        float rOffset = sin(this->time * COLOR_BACKGROUND_R_OFFSET_SIN) * COLOR_BACKGROUND_R_OFFSET;
        float gOffset = sin(this->time * COLOR_BACKGROUND_G_OFFSET_SIN) * COLOR_BACKGROUND_G_OFFSET;
        float bOffset = sin(this->time * COLOR_BACKGROUND_B_OFFSET_SIN) * COLOR_BACKGROUND_B_OFFSET;

        for (float y = 0; y < this->Context->windowHeight; y++) {
            float yPos = static_cast<float>(y) / static_cast<float>(this->Context->windowHeight);

            UTILS::Color bgColor = this->GetColorGradient(yPos);
            int r = (int)bgColor.r + (int)rOffset;
            int g = (int)bgColor.g + (int)gOffset;
            int b = (int)bgColor.b + (int)bOffset;
            bgColor.r = UTILS::clamp(r, 0, 255);
            bgColor.g = UTILS::clamp(g, 0, 255);
            bgColor.b = UTILS::clamp(b, 0, 255);
            bgColor.Set(this->Context->renderer);

            SDL_RenderLine(this->Context->renderer, 0, y, static_cast<float>(this->Context->windowWidth), y);
        }
    }
};

const string BackgroundScene::Name = "Background";
