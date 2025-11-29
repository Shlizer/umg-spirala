#pragma once
#include <SDL3_ttf/SDL_ttf.h>

namespace UTILS {
	struct Color { // +
		uint8_t r, g, b, a;

		void Set(SDL_Renderer* renderer) {
			SDL_SetRenderDrawColor(renderer, this->r, this->g, this->b, this->a);
		}

		SDL_Color toColor() const {
			return { r, g , b , a };
		}

		SDL_FColor toFColor() const {
			return { r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f };
		}
	};

	void setColor(SDL_Renderer* renderer, SDL_Color c, float alpha = -1.f) {
		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
		Uint8 a = (alpha >= 0.f ? static_cast<Uint8>(alpha) : c.a);
		SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, a);
	}

	TTF_Font* loadFont(const char* fontFace, float fontSize) { // +
		TTF_Font* font = TTF_OpenFont(fontFace, fontSize);
		if (!font) {
			SDL_Log("Failed to load font!");
			return nullptr;
		}
		return font;
	}

	template<typename T> // +
	T clamp(const T& v, const T& lo, const T& hi)
	{
		return (v < lo) ? lo : (v > hi) ? hi : v;
	}
}
