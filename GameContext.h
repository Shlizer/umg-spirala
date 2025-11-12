#pragma once

#include <SDL3/SDL.h>
#include <string>
#include "IEvent.h"
#include "Static.h"
#include "TaskManager.h"

class GameContext {
public:
    SDL_Renderer* renderer = nullptr;
    SDL_Window* window = nullptr;
    bool isRunning = true;
    int windowWidth = 1280;
    int windowHeight = 720;
    bool fullscreen = false;

    int targetFPS = 60;

    unique_ptr<TaskManager> taskManager = make_unique<TaskManager>();

    IEvent<string> OnSceneShow;
};
