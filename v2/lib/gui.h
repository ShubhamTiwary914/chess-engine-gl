#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_events.h>

namespace gui 
{

typedef struct {
    int renderFlags; 
    int windowFlags;
    SDL_Rect baseCoords = {0, 0, 640, 360};
    int SCREEN_WIDTH;
    int SCREEN_HEIGHT;
} ScreenOptions;

typedef struct{
    SDL_Renderer *renderer;
    SDL_Window *window;
} App;

typedef struct{
    char file;
    int rank;
} BoardUnit;


void initSDL(App *app, ScreenOptions *options); 
/// @returns integer pair of <width,height> in pixels 
std::pair<int,int> resizeHandler();
void updateScreen(App *app);



void start(gui::ScreenOptions *opts, gui::App *app);
void eventloop(gui::App *app);


};


