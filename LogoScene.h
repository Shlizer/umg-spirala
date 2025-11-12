#pragma once
#include <string>
#include "IScene.h"

class LogoScene : public IScene {
	const float FONT_SCALE_INIT = .6f;
	const float FONT_SIZE = 70;

	TTF_Font* font;
	SDL_Surface* textSurface;
	SDL_Texture* textTexture;
	SDL_Color color = { 255,255,255,255 };

	float scale = FONT_SCALE_INIT;
	int alpha = 0;
	int positionY;

public:
	static const string Name;

	string GetName() override { return Name; }

	LogoScene(GameContext* ctx) : IScene(ctx) {
		font = TTF_OpenFont("assets/fonts/Audiowide/Audiowide-Regular.ttf", FONT_SIZE);
		if (!font) {
			SDL_Log("Failed to load font!");
		}
		textSurface = TTF_RenderText_Blended(font, "Spirala", 0, color);
		textTexture = SDL_CreateTextureFromSurface(Context->renderer, textSurface);
	}

	~LogoScene() {
		SDL_DestroySurface(textSurface);
		SDL_DestroyTexture(textTexture);
		TTF_CloseFont(font);
	}

	void Render() override {
		if (!textTexture) return;
		float w, h;
		SDL_GetTextureSize(textTexture, &w, &h);
		SDL_FRect rect = {
			(Context->windowWidth - w) / 2.0f,
			(Context->windowHeight - h) / 2.0f,
			w,
			h
		};
		SDL_RenderTexture(Context->renderer, textTexture, NULL, &rect);
	}
};

const string LogoScene::Name = "Logo";
