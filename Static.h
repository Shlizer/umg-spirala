#pragma once
#include "StateAnimator.h"

const double PI = 3.14159265358979323846;

struct Color {
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

namespace CONFIG {
	const char* TITLE = "Spirala";

	/*****************************************
	 *  F O N T   F A C E S
	 */
	const char* FONT_FACE_STYLED = "assets/fonts/Audiowide/Audiowide-Regular.ttf";
	const char* FONT_FACE_LATO_BLACK = "assets/fonts/Lato/Lato-Black.ttf";
	const char* FONT_FACE_LATO_BLACK_I = "assets/fonts/Lato/Lato-BlackItalic.ttf";
	const char* FONT_FACE_LATO_BOLD = "assets/fonts/Lato/Lato-Bold.ttf";
	const char* FONT_FACE_LATO_BOLD_I = "assets/fonts/Lato/Lato-BoldItalic.ttf";
	const char* FONT_FACE_LATO_REGULAR = "assets/fonts/Lato/Lato-Regular.ttf";
	const char* FONT_FACE_LATO_REGULAR_I = "assets/fonts/Lato/Lato-Italic.ttf";
	const char* FONT_FACE_LATO_LIGHT = "assets/fonts/Lato/Lato-Light.ttf";
	const char* FONT_FACE_LATO_LIGHT_I = "assets/fonts/Lato/Lato-LightItalic.ttf";
	const char* FONT_FACE_LATO_THIN = "assets/fonts/Lato/Lato-Thin.ttf";
	const char* FONT_FACE_LATO_THIN_I = "assets/fonts/Lato/Lato-ThinItalic.ttf";

	/*****************************************
	 *  F O N T   S I Z E S
	 */
	const int FONT_SIZE_LOGO = 90;
	const int FONT_SIZE_LOGO_MENU = 0;
	const int FONT_SIZE_LOGO_MENU_DOWN = 0;
	const int FONT_SIZE_LOGO_INIT = 0;
	const int FONT_SIZE_DEBUG_FPS = 16;

	/*****************************************
	 *  F O N T   S C A L E S
	 */
	const float FONT_SCALE_LOGO_INIT = 0.4f;
	const float FONT_SCALE_LOGO_MENU_UP = 1.0f;
	const float FONT_SCALE_LOGO_MENU_DOWN = 0.9f;
	const float FONT_SCALE_LOGO_GAMEPLAY = 0.4f;

	/*****************************************
	 *  A N I M A T I O N   S T A T E S
	 */
	//								alpha	scale						x	y
	AnimState LOGO_INIT = {			0,		FONT_SCALE_LOGO_INIT,		0,	300 };
	AnimState LOGO_MENU_UP = {		255,	FONT_SCALE_LOGO_MENU_UP,	0,	100 };
	AnimState LOGO_MENU_DOWN = {	155,	FONT_SCALE_LOGO_MENU_DOWN,	0,	150 };
	AnimState LOGO_GAMEPLAY = {		155,	FONT_SCALE_LOGO_GAMEPLAY,	0,	20 };

	/*****************************************
	 *  T I M E
	 */
	float TIME_LOGO_SHOW = 1.2f;
	float TIME_LOGO_MENU_YOYO = 1.5f;
	float TIME_LOGO_SWITCH_MENU = 0.8f;
	float TIME_LOGO_SWITCH_GAMEPLAY = 0.8f;

	/*****************************************
	 *  C O L O R S
	 */
	Color COLOR_BACKGROUND(float t) {
		Uint8 r = static_cast<Uint8>(20 + t * 30);
		Uint8 g = static_cast<Uint8>(15 + t * 40);
		Uint8 b = static_cast<Uint8>(40 + t * 80);
		return Color{ r, g, b, 255 };
	};
	Color COLOR_LOGO = Color{ 255,255,255,255 };
}