#pragma once
#include <SDL3_ttf/SDL_ttf.h>
#include <vector>
#include "IScene.h"
#include "Config.h"
#include "Utils.h"

using namespace CONFIG;
using namespace UTILS;

class KillLogScene : public IScene {
    TTF_Font* font = nullptr;
    size_t lastLogSize = 0;

    struct LogLine {
        SDL_Texture* texture = nullptr;
        int width = 0;
        int height = 0;
    };
    std::vector<LogLine> lines;

    void ClearLines() {
        for (auto& line : lines) {
            if (line.texture) SDL_DestroyTexture(line.texture);
        }
        lines.clear();
        lastLogSize = 0;
    }

    void RebuildLines() {
        ClearLines();

        for (const auto& event : this->Context->killLog) {
            char buf[64];

            sprintf_s(buf, "Player %d crashed", event.victimId + 1);

            SDL_Surface* surface = TTF_RenderText_Blended(font, buf, 0, CONFIG::PLAYERS[event.victimId].color.toColor());
            LogLine line;
            line.texture = SDL_CreateTextureFromSurface(this->Context->renderer, surface);
            line.width = surface->w;
            line.height = surface->h;
            SDL_DestroySurface(surface);
            lines.push_back(line);
        }
        lastLogSize = this->Context->killLog.size();
    }

public:
    static const string Name;
    string GetName() override { return Name; }

    KillLogScene(GameContext* ctx) : IScene(ctx) {
        font = UTILS::loadFont(FONT_FACE_LATO_REGULAR, 16);
    }

    ~KillLogScene() {
        ClearLines();
        if (font) TTF_CloseFont(font);
    }

    void Update(float deltaTime) override {
        if (this->Context->killLog.size() != lastLogSize) {
            RebuildLines();
        }
    }

    void Render() override {
        float y = 10.0f;
        float padding = 10.0f;

        for (const auto& line : lines) {
            if (!line.texture) continue;
            float x = this->Context->windowWidth - line.width - padding;
            SDL_FRect rect = { x, y, (float)line.width, (float)line.height };
            SDL_RenderTexture(this->Context->renderer, line.texture, nullptr, &rect);
            y += line.height + 4.0f;
        }
    }
};

const string KillLogScene::Name = "KillLog";
