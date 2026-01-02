#pragma once
#include <SDL3/SDL_keycode.h>
#include "StateAnimator.h"
#include "Utils.h"

using namespace std;
using namespace UTILS;

struct PlayerInfo {
	Color color;
	SDL_Scancode keyLeft, keyRight;
};

namespace CONFIG {
	const double PI = 3.14159265358979323846;

	const char* TITLE = "Spirala";
	const int WIDTH = 1280;
	const int HEIGHT = 720;

	/*****************************************
	 *  P L A Y E R S
	 */
	PlayerInfo PLAYERS[2] = {
		{ Color({ 255, 255, 0, 255 }), SDL_SCANCODE_LEFT, SDL_SCANCODE_RIGHT},
		{ Color({ 255, 0, 255, 255 }), SDL_SCANCODE_A, SDL_SCANCODE_S},
		//{ Color({ 0, 255, 255, 255 }), SDL_SCANCODE_N, SDL_SCANCODE_M},
	};

	int PLAYER_COUNT = (sizeof(PLAYERS) / sizeof(*PLAYERS));

	/*****************************************
	 *  K E Y C O D E S
	 */
	const SDL_Keycode KEY_TOGGLE_FPS = SDLK_F1;
	const SDL_Keycode KEY_RESTART = SDLK_RETURN;
	const SDL_Keycode KEY_EXIT = SDLK_ESCAPE;

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
	const int FONT_SIZE_DEBUG_FPS = 16;
	const int FONT_SIZE_COUNTER = 120;

	/*****************************************
	 *  C O U N T E R
	 */
	const Color COUNTER_COLOR = Color({ 255, 255, 255, 255 });
	const int COUNTER_SIZE = 3;
	const float COUNTER_TIME = 1.0f;

	/*****************************************
	 *  P L A Y E R
	 */
	const float PLAYER_SPEED = 0.14f;
	const float PLAYER_TURN_SPEED = 0.005f;
	const int PLAYER_THICKNESS = 1;
	
	const float TRAIL_GAP_INTERVAL = 2.0f;
	const float TRAIL_GAP_DURATION = 0.1f;

	/*****************************************
	 *  A N I M A T I O N   S T A T E S
	 *///								alpha	scale	x	y  */
	AnimState ANIM_COUNTDOWN_START = {	220,	0.8f,	0,	-100 };
	AnimState ANIM_COUNTDOWN_END = {	0,		4.0f,	0,	0 };
	AnimState ANIM_CLOSED = {			0,		0.2f,	0,	0 };
	AnimState ANIM_OPENED = {			180,	1.0f,	0,	0 };


	/*****************************************
	 *  T I M E
	 */
	float TIME_BACKGROUND_CHANGE_SPEED = 5; // smaller = faster

	/*****************************************
	 *  C O L O R S
	 */
	Uint8 COLOR_BACKGROUND_R_ADD = 20;
	Uint8 COLOR_BACKGROUND_R_MULT = 30;
	Uint8 COLOR_BACKGROUND_G_ADD = 15;
	Uint8 COLOR_BACKGROUND_G_MULT = 40;
	Uint8 COLOR_BACKGROUND_B_ADD = 40;
	Uint8 COLOR_BACKGROUND_B_MULT = 80;

	float COLOR_BACKGROUND_R_OFFSET_SIN = 1.2f;
	float COLOR_BACKGROUND_G_OFFSET_SIN = 1.5f;
	float COLOR_BACKGROUND_B_OFFSET_SIN = 1.8f;
	float COLOR_BACKGROUND_R_OFFSET = 10.0f;
	float COLOR_BACKGROUND_G_OFFSET = 15.0f;
	float COLOR_BACKGROUND_B_OFFSET = 20.0f;

	/*****************************************
	 *  S C E N E   -   G A M E P L A Y
	 */
	float SCENE_GAMEPLAY_PADDING_L = 70;
	float SCENE_GAMEPLAY_PADDING_R = 70;
	float SCENE_GAMEPLAY_PADDING_T = 90;
	float SCENE_GAMEPLAY_PADDING_B = 50;

	float SCENE_GAMEPLAY_SPAWN_DISTANCE_EDGE = 200; // minimal distance from edge where player can spawn
	float SCENE_GAMEPLAY_SPAWN_DISTANCE_PLAYER = 100; // minimal distance from other players where player can spawn
}