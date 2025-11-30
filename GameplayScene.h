#pragma once
#include <string>
#include "TaskCounter.h"
#include "IScene.h"
#include "Utils.h"
#include "Player.h"
#include "Task.h"
#include "Config.h"

using namespace std;
using namespace CONFIG;

class GameplayScene : public IScene {
    bool isPlaying = false;
    SDL_Color colorBg = { 0,0,0,100 };
    SDL_Color colorBorder = { 255,255,255,255 };

    StateAnimator animator;

    std::vector<Player*> players;

    void SetupPlayers() {
        players.clear();

        Player* p1 = new Player(
            this->Context,
            COLOR_PLAYER_1,
            KEY_PLAYER_1_LEFT,
            KEY_PLAYER_1_RIGHT
        );
        Player* p2 = new Player(
            this->Context,
            COLOR_PLAYER_2,
            KEY_PLAYER_2_LEFT,
            KEY_PLAYER_2_RIGHT
        );

        p1->SetRandomPosition(SCENE_GAMEPLAY_SPAWN_DISTANCE_EDGE,
            SCENE_GAMEPLAY_SPAWN_DISTANCE_PLAYER,
            players);
        players.push_back(p1);

        p2->SetRandomPosition(SCENE_GAMEPLAY_SPAWN_DISTANCE_EDGE,
            SCENE_GAMEPLAY_SPAWN_DISTANCE_PLAYER,
            players);
        players.push_back(p2);
    }

    void Restart() {
        this->isPlaying = false;
        this->SetupPlayers();

        for (int i = 0; i < COUNTER_SIZE; i++) {
            char buf[3];
            sprintf_s(buf, "%d", COUNTER_SIZE - i);
            auto task = make_unique<TaskCounter>(this->Context, buf, COUNTER_TIME, i);
            this->Context->taskManager->AddTask(move(task));
        }
        auto task = make_unique<TaskCounter>(this->Context, "Start!", COUNTER_TIME, COUNTER_SIZE * COUNTER_TIME);
        this->Context->taskManager->AddTask(move(task));

        auto finishTask = make_unique<Task>(COUNTER_SIZE, [this](float d) {},
            [this]() {
                this->isPlaying = true;
                for (auto* p : players) {
                    p->Start();
                }
            });
        this->Context->taskManager->AddTask(move(finishTask));
    }

public:
    static const string Name;
    string GetName() override { return Name; }

    GameplayScene(GameContext* ctx) : IScene(ctx) {
        this->animator.setSmooth(true);
        this->animator.SetState(ANIM_CLOSED);
    }

    void Activate() override {
        this->Active = true;

        this->animator.BlendTo(ANIM_OPENED, 2.0f);
        this->Restart();
    }

    void HandleEvent(const SDL_Event& event) override {
        if (event.type == SDL_EVENT_KEY_DOWN) {
            if (event.key.key == KEY_EXIT) {
                this->Context->isRunning = false;
            }
        }
        if (event.type == SDL_EVENT_KEY_DOWN) {
            if (event.key.key == KEY_RESTART) {
                if (this->isPlaying == true) {
                    this->Restart();
                }
            }
        }

    }

    void Update(float deltaTime) override {
        this->animator.Update(deltaTime);

        const bool* keystate = SDL_GetKeyboardState(nullptr);

        if (!isPlaying) return;

        for (auto* player : players) {
            if (player->IsAlive()) {
                player->HandleInput(keystate);
                player->Update(deltaTime);

                float minX = SCENE_GAMEPLAY_PADDING_L;
                float minY = SCENE_GAMEPLAY_PADDING_T;
                float maxX = this->Context->windowWidth - SCENE_GAMEPLAY_PADDING_R;
                float maxY = this->Context->windowHeight - SCENE_GAMEPLAY_PADDING_B;

                if (player->GetX() < minX || player->GetX() > maxX ||
                    player->GetY() < minY || player->GetY() > maxY) {
                    player->Kill();
                    continue;
                }

                bool collision = false;
                for (const auto* other : players) {
                    const auto& trail = other->GetTrail();
                    if (trail.size() < 2) continue;

                    size_t endIdx;
                    if (other == player) {
                        if (trail.size() <= 30) continue;
                        endIdx = trail.size() - 10;
                    }
                    else {
                        endIdx = trail.size();
                    }

                    for (size_t i = 0; i < endIdx; i++) {
                        float dx = trail[i].x - player->GetX();
                        float dy = trail[i].y - player->GetY();
                        float dist = sqrtf(dx * dx + dy * dy);

                        if (dist < other->GetLineThickness()) {
                            collision = true;
                            break;
                        }
                    }
                    if (collision) break;
                }

                if (collision) {
                    player->Kill();
                }
            }
        }
    }

    void Render() override {
        AnimState state = this->animator.GetCurrent();

        float originalW = (float)this->Context->windowWidth - SCENE_GAMEPLAY_PADDING_L * 2;
        float originalH = (float)this->Context->windowHeight - SCENE_GAMEPLAY_PADDING_T - SCENE_GAMEPLAY_PADDING_B;

        float centerX = SCENE_GAMEPLAY_PADDING_L + originalW / 2.0f;
        float centerY = SCENE_GAMEPLAY_PADDING_T + originalH / 2.0f;

        float scaledW = originalW * state.scale;
        float scaledH = originalH * state.scale;

        SDL_FRect rect = {
            centerX - scaledW / 2.0f,
            centerY - scaledH / 2.0f,
            scaledW,
            scaledH
        };

        UTILS::setColor(this->Context->renderer, colorBg, state.alpha);
        SDL_RenderFillRect(this->Context->renderer, &rect);
        UTILS::setColor(this->Context->renderer, colorBorder, state.alpha);
        SDL_RenderRect(this->Context->renderer, &rect);

        for (auto* player : players) {
            if (isPlaying)
                player->Draw();
            else
                player->DrawStart(state.scale, state.alpha);
        }
    }
};

const string GameplayScene::Name = "Gameplay";
