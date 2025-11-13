#pragma once
#include <string>
#include "IScene.h"
#include "Static.h"

class BackgroundScene : public IScene {
	float time = 0.0f;

public:
	static const string Name;

	string GetName() override { return this->Name; }

	BackgroundScene(GameContext* ctx) : IScene(ctx) {}

	void Update(float deltaTime) override {
        this->time += deltaTime / 5;
	}

    void Render() override {
        float wave = (sin(this->time * 1.5f) + 1.0f) * 0.5f;

        float rOffset = sin(this->time * 1.2f) * 10.0f;
        float gOffset = sin(this->time * 1.5f) * 15.0f;
        float bOffset = sin(this->time * 1.8f) * 20.0f;

        for (int y = 0; y < this->Context->windowHeight; y++) {
            float t = static_cast<float>(y) / static_cast<float>(this->Context->windowHeight);

            Color bgColor = CONFIG::COLOR_BACKGROUND(t);
            bgColor.r += rOffset;
            bgColor.g += gOffset;
            bgColor.b += bOffset;
            bgColor.Set(this->Context->renderer);

            SDL_RenderLine(this->Context->renderer, 0, y, this->Context->windowWidth, y);
        }
    }
};

const string BackgroundScene::Name = "Background";
