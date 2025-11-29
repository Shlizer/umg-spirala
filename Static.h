#pragma once
#include <SDL3/SDL_keycode.h>
#include "StateAnimator.h"
#include "Utils.h"

using namespace UTILS;

namespace CONFIG {
	const double PI = 3.14159265358979323846;

	const char* TITLE = "Spirala"; // + 

	/*****************************************
	 *  K E Y C O D E S
	 */
	const SDL_Keycode KEY_TOGGLE_FPS = SDLK_F1; // +
	const SDL_Keycode KEY_RESTART = SDLK_RETURN; // +
	const SDL_Keycode KEY_EXIT = SDLK_ESCAPE; // +
	const SDL_Scancode KEY_PLAYER_1_LEFT = SDL_SCANCODE_LEFT; // +
	const SDL_Scancode KEY_PLAYER_1_RIGHT = SDL_SCANCODE_RIGHT; // +
	const SDL_Scancode KEY_PLAYER_2_LEFT = SDL_SCANCODE_A; // +
	const SDL_Scancode KEY_PLAYER_2_RIGHT = SDL_SCANCODE_D; // +

	/*****************************************
	 *  F O N T   F A C E S
	 */
	const char* FONT_FACE_STYLED = "assets/fonts/Audiowide/Audiowide-Regular.ttf";
	const char* FONT_FACE_LATO_BLACK = "assets/fonts/Lato/Lato-Black.ttf";
	const char* FONT_FACE_LATO_BLACK_I = "assets/fonts/Lato/Lato-BlackItalic.ttf";
	const char* FONT_FACE_LATO_BOLD = "assets/fonts/Lato/Lato-Bold.ttf";
	const char* FONT_FACE_LATO_BOLD_I = "assets/fonts/Lato/Lato-BoldItalic.ttf";
	const char* FONT_FACE_LATO_REGULAR = "assets/fonts/Lato/Lato-Regular.ttf"; // +
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
	const int FONT_SIZE_DEBUG_FPS = 16; // +

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
	//									alpha	scale						x	y
	AnimState ANIM_LOGO_INIT = {		0,		FONT_SCALE_LOGO_INIT,		0,	300 };
	AnimState ANIM_LOGO_MENU_UP = {		255,	FONT_SCALE_LOGO_MENU_UP,	0,	100 };
	AnimState ANIM_LOGO_MENU_DOWN = {	155,	FONT_SCALE_LOGO_MENU_DOWN,	0,	150 };
	AnimState ANIM_LOGO_GAMEPLAY = {	155,	FONT_SCALE_LOGO_GAMEPLAY,	0,	20 };
	AnimState ANIM_COUNTDOWN_START = {	220,	0.8f,						0,	-100 };
	AnimState ANIM_COUNTDOWN_END = {	0,		4.0f,						0,	0 };

	/*****************************************
	 *  T I M E
	 */
	float TIME_BACKGROUND_CHANGE_SPEED = 5; // smaller = faster // +
	float TIME_LOGO_SHOW = 1.2f;
	float TIME_LOGO_MENU_YOYO = 1.5f;
	float TIME_LOGO_SWITCH_MENU = 0.8f;
	float TIME_LOGO_SWITCH_GAMEPLAY = 0.8f;

	/*****************************************
	 *  C O L O R S
	 */
	Uint8 COLOR_BACKGROUND_R_ADD = 20; // +
	Uint8 COLOR_BACKGROUND_R_MULT = 30; // +
	Uint8 COLOR_BACKGROUND_G_ADD = 15; // +
	Uint8 COLOR_BACKGROUND_G_MULT = 40; // +
	Uint8 COLOR_BACKGROUND_B_ADD = 40; // +
	Uint8 COLOR_BACKGROUND_B_MULT = 80; // +

	float COLOR_BACKGROUND_R_OFFSET_SIN = 1.2f; // +
	float COLOR_BACKGROUND_G_OFFSET_SIN = 1.5f; // +
	float COLOR_BACKGROUND_B_OFFSET_SIN = 1.8f; // +
	float COLOR_BACKGROUND_R_OFFSET = 10.0f; // +
	float COLOR_BACKGROUND_G_OFFSET = 15.0f; // +
	float COLOR_BACKGROUND_B_OFFSET = 20.0f; // +

	const Color COLOR_PLAYER_1 = Color({ 255, 255, 0, 255 }); // +
	const Color COLOR_PLAYER_2 = Color({ 255, 0, 255, 255 }); // +

	/*****************************************
	 *  S C E N E   -   G A M E P L A Y
	 */
	float SCENE_GAMEPLAY_PADDING_L = 70;
	float SCENE_GAMEPLAY_PADDING_R = 70;
	float SCENE_GAMEPLAY_PADDING_T = 90;
	float SCENE_GAMEPLAY_PADDING_B = 50;

	float SCENE_GAMEPLAY_SPAWN_DISTANCE_EDGE = 100; // minimal distance from edge where player can spawn
	float SCENE_GAMEPLAY_SPAWN_DISTANCE_PLAYER = 100; // minimal distance from other players where player can spawn
}