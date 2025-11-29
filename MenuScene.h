#pragma once
#include <string>
#include "IScene.h"
#include "GameplayScene.h"
#include "Button.h"
#include "Utils.h"

class MenuScene : public IScene {
    bool isHiding = false;
    float alpha = NULL;

    SelectionGroup group;
    Button* playButton;
    Button* optionsButton;
    Button* exitButton;
    TTF_Font* font;

    AnimState HIDDEN = { 0,		0,		0,	0 };
    AnimState VISIBLE = { 0,		0,		0,	0 };
    float DURATION = 0.1f;

    StateAnimator animator;
public:
    static const string Name;
    string GetName() override { return Name; }

    MenuScene(GameContext* ctx) : IScene(ctx) {
        font = UTILS::loadFont("assets/fonts/Lato/Lato-Regular.ttf", 16);

        playButton = new Button(ctx->renderer, "Graj", font, { 440, 250, 400, 60 });
        optionsButton = new Button(ctx->renderer, "Opcje", font, { 440, 320, 400, 60 });
        exitButton = new Button(ctx->renderer, "Wyjście", font, { 440, 410, 400, 60 });

        group.Add(playButton);
        group.Add(optionsButton);
        group.Add(exitButton);

        playButton->onClick.AddListener([this]() {
            this->Deactivate();
            this->Context->OnSceneShow.Invoke("Gameplay");
        });

        exitButton->onClick.AddListener([ctx]() {
            ctx->isRunning = false;
        });

        this->animator.setSmooth(true);
        this->animator.SetState(HIDDEN);
    }

    ~MenuScene() {
        delete playButton;
        delete optionsButton;
        delete exitButton;
        TTF_CloseFont(font);
    }

    void Activate() override {
        this->Active = true;
        this->isHiding = false;
        this->animator.BlendTo(VISIBLE, DURATION);
        this->Context->selectionManager->RegisterGroup(&this->group);
        this->Context->selectionManager->SetActiveGroup(&this->group);
    }

    void Deactivate() override {
        this->isHiding = true;
        this->animator.BlendTo(HIDDEN, DURATION, [this]() { this->Active = false; });
        this->Context->selectionManager->UnregisterGroup(&this->group);
    }

    void HandleEvent(const SDL_Event& event) override {
        if (this->isHiding) return;

        Context->selectionManager->HandleKeyboard(event);

        if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
            Context->selectionManager->HandleMouse(
                event.button.x,
                event.button.y,
                true
            );
        }
        // Mouse motion
        else if (event.type == SDL_EVENT_MOUSE_MOTION) {
            Context->selectionManager->HandleMouse(
                event.motion.x,
                event.motion.y,
                false
            );
        }
        //this->Context->OnSceneShow.Invoke("Lobby");
    }

    void Update(float deltaTime) override {
        this->animator.Update(deltaTime);
    }

    void SetAlpha(float alpha) {
        this->alpha = alpha;
    }

    void Render() override {
        if (this->Active) {
            AnimState state = this->animator.GetCurrent();

            if (this->alpha != NULL) {
                //playButton->
            }

            playButton->Render();
            optionsButton->Render();
            exitButton->Render();
        }
    }
};

const string MenuScene::Name = "Menu";
