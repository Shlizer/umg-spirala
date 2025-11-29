#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_ttf/SDL_ttf.h>
#include "Static.h"
#include "Game.h"
#include "GameContext.h"

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();

    GameContext* ctx = new GameContext();
    ctx->window = SDL_CreateWindow(
        CONFIG::TITLE,
        ctx->windowWidth,
        ctx->windowHeight,
        ctx->fullscreen ? SDL_WINDOW_FULLSCREEN : 0
    );
    ctx->renderer = SDL_CreateRenderer(ctx->window, nullptr);

    SDL_SetRenderLogicalPresentation(
        ctx->renderer,
        ctx->windowWidth,
        ctx->windowHeight,
        SDL_LOGICAL_PRESENTATION_STRETCH
    );

    {
        Game game(ctx);
        game.Run();
    }

    SDL_DestroyRenderer(ctx->renderer);
    SDL_DestroyWindow(ctx->window);
    delete ctx;
    TTF_Quit();
    SDL_Quit();

    return 0;
}
