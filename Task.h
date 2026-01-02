#pragma once
#include <algorithm>
#include <SDL3_ttf/SDL_ttf.h>
#include <string>
#include <functional>
#include "ITask.h"
#include "GameContext.h"
#include "Utils.h"

typedef function<void(void)> tVoidFunction;
typedef function<void(float)> tFloatFunction;

class Task : public ITask {
    tFloatFunction onUpdateCallback = nullptr;
    tVoidFunction onCompleteCallback = nullptr;

public:
    Task(float delay = 0, tFloatFunction OnUpdate = nullptr, tVoidFunction OnComplete = nullptr) : ITask(delay) {
        onUpdateCallback = OnUpdate;
        onCompleteCallback = OnComplete;
    }

    bool UpdateDelayed(float deltaTime) override {
        if (onCompleteCallback != nullptr)
            onCompleteCallback();
        return true;
    }

    void Complete() override {
        if (onCompleteCallback != nullptr)
            onCompleteCallback();
    }

    void Render() override {
    }
};
