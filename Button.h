#pragma once
#include <SDL3/SDL_render.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <string>
#include "Selectable.h"
#include "CreateRoundedRectTexture.h"

using namespace std;

class Button : public Selectable {
protected:
    SDL_Renderer* renderer;

    SDL_FRect rectText;
    string text;
    TTF_Font* font;
    SDL_Texture* textureBG = nullptr;
    SDL_Texture* textureBGSelected = nullptr;
    SDL_Texture* textureText = nullptr;

    //SDL_Color colorNormal;
    //SDL_Color colorHover;
    //SDL_Color colorCurrent;
    //SDL_Color colorBg = { 90,120,150,220 };
    //SDL_Color colorBgHover = { 70,100,130,220 };
    //SDL_Color colorBorder = { 255,255,255,255 };
    SDL_Color colorText = { 255, 255, 255, 255 };
    //SDL_Color colorBg = { 90, 120, 150, 220 };
    //SDL_Color colorBgHover = { 110, 140, 170, 220 };
    //SDL_Color colorBgSelected = { 70, 100, 130, 255 };

    SDL_Color colorBorder = { 255, 255, 255, 155 };
    SDL_Color colorBorderSelected = { 255, 255, 255, 255 };

    SDL_Color colorNormalBG1 = { 100, 100, 100, 190 };
    SDL_Color colorNormalBG2 = { 140, 140, 140, 190 };
    SDL_Color colorHoverBG1 = { 100, 100, 100, 255 };
    SDL_Color colorHoverBG2 = { 140, 140, 140, 255 };
    SDL_Color colorSelectedBG1 = { 76, 115, 143, 250 };
    SDL_Color colorSelectedBG2 = { 114, 176, 169, 250 };
    SDL_Color colorSelectedHoverBG1 = { 76, 115, 143, 255 };
    SDL_Color colorSelectedHoverBG2 = { 114, 176, 169, 255 };

public:
    Button(SDL_Renderer* renderer, const string& txt, TTF_Font* font, SDL_FRect rect)
        : renderer(renderer), text(txt), font(font), Selectable(rect)
    {
        UpdateTextures();
        onSelected.AddListener([this](bool selected) { UpdateTextures(); });
    }

    ~Button() {
        DestroyTextures();
    }

    void DestroyTextures() {
        if (textureBG) SDL_DestroyTexture(textureBG);
        if (textureBGSelected) SDL_DestroyTexture(textureBGSelected);
        if (textureText) SDL_DestroyTexture(textureText);
    }

    void UpdateTextures() {
        DestroyTextures();

        SDL_Surface* surf = TTF_RenderText_Blended(font, text.c_str(), 0, colorText);
        textureText = SDL_CreateTextureFromSurface(renderer, surf);
        float w = (float)surf->w;
        float h = (float)surf->h;
        rectText = {
            rect.x + (rect.w / 2) - (w / 2),
            rect.y + (rect.h / 2) - (h / 2),
            (float)surf->w,
            (float)surf->h
        };
        SDL_DestroySurface(surf);

        SDL_Color bg1;
        SDL_Color bg2;
        if (IsSelected()) {
            if (IsHovered()) {
                bg1 = colorSelectedHoverBG1;
                bg2 = colorSelectedHoverBG2;
            }
            else {
                bg1 = colorSelectedBG1;
                bg2 = colorSelectedBG2;
            }
        }
        else {
            if (IsHovered()) {
                bg1 = colorHoverBG1;
                bg2 = colorHoverBG2;
            }
            else {
                bg1 = colorNormalBG1;
                bg2 = colorNormalBG2;
            }
        }

        textureBG = CreateRoundedRectTexture(
            renderer,
            rect.w, rect.h,        // w, h
            10,              // radius
            IsSelected() ? 2 : 1,               // border thickness
            bg1,// gradient start (red)
            bg2,// gradient end (blue)
            IsSelected() ? colorBorderSelected : colorBorder, // border color
            true             // verticalGradient
        );
    }

    void Render() {
        if (!textureText) return;

        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_RenderTexture(renderer, textureBG, nullptr, &rect);

        // Text
        SDL_RenderTexture(renderer, textureText, nullptr, &rectText);

        // Tło
        //SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        //SDL_SetRenderDrawColor(renderer, bg.r, bg.g, bg.b, bg.a);
        //SDL_RenderFillRect(renderer, &rect);

        // Border
        //SDL_Color border = IsSelected() ? colorBorderSelected : colorBorder;
        //SDL_SetRenderDrawColor(renderer, border.r, border.g, border.b, border.a);

        // Gruby border dla selected
        //if (IsSelected()) {
        //    SDL_FRect outer = { rect.x - 2, rect.y - 2, rect.w + 4, rect.h + 4 };
        //    SDL_RenderRect(renderer, &outer);
        //}
        //SDL_RenderRect(renderer, &rect);

        // Text
        //SDL_RenderTexture(renderer, textureText, nullptr, &rectText);
    }
};
