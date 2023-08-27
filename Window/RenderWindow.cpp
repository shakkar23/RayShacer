//not my code
#include <SDL.h>
#include <SDL_image.h>

#include "RenderWindow.hpp"


RenderWindow::RenderWindow(const char* p_title, const int p_w, const int p_h)
    : basePath(SDL_GetBasePath()), prefPath(SDL_GetPrefPath("Shakkar",p_title)), numCoreCount(SDL_GetCPUCount()),  window(NULL), renderer(NULL) {

    window =
        SDL_CreateWindow(p_title, SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED, p_w, p_h, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
    if (window == NULL) {
        // std::cout << "Window failed to init. Error: " << SDL_GetError() << std::endl;
    }

    renderer = SDL_CreateRenderer(
        window, -1, SDL_RENDERER_ACCELERATED | SDL_RendererFlags::SDL_RENDERER_PRESENTVSYNC);
    //SDL_RenderSetLogicalSize(this->renderer, logicalWindowSize, logicalWindowSize);



    SDL_Surface* surface;     // Declare an SDL_Surface to be filled in with pixel data from an image file
#define BLACK 0x0000
#define WHITE 0x0fff
    Uint16 pixels[16 * 16] = {  // ...or with raw pixel data:
      WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE,
      WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, BLACK, BLACK, BLACK, BLACK, BLACK, WHITE, WHITE, WHITE, WHITE, WHITE,
      WHITE, WHITE, WHITE, WHITE, WHITE, BLACK, WHITE, WHITE, WHITE, WHITE, WHITE, BLACK, WHITE, WHITE, WHITE, WHITE,
      WHITE, WHITE, WHITE, WHITE, BLACK, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, BLACK, WHITE, WHITE, WHITE, WHITE,
      WHITE, WHITE, WHITE, WHITE, BLACK, WHITE, WHITE, WHITE, WHITE, WHITE, BLACK, WHITE, BLACK, WHITE, WHITE, WHITE,
      WHITE, WHITE, BLACK, BLACK, WHITE, WHITE, BLACK, WHITE, WHITE, WHITE, WHITE, WHITE, BLACK, BLACK, WHITE, WHITE,
      WHITE, WHITE, WHITE, BLACK, BLACK, WHITE, WHITE, WHITE, BLACK, BLACK, WHITE, WHITE, BLACK, WHITE, WHITE, WHITE,
      WHITE, WHITE, WHITE, BLACK, WHITE, WHITE, WHITE, WHITE, BLACK, BLACK, WHITE, BLACK, BLACK, WHITE, WHITE, WHITE,
      WHITE, WHITE, WHITE, BLACK, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, BLACK, WHITE, WHITE, WHITE, WHITE,
      WHITE, WHITE, WHITE, BLACK, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, BLACK, WHITE, WHITE, WHITE, WHITE,
      WHITE, WHITE, WHITE, WHITE, BLACK, WHITE, WHITE, WHITE, WHITE, BLACK, BLACK, WHITE, WHITE, WHITE, WHITE, WHITE,
      WHITE, WHITE, WHITE, WHITE, WHITE, BLACK, BLACK, BLACK, BLACK, WHITE, WHITE, BLACK, WHITE, WHITE, WHITE, WHITE,
      WHITE, WHITE, WHITE, WHITE, WHITE, BLACK, WHITE, WHITE, WHITE, WHITE, WHITE, BLACK, BLACK, WHITE, WHITE, WHITE,
      WHITE, WHITE, WHITE, WHITE, WHITE, BLACK, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE,
      WHITE, WHITE, WHITE, WHITE, BLACK, BLACK, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE,
      WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE
    };
#undef WHITE
#undef BLACK
    surface = SDL_CreateRGBSurfaceFrom(pixels, 16, 16, 16, 16 * 2, 0x0f00, 0x00f0, 0x000f, 0xf000);

    // The icon is attached to the window pointer
    SDL_SetWindowIcon(window, surface);

    // ...and the surface containing the icon pixel data is no longer required.
    SDL_FreeSurface(surface);

    
}

int RenderWindow::getRefreshrate() {
    int displayIndex = SDL_GetWindowDisplayIndex(window);
    SDL_DisplayMode mode;
    SDL_GetDisplayMode(displayIndex, 0, &mode);
    return mode.refresh_rate;
}


void RenderWindow::cleanUp() { SDL_DestroyWindow(window); }

void RenderWindow::clear() { SDL_RenderClear(renderer); }

bool RenderWindow::render(SDL_Rect src, SDL_Rect dst, SDL_Texture* tex) {

    auto checkIfSet = [&](SDL_Rect box) {if ((box.x == 0) && (box.y == 0) && (box.w == 0) && (box.h == 0)) return true; else return false; };

    auto err = SDL_RenderCopy(this->renderer, tex, (checkIfSet(src)) ? NULL : &src, &dst);

    if (err != 0) {
        SDL_Log("SDL2 Error: %s", SDL_GetError());
        return false;
    }
    return true;

}

// passing in NULL for center means no rotation
bool RenderWindow::renderEx(SDL_Rect src, SDL_Rect dst, SDL_Texture* tex, double angle, SDL_Point* center, SDL_RendererFlip flip) {

    auto checkIfSet = [&](SDL_Rect box) {if ((box.x == 0) && (box.y == 0) && (box.w == 0) && (box.h == 0)) return true; else return false; };

    auto err = SDL_RenderCopyEx(this->renderer, tex, (checkIfSet(src)) ? NULL : &src, &dst, angle, center, flip);

    if (err != 0) {
        SDL_Log("SDL2 Flip Error: %s", SDL_GetError());
        return false;
    }
    return true;
}

void RenderWindow::RenderRect(SDL_Rect dst) {
    SDL_RenderDrawRect(this->renderer, &dst);
}

void RenderWindow::RenderPixel(SDL_Color color, int x, int y) {
    SDL_Color c{};
    this->GetRenderDrawColor(&c.r, &c.g, &c.b, &c.a);
	
	SDL_SetRenderDrawColor(this->renderer, color.r, color.g, color.b, color.a);
	SDL_RenderDrawPoint(this->renderer, x, y);
    
	SDL_SetRenderDrawColor(this->renderer, c.r, c.g, c.b, c.a);
}
void RenderWindow::RenderPixels(SDL_Point *points, int count) {
	SDL_Color c{};
	this->GetRenderDrawColor(&c.r, &c.g, &c.b, &c.a);
	
	SDL_SetRenderDrawColor(this->renderer, c.r, c.g, c.b, c.a);
	SDL_RenderDrawPoints(this->renderer, points, count);
	
	SDL_SetRenderDrawColor(this->renderer, c.r, c.g, c.b, c.a);
}
void RenderWindow::renderSurface(SDL_Surface * surface, SDL_Rect * src, SDL_Rect * dst) {
    auto tex  = SDL_CreateTextureFromSurface(this->renderer, surface);
	SDL_RenderCopy(this->renderer, tex, src, dst);
    SDL_DestroyTexture(tex);
}

void RenderWindow::SetRenderDrawColor(Uint8 R, Uint8 G, Uint8 B, Uint8 A) {
    SDL_SetRenderDrawColor(this->renderer, R, G, B, A);
}

void RenderWindow::GetRenderDrawColor(Uint8* r, Uint8* g, Uint8* b, Uint8* a) {
    SDL_GetRenderDrawColor(this->renderer, r, g, b, a);
}

void RenderWindow::display() { SDL_RenderPresent(renderer); }

void RenderWindow::getWindowSize(int& x, int& y) {
    SDL_GetWindowSize(this->window, &x, &y);
}

void RenderWindow::setWindowSize(int x, int y) {
    SDL_SetWindowSize(this->window, x, y);
}

SDL_Texture* RenderWindow::CreateTextureFromSurface(SDL_Surface* surface) {
    return SDL_CreateTextureFromSurface(this->renderer, surface);
}


void RenderWindow::renderCopy(SDL_Texture* texture, const SDL_Rect* srcrect, const SDL_Rect* dstrect) {
    SDL_RenderCopy(renderer, texture, srcrect, dstrect);
}

int RenderWindow::QueryTexture(SDL_Texture* texture, Uint32* format, int* access, int* w, int* h) {
    return SDL_QueryTexture(texture, format, access, w, h);
}


SDL_Renderer* RenderWindow::getRenderer() { return renderer; }

SDL_Window* RenderWindow::getWindow() { return window; }