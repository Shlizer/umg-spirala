#pragma once
#include <cmath>
#include <functional>

using namespace std;

struct AnimState {
    float alpha = 255.0f;
    float scale = 1.0f;
    float x = 0.0f;
    float y = 0.0f;

    AnimState() = default;
    AnimState(float alpha, float scale, float x, float y)
        : alpha(alpha), scale(scale), x(x), y(y) {
    }
};

enum class AnimMode {
    None,
    Blend,
    Yoyo
};

class StateAnimator {
private:
    AnimState current;
    AnimState stateTargetA;
    AnimState stateTargetB;

    AnimMode mode = AnimMode::None;
    float time = 0.0f;
    float duration = 1.0f;
    bool yoyoReverse = false;
    bool smoothstep = false;

    function<void()> onComplete = nullptr;

public:
    void SetState(AnimState state) {
        this->current = state;
        this->stateTargetA = state;
        this->mode = AnimMode::None;
        this->time = 0.0f;
    }

    void setSmooth(bool smoothstep) {
        this->smoothstep = smoothstep;
    }

    void BlendTo(AnimState target, float duration, function<void()> callback = nullptr) {
        this->stateTargetA = this->current;
        this->stateTargetB = target;
        this->duration = duration;
        this->time = 0.0f;
        this->mode = AnimMode::Blend;
        this->onComplete = callback;
    }

    void YoyoBetween(AnimState a, AnimState b, float duration) {
        this->stateTargetA = a;
        this->stateTargetB = b;
        this->current = a;
        this->duration = duration;
        this->time = 0.0f;
        this->mode = AnimMode::Yoyo;
        this->yoyoReverse = false;
        this->onComplete = nullptr;
    }

    void Update(float deltaTime) {
        if (this->mode == AnimMode::None) return;

        this->time += deltaTime;
        float t = this->time / this->duration;

        if (this->mode == AnimMode::Blend) {
            this->Blend(t);
        }
        else if (this->mode == AnimMode::Yoyo) {
            this->Yoyo(t);
        }
    }

    AnimState GetCurrent() {
        return this->current;
    }

    void Stop() {
        this->mode = AnimMode::None;
    }

private:
    void Blend(float t) {
        if (t >= 1.0f) {
            this->current = this->stateTargetB;
            this->mode = AnimMode::None;
            if (this->onComplete) this->onComplete();
        }
        else {
            this->current = this->Lerp(this->stateTargetA, this->stateTargetB, t);
        }
    }

    void Yoyo(float t) {
        if (t >= 1.0f) {
            this->yoyoReverse = !this->yoyoReverse;
            this->time = 0.0f;
        }

        float progress = this->time / this->duration;
        if (this->yoyoReverse) {
            this->current = this->Lerp(this->stateTargetB, this->stateTargetA, progress);
        }
        else {
            this->current = this->Lerp(this->stateTargetA, this->stateTargetB, progress);
        }
    }

    AnimState Lerp(AnimState from, AnimState to, float t) {
        if (smoothstep) t = t * t * (3 - 2 * t);
        return {
            from.alpha + (to.alpha - from.alpha) * t,
            from.scale + (to.scale - from.scale) * t,
            from.x + (to.x - from.x) * t,
            from.y + (to.y - from.y) * t
        };
    }
};