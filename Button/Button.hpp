#pragma once
#include <SDL.h>
#include <SDL_ttf.h>

#include "../Window/RenderWindow.hpp"
#include <string>
class Button {
public:
	~Button() {
		if(texture)
			SDL_DestroyTexture(texture);
		if(surface)
			SDL_FreeSurface(surface);
	}
	void setFont(TTF_Font * other) {
	font = other;
	}
	void load(RenderWindow &window, std::string contents) {

		if (surface) {
			SDL_FreeSurface(surface);
		}
		SDL_Color  color = { r,g,b,a };
		surface = TTF_RenderText_Solid(font, contents.c_str(), color);
		texture = window.CreateTextureFromSurface(surface);

		SDL_SetTextureColorMod(texture, 0, 0, 255);
	}
	bool isClicked(int x, int y) {
	// returns if the button was clicked
		if (x > core.x && x < core.x + core.w && y > core.y && y < core.y + core.h) {
			return true;
		}
		return false;
	}
	void render(RenderWindow &window) {
		window.RenderRect(core);
		window.renderCopy(texture, NULL, &core);
	}
	TTF_Font *font;
	SDL_Surface* surface;
	SDL_Texture* texture;
	SDL_Rect core = { 0, 0, 0, 0 };
	Uint8 r = 0, g = 0, b = 255, a = 0;
};