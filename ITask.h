#pragma once
#include <SDL3/SDL_timer.h>

class ITask {
protected:
    float delayRemaining = 0.0f;
    virtual bool UpdateDelayed(float deltaTime) = 0;

public:
    ITask(float delaySeconds = 0.0f) : delayRemaining(delaySeconds) {}
    virtual ~ITask() = default;
    virtual bool Update(float deltaTime) {
        if (delayRemaining > 0.0f) {
            delayRemaining -= deltaTime;
            return false;
        }
        return UpdateDelayed(deltaTime);
    };
    virtual void Render() = 0;
};
