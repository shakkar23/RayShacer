#pragma once

// this is for both x and y of the screen because square
//constexpr inline auto logicalWindowSize = 1000;
constexpr inline auto logicalWindowBottom = 0;

#include <SDL.h>
#include <SDL_image.h>

#include <vector>
#include <string>

class RenderWindow {
public:
	RenderWindow(const char* p_title, int p_w, int p_h);

	// window properties
	void getWindowSize(int& x, int& y);
	void setWindowSize(int x, int y);
	int getRefreshrate();

	//clean up
	void cleanUp();
	void clear();
	
	// sets what gets rendered
	bool render(SDL_Rect src, SDL_Rect dst, SDL_Texture* tex);
	bool renderEx(SDL_Rect src, SDL_Rect dst, SDL_Texture* tex, double angle, SDL_Point* center, SDL_RendererFlip flip);
	void display();
	void RenderRect(SDL_Rect dst);
	void RenderPixel(SDL_Color color, int x, int y);
	void RenderPixels(SDL_Point* points, int count);
	void renderCopy(SDL_Texture* texture, const SDL_Rect* srcrect, const SDL_Rect* dstrect);
	void renderSurface(SDL_Surface * surface, SDL_Rect * srcrect, SDL_Rect * dstrect);

	// get backend, should never be used
	SDL_Renderer* getRenderer();
	SDL_Window* getWindow();

	//utility functions for future renders
	int QueryTexture(SDL_Texture* texture, Uint32* format, int* access, int* w, int* h);
	void SetRenderDrawColor(Uint8 R, Uint8 G, Uint8 B, Uint8 A);
	void GetRenderDrawColor(Uint8* r, Uint8* g, Uint8* b, Uint8* a);
	SDL_Texture* CreateTextureFromSurface(SDL_Surface* surface);
	const std::string basePath;
	const std::string prefPath;
	const int numCoreCount;
private:
	SDL_Window* window;
	SDL_Renderer* renderer;

};
