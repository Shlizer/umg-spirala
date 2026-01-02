class TaskEndGame : public ITask {
    GameContext* Context;
    TTF_Font* font;
    
    SDL_Texture* texPrefix = nullptr;
    SDL_Texture* texPlayer = nullptr;
    SDL_Texture* texSuffix = nullptr;
    float wPrefix = 0, wPlayer = 0, wSuffix = 0;
    float h = 0;
    bool hasPlayer = false;
    bool flagToErase = false;

public:
    TaskEndGame(GameContext* Context, const char* prefix, const char* playerNum = nullptr, const char* suffix = nullptr, SDL_Color playerColor = {255,255,255,255}) 
        : Context(Context), ITask(0) 
    {
        this->font = UTILS::loadFont(CONFIG::FONT_FACE_STYLED, CONFIG::FONT_SIZE_COUNTER);
        SDL_Color white = {255,255,255,255};
        
        SDL_Surface* s1 = TTF_RenderText_Blended(font, prefix, 0, white);
        texPrefix = SDL_CreateTextureFromSurface(Context->renderer, s1);
        wPrefix = (float)s1->w;
        h = (float)s1->h;
        SDL_DestroySurface(s1);
        
        if (playerNum) {
            hasPlayer = true;
            SDL_Surface* s2 = TTF_RenderText_Blended(font, playerNum, 0, playerColor);
            texPlayer = SDL_CreateTextureFromSurface(Context->renderer, s2);
            wPlayer = (float)s2->w;
            SDL_DestroySurface(s2);
            
            SDL_Surface* s3 = TTF_RenderText_Blended(font, suffix, 0, white);
            texSuffix = SDL_CreateTextureFromSurface(Context->renderer, s3);
            wSuffix = (float)s3->w;
            SDL_DestroySurface(s3);
        }
    }

    ~TaskEndGame() {
        if (texPrefix) SDL_DestroyTexture(texPrefix);
        if (texPlayer) SDL_DestroyTexture(texPlayer);
        if (texSuffix) SDL_DestroyTexture(texSuffix);
        TTF_CloseFont(font);
    }

    bool UpdateDelayed(float deltaTime) override { return this->flagToErase; }

    void HandleEvent(const SDL_Event& event) override {
        if (event.key.key == KEY_RESTART) {
            this->flagToErase = true;
        }
    }

    void Render() override {
        float totalW = wPrefix + wPlayer + wSuffix;
        float x = (Context->windowWidth - totalW) / 2.0f;
        float y = (Context->windowHeight - h) / 2.0f;
        
        SDL_FRect r1 = {x, y, wPrefix, h};
        SDL_RenderTexture(Context->renderer, texPrefix, nullptr, &r1);
        
        if (hasPlayer) {
            SDL_FRect r2 = {x + wPrefix, y, wPlayer, h};
            SDL_RenderTexture(Context->renderer, texPlayer, nullptr, &r2);
            
            SDL_FRect r3 = {x + wPrefix + wPlayer, y, wSuffix, h};
            SDL_RenderTexture(Context->renderer, texSuffix, nullptr, &r3);
        }
    }
};