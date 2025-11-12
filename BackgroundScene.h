#pragma once
#include <string>
#include "IScene.h"

class BackgroundScene : public IScene {
	float time = 0.0f;

public:
	static const string Name;

	string GetName() override { return Name; }

	BackgroundScene(GameContext* ctx) : IScene(ctx) {}

	void Update(float deltaTime) override {
		time += deltaTime / 5;
	}

    void Render() override {
        float wave = (sin(time * 1.5f) + 1.0f) * 0.5f;

        float rOffset = sin(time * 1.2f) * 10.0f;
        float gOffset = sin(time * 1.5f) * 15.0f;
        float bOffset = sin(time * 1.8f) * 20.0f;

        for (int y = 0; y < Context->windowHeight; y++) {
            float t = static_cast<float>(y) / static_cast<float>(Context->windowHeight);

            Uint8 r = static_cast<Uint8>(20 + t * 30 + rOffset);
            Uint8 g = static_cast<Uint8>(15 + t * 40 + gOffset);
            Uint8 b = static_cast<Uint8>(40 + t * 80 + bOffset);

            SDL_SetRenderDrawColor(Context->renderer, r, g, b, 255);
            SDL_RenderLine(Context->renderer, 0, y, Context->windowWidth, y);
        }
    }
};

const string BackgroundScene::Name = "Background";
