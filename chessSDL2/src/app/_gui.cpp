#include "./_gui.h"


// GUI globals
SDL_Window *window = nullptr;
SDL_Renderer *screen = nullptr;
SDL_Rect baseCoords = {0, 0, 640, 360};
std::unordered_map<char, SDL_Texture *> textures;
int sqBase = (baseCoords.h/8);
int scaleX = 0;
int scaleY = 0;

//logger
SDL_Window *logWindow = nullptr;
SDL_Renderer *logScreen = nullptr;
TTF_Font* font;
int fontSize = 18;
std::string fontFamily = "jetbrains_var";
bool LOG_MODE = false;


int initSDL(){
    window = SDL_CreateWindow("Chess Master", baseCoords.x, baseCoords.y, baseCoords.w, baseCoords.h, SDL_WINDOW_RESIZABLE);
    assert(window);
    screen = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    assert(screen);
    SDL_SetRenderDrawColor(screen, 0xff, 0xff, 0xff, 1);
    SDL_RenderClear(screen);

    if(LOG_MODE){
        logWindow = SDL_CreateWindow("Chess Log Window", baseCoords.x, baseCoords.y*2, baseCoords.w, baseCoords.h, SDL_WINDOW_RESIZABLE);
        logScreen = SDL_CreateRenderer(logWindow, -1, SDL_RENDERER_ACCELERATED);
        SDL_SetRenderDrawColor(logScreen, 0xff, 0xff, 0xff, 1);
        SDL_RenderClear(logScreen);
    }
    return 0;
}



void renderBoard(selectedPiece &selected) {
    int i = 63;
    SDL_SetRenderDrawColor(screen, 255, 255, 255, 255);
    SDL_RenderClear(screen);
    SDL_Rect squareRect = {0, 0, sqBase, sqBase};
    for(int rank=8; rank>=1; rank--) {
        for(int file=0; file<8; file++) {
            setSquareCoords(squareRect, sqBase, file, rank);
            //possible move?
            bool isPossibleMove = false;
            if(selected.isSelected) {
                int bitIndex = getPieceBitIndex((char)('a' + file), rank);
                isPossibleMove = getBit(selected.moves, bitIndex);
            }
            // selected piece square
            if(selected.isSelected && file == (selected.file - 'a') && rank == selected.rank) {
                if((rank + file) % 2 == 0) //black square
                    SDL_SetRenderDrawColor(screen, 200, 173, 53, 0xff);
                else  //white square
                    SDL_SetRenderDrawColor(screen, 226, 198, 75, 0xff);
            }
            // possible move square
            else if(isPossibleMove) {
                if((rank + file) % 2 == 0) //black square
                    SDL_SetRenderDrawColor(screen, 200, 173, 53, 0xff);
                else  //white square
                    SDL_SetRenderDrawColor(screen, 226, 198, 75, 0xff);
            }
            //normal board 
            else if((rank + file) % 2 == 0) { //black square
                SDL_SetRenderDrawColor(screen, 83, 124, 73, 0xff);
            }
            else { //white square
                SDL_SetRenderDrawColor(screen, 255, 233, 175, 0xff);
            } 
            SDL_RenderFillRect(screen, &squareRect);
            i--;
        }
    }
}


void renderPieces(PieceList &mainboard){
    //reduce sprite size (100%):  (100-x)% 
    double spriteDelta = (1.0f - pieceSpriteGap);
    SDL_Rect destRect = {sqBase, sqBase, (int)(spriteDelta * sqBase), (int)(spriteDelta * sqBase)};
    SDL_Rect pieceRect = destRect;
    for(int rank=8; rank>=1; rank--){
        for(int file=0; file<8; file++){
            setSquareCoords(destRect, sqBase, file, rank);
            pieceRect = destRect;
            pieceRect.x += sqBase*(pieceSpriteGap/2);
            pieceRect.y += sqBase*(pieceSpriteGap/2);
            char piece = mainboard.getPiece((char)(file+65), rank);
            if(piece == 'E' || piece == 'e') 
                continue;
            SDL_RenderCopy(screen, textures[piece], NULL, &pieceRect);
        }
    }
}


void renderString(std::string log){
    // Set background color to white
    SDL_SetRenderDrawColor(logScreen, 255, 255, 255, 255);
    SDL_RenderClear(logScreen);

    SDL_Color textColor = {0, 0, 0, 255};
    int lineHeight = fontSize + 2; // Add some spacing between lines
    int yPos = 10; // Starting y position

    // Split text by newlines
    std::string line;
    std::stringstream ss(log);
    while (std::getline(ss, line)) {
        if (line.empty()) continue; // Skip empty lines

        SDL_Surface* textSurface = TTF_RenderText_Solid(font, line.c_str(), textColor);
        BREAKPOINT(textSurface, "Failed to create text surface: %s"+ (std::string)TTF_GetError()); 
        
        // Create texture from surface
        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(logScreen, textSurface);
        BREAKPOINT(textTexture, "Texture for Font not created!");
        
        // Set up destination rectangle
        SDL_Rect destRect = {10, yPos, textSurface->w, textSurface->h};
        
        // Render the text
        SDL_RenderCopy(logScreen, textTexture, NULL, &destRect);
        
        // Clean up
        SDL_FreeSurface(textSurface);
        SDL_DestroyTexture(textTexture);
        
        // Move to next line
        yPos += lineHeight;
    }
}


//fill board -> fill pieces -> update screen
void updateScreen(){
    SDL_RenderPresent(screen);
    if(LOG_MODE)
        SDL_RenderPresent(logScreen);
}



void resizeHandler(){
    int currentW, currentH;
    SDL_GetWindowSize(window, &currentW, &currentH);
    sqBase = std::min(currentW/8, currentH/8);
}


void preLoadTextures(std::unordered_map<char, int> &piecesCharMap){
    //For font (if log mode)
    if(LOG_MODE){
        BREAKPOINT((TTF_Init() != -1), "SDL_TTF package properly not init");
        const char *fontPath = getFullPath(((std::string)("assets/fonts/" + fontFamily +".ttf")).c_str() );
        font = TTF_OpenFont(fontPath, 18);
        BREAKPOINT(font, "Font not loaded!");
    }
    //IMG_Init(IMG_INIT_PNG);
    for(auto it: piecesCharMap){
        char piece = it.first;
        if(piece == 'E' || piece == 'e') 
            continue;
        std::string filePath = getFullPath("assets/v2/" + std::string(1, piece) + ".png");
        SDL_Surface *img = IMG_Load(filePath.c_str());
        SDL_Texture *texture;
        texture = SDL_CreateTextureFromSurface(screen, img);
        SDL_FreeSurface(img);
        textures[piece] = texture;
        assert(texture != nullptr);
    }
}


void exitGame(){
    SDL_DestroyRenderer(screen);
	SDL_DestroyWindow(window);
	SDL_Quit();
}