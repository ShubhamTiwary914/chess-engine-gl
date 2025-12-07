#include "../lib/gui.h"

void gui::initSDL(gui::App *app, gui::ScreenOptions *options){
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("Couldn't initialize SDL: %s\n", SDL_GetError());
        exit(1);
    }
    options->renderFlags = SDL_RENDERER_ACCELERATED;
    options->windowFlags=0;

    app->window = SDL_CreateWindow("Chess Engine", 
        options->baseCoords.x, options->baseCoords.y, 
        options->baseCoords.w, options->baseCoords.h, 
        options->windowFlags
    );

    if (!app->window)
    {
        printf("Failed to open %d x %d window: %s\n", options->SCREEN_WIDTH, options->SCREEN_HEIGHT, SDL_GetError());
        exit(1);
    }

    app->renderer = SDL_CreateRenderer(app->window, -1, options->renderFlags); 
    if (!app->renderer)
    {
        printf("Failed to create renderer: %s\n", SDL_GetError());
        exit(1);
    }
}

void gui::updateScreen(gui::App *app){
    SDL_SetRenderDrawColor(app->renderer, 96, 128, 255, 255);
    SDL_RenderClear(app->renderer);
    SDL_RenderPresent(app->renderer);
}

///@brief start loop of game (runs once)
void start(gui::ScreenOptions *opts, gui::App *app){
    opts->SCREEN_HEIGHT = 640;
    opts->SCREEN_WIDTH  = 360;
    gui::initSDL(app, opts);    
    eventloop(app);
}

///@brief event loop of game (runs every clock cycle)
void eventloop(gui::App *app){
    bool EVENTLOOP = true;
    SDL_Event event;

    while(EVENTLOOP){
        gui::updateScreen(app);
        SDL_Delay(50);
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT) {
                EVENTLOOP = false;
            }
            //pressed escape => quit game
            else if(event.type == SDL_KEYDOWN){
                if(event.key.keysym.sym == SDLK_ESCAPE)
                    EVENTLOOP = false;
            }
        }
    }      
}