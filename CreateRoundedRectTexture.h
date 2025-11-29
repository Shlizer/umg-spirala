#include <SDL3/SDL.h>
#include <cmath>

static inline float clamp01(float v) {
    return v < 0 ? 0 : (v > 1 ? 1 : v);
}

SDL_Texture* CreateRoundedRectTexture(
    SDL_Renderer* renderer,
    int w, int h,
    int radius,
    int borderThickness,
    SDL_Color colorStart,
    SDL_Color colorEnd,
    SDL_Color borderColor,
    bool verticalGradient
) {
    SDL_Surface* surface = SDL_CreateSurface(w, h, SDL_PIXELFORMAT_RGBA32);
    if (!surface) return nullptr;

    Uint32* px = (Uint32*)surface->pixels;

    auto pack = [&](SDL_Color c) {
        return (Uint32)((c.a << 24) | (c.b << 16) | (c.g << 8) | c.r);
        };

    float outerR = radius;
    float innerR = radius - borderThickness;
    if (innerR < 0) innerR = 0;

    float outerR2 = outerR * outerR;
    float innerR2 = innerR * innerR;

    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {

            float t = clamp01(
                verticalGradient ? float(y) / float(h - 1)
                : float(x) / float(w - 1));

            SDL_Color fillColor = {
                (uint8_t)(colorStart.r + (colorEnd.r - colorStart.r) * t),
                (uint8_t)(colorStart.g + (colorEnd.g - colorStart.g) * t),
                (uint8_t)(colorStart.b + (colorEnd.b - colorStart.b) * t),
                (uint8_t)(colorStart.a + (colorEnd.a - colorStart.a) * t)
            };

            //-----------------------------
            // 1. Wyznaczenie "okręgu roboczego"
            //-----------------------------
            float dx = 0, dy = 0;
            float dist2 = 0;

            bool inCorner =
                (x < radius && y < radius) ||                     // TL
                (x >= w - radius && y < radius) ||                // TR
                (x < radius && y >= h - radius) ||                // BL
                (x >= w - radius && y >= h - radius);             // BR

            if (inCorner) {
                int cx = (x < radius) ? radius : (w - radius - 1);
                int cy = (y < radius) ? radius : (h - radius - 1);

                dx = float(x - cx);
                dy = float(y - cy);
                dist2 = dx * dx + dy * dy;
            }
            else {
                // boczne krawędzie (prosta)
                dist2 = 0; // traktujemy jako wewnątrz profilu
            }

            //-----------------------------
            // 2. Poza kształtem (transparent)
            //-----------------------------
            if (inCorner && dist2 > outerR2) {
                px[y * w + x] = 0;
                continue;
            }

            //-----------------------------
            // 3. Border
            //-----------------------------
            bool isBorder = false;
            if (inCorner) {
                isBorder = dist2 >= innerR2;
            }
            else {
                // płaskie krawędzie
                isBorder =
                    x < borderThickness ||
                    y < borderThickness ||
                    x >= w - borderThickness ||
                    y >= h - borderThickness;
            }

            if (isBorder) {
                px[y * w + x] = pack(borderColor);
                continue;
            }

            //-----------------------------
            // 4. Wypełnienie gradientem
            //-----------------------------
            px[y * w + x] = pack(fillColor);
        }
    }

    SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_DestroySurface(surface);
    return tex;
}
