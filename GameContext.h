#pragma once

#include <SDL3/SDL.h>
#include <string>
#include "TaskManager.h"
#include "Config.h"

class GameContext {
public:
    SDL_Renderer* renderer = nullptr;
    SDL_Window* window = nullptr;
    bool isRunning = true;
    int windowWidth = WIDTH;
    int windowHeight = HEIGHT;
    bool fullscreen = false;

    int targetFPS = 60;

    unique_ptr<TaskManager> taskManager = make_unique<TaskManager>();
};
