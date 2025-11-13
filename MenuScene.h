#pragma once
#include <string>
#include "IScene.h"

class MenuScene : public IScene {
    float time = 0.0f;

public:
    static const string Name;

    string GetName() override { return Name; }

    MenuScene(GameContext* ctx) : IScene(ctx) {}

    void Render() override {
    }
};

const string MenuScene::Name = "Menu";
