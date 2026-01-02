#pragma once
#include <string>
#include "TaskCounter.h"
#include "TaskEndGame.h"
#include "IScene.h"
#include "Utils.h"
#include "Player.h"
#include "Task.h"
#include "Config.h"
#include "CollisionSystem.h"

using namespace std;
using namespace CONFIG;

class GameplayScene : public IScene {
    bool isPlaying = false;
    bool gameEnded = false;
    SDL_Color colorBg = { 0, 0, 0, 100 };
    SDL_Color colorBorder = { 255, 255, 255, 255 };

    StateAnimator animator;
    vector<Player*> players;
    CollisionSystem collision;

    void SetupPlayers() {
        for (auto* p : players) delete p;
        players.clear();
        collision.Clear();

        this->Context->killLog.clear();
        this->Context->aliveCount = 0;
        this->Context->totalPlayers = 0;
        this->Context->winnerId = -1;

        int id = 0;

        for (auto& playerInfo : PLAYERS) {
            Player* p = new Player(this->Context, playerInfo, id++);
            p->SetRandomPosition(SCENE_GAMEPLAY_SPAWN_DISTANCE_EDGE,
                SCENE_GAMEPLAY_SPAWN_DISTANCE_PLAYER, players);
            players.push_back(p);
            this->Context->totalPlayers++;
            this->Context->aliveCount++;
        }
    }

    void Restart() {
        this->isPlaying = false;
        this->gameEnded = false;
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
                for (auto* p : players) p->Start();
            });
        this->Context->taskManager->AddTask(move(finishTask));
    }

public:
    static const string Name;
    string GetName() override { return Name; }

    GameplayScene(GameContext* ctx) : IScene(ctx) {
        this->animator.setSmooth(true);
        this->animator.SetState(ANIM_CLOSED);
        collision.Init(ctx->renderer, ctx->windowWidth, ctx->windowHeight);
    }

    ~GameplayScene() {
        for (auto* p : players) delete p;
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
            if (event.key.key == KEY_RESTART && this->isPlaying) {
                this->Restart();
            }
        }
    }

    void Update(float deltaTime) override {
        this->animator.Update(deltaTime);

        if (!isPlaying || gameEnded) return;

        const bool* keystate = SDL_GetKeyboardState(nullptr);
        
        for (auto* player : players) {
            if (!player->IsAlive()) continue;

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

            if (player->CheckCollisionAndDraw(collision)) {
                player->Kill();
            }

            if (this->Context->aliveCount <= 1 && !gameEnded) {
                gameEnded = true;

                char buf[32];
                if (this->Context->totalPlayers == 1 && this->Context->aliveCount == 0) {
                    sprintf_s(buf, "You lost!");
                }
                else if (this->Context->aliveCount == 1) {
                    for (auto* p : players) {
                        if (p->IsAlive()) {
                            this->Context->winnerId = p->GetId();
                            this->Context->winnerColor = p->GetColor();
                            break;
                        }
                    }
                    sprintf_s(buf, "Player %d won!", this->Context->winnerId + 1);
                }
                else {
                    sprintf_s(buf, "Draw!");
                }

                auto task = make_unique<TaskEndGame>(this->Context, buf);
                this->Context->taskManager->AddTask(move(task));
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

        if (isPlaying) {
            SDL_RenderTexture(this->Context->renderer, collision.GetTexture(), nullptr, nullptr);
        }
        else {
            for (auto* player : players) {
                player->DrawStart(state.scale, state.alpha);
            }
        }
    }
};

const string GameplayScene::Name = "Gameplay";