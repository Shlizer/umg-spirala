#pragma once
#include <vector>
#include <cmath>
#include <SDL3/SDL.h>

class CollisionSystem {
private:
    std::vector<uint8_t> grid;
    SDL_Texture* texture = nullptr;
    SDL_Renderer* renderer = nullptr;
    int width = 0;
    int height = 0;

public:
    ~CollisionSystem() {
        if (this->texture) SDL_DestroyTexture(this->texture);
    }

    void Init(SDL_Renderer* renderer, int w, int h) {
        this->renderer = renderer;
        this->width = w;
        this->height = h;
        this->grid.assign(w * h, 0);

        if (this->texture) SDL_DestroyTexture(this->texture);
        this->texture = SDL_CreateTexture(
            renderer,
            SDL_PIXELFORMAT_RGBA8888,
            SDL_TEXTUREACCESS_TARGET,
            w, h
        );
        SDL_SetTextureBlendMode(this->texture, SDL_BLENDMODE_BLEND);
        Clear();
    }

    void Clear() {
        std::fill(this->grid.begin(), this->grid.end(), 0);
        SDL_Texture* prevTarget = SDL_GetRenderTarget(this->renderer);
        SDL_SetRenderTarget(this->renderer, this->texture);
        SDL_SetRenderDrawColor(this->renderer, 0, 0, 0, 0);
        SDL_RenderClear(this->renderer);
        SDL_SetRenderTarget(this->renderer, prevTarget);
    }

    bool IsInBounds(int x, int y) const {
        return x >= 0 && x < this->width && y >= 0 && y < this->height;
    }

    bool CheckPixel(int x, int y) const {
        if (!IsInBounds(x, y)) return true;
        return this->grid[y * this->width + x] != 0;
    }

    void SetPixel(int x, int y) {
        if (IsInBounds(x, y)) {
            this->grid[y * this->width + x] = 1;
        }
    }

    bool CheckAndDrawLine(float x0, float y0, float x1, float y1, int thickness, SDL_Color color, bool skipCollisionCheck, bool skipDraw, float* outCollisionX = nullptr, float* outCollisionY = nullptr) {
        int ix0 = (int)x0, iy0 = (int)y0;
        int ix1 = (int)x1, iy1 = (int)y1;
        int halfThick = thickness / 2;

        bool newPixel = (ix0 != ix1 || iy0 != iy1);
        bool collision = false;

        if (newPixel) {
            if (!skipCollisionCheck) {
                for (int tx = -halfThick; tx <= halfThick && !collision; tx++) {
                    for (int ty = -halfThick; ty <= halfThick && !collision; ty++) {
                        if (CheckPixel(ix1 + tx, iy1 + ty)) {
                            collision = true;
                        }
                    }
                }
            }

            if (!skipDraw) {
                DrawLineToGridAndTexture(ix0, iy0, ix1, iy1, thickness, color);
            }
        }

        if (collision && outCollisionX && outCollisionY) {
            *outCollisionX = x1;
            *outCollisionY = y1;
        }

        return collision;
    }

    void DrawPoint(int x, int y, int thickness, SDL_Color color) {
        SDL_Texture* prevTarget = SDL_GetRenderTarget(this->renderer);
        SDL_SetRenderTarget(this->renderer, this->texture);
        SDL_SetRenderDrawColor(this->renderer, color.r, color.g, color.b, color.a);

        int halfThick = thickness / 2;
        for (int tx = -halfThick; tx <= halfThick; tx++) {
            for (int ty = -halfThick; ty <= halfThick; ty++) {
                SetPixel(x + tx, y + ty);
            }
        }
        SDL_FRect rect = { (float)(x - halfThick), (float)(y - halfThick), (float)thickness, (float)thickness };
        SDL_RenderFillRect(this->renderer, &rect);

        SDL_SetRenderTarget(this->renderer, prevTarget);
    }

    SDL_Texture* GetTexture() const { return this->texture; }

private:
    void DrawLineToGridAndTexture(int x0, int y0, int x1, int y1, int thickness, SDL_Color color) {
        SDL_Texture* prevTarget = SDL_GetRenderTarget(this->renderer);
        SDL_SetRenderTarget(this->renderer, this->texture);
        SDL_SetRenderDrawColor(this->renderer, color.r, color.g, color.b, color.a);

        int dxAbs = std::abs(x1 - x0);
        int dyAbs = std::abs(y1 - y0);
        int sx = x0 < x1 ? 1 : -1;
        int sy = y0 < y1 ? 1 : -1;
        int err = dxAbs - dyAbs;
        int cx = x0, cy = y0;
        int halfThick = thickness / 2;

        while (true) {
            for (int tx = -halfThick; tx <= halfThick; tx++) {
                for (int ty = -halfThick; ty <= halfThick; ty++) {
                    SetPixel(cx + tx, cy + ty);
                }
            }
            SDL_FRect rect = { (float)(cx - halfThick), (float)(cy - halfThick), (float)thickness, (float)thickness };
            SDL_RenderFillRect(this->renderer, &rect);

            if (cx == x1 && cy == y1) break;
            int e2 = 2 * err;
            if (e2 > -dyAbs) { err -= dyAbs; cx += sx; }
            if (e2 < dxAbs) { err += dxAbs; cy += sy; }
        }

        SDL_SetRenderTarget(this->renderer, prevTarget);
    }
};