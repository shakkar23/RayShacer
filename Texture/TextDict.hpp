#pragma once
#pragma once
//not my code, but modified by me
#ifndef SHAK_ENTITY
#define SHAK_ENTITY

#include <map>
#include <string>
#include <string_view>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "../Window/RenderWindow.hpp"

class RenderWindow;
class Texture;

class Renderable {
public:
	Renderable() :texture(nullptr) {}
	Renderable(Texture* tex) :texture(tex) {}
	virtual void load(RenderWindow& window, std::string path, int32_t x, int32_t y, uint8_t tileSize) = 0;
	virtual void render(RenderWindow& renderer) = 0;
	Texture* texture;
};


class Texture {
public:
	Texture() = default;
	Texture(RenderWindow& window, std::string_view path) :path(path) {
		auto surface = IMG_Load(path.data());

		width = surface->w;
		height = surface->h;

		this->texture = SDL_CreateTextureFromSurface(window.getRenderer(), surface);
		SDL_FreeSurface(surface);

	}

	~Texture() {
		if (texture)
			SDL_DestroyTexture(texture);
	}

	Texture& operator=(const Texture& other) {

		if (texture)
			SDL_DestroyTexture(texture);

		this->texture = other.texture;

		return (*this);
	}

	SDL_Texture* texture;
	std::string path;
	uint8_t width;
	uint8_t height;
};

class Sprite : public Renderable {
public:
	Sprite() :Renderable(), surface(nullptr), srcRect({ 0,0,0,0 }), destRect({ 0,0,0,0 }) {};
	Sprite(RenderWindow& window, std::string_view path, Texture* tex) {

		// set class members
		this->texture = tex;
		this->srcRect = { 0, 0, 0, 0 };
		this->destRect = { 0, 0, 0, 0 };
	}
	Sprite(const Sprite& other) = delete;
	Sprite(Sprite&& other) = delete;
	Sprite& operator=(const Sprite& other) = delete;
	Sprite& operator=(Sprite&& other) noexcept {
		this->texture = other.texture;
		this->surface = other.surface;
		this->srcRect = other.srcRect;
		this->destRect = other.destRect;

		other.texture = nullptr;
		other.surface = nullptr;

		return (*this);
	}


	void load(RenderWindow& window, std::string path, int32_t x = 0, int32_t y = 0, uint8_t tileSize = 0) override;
	void render(RenderWindow& renderer) override;
	~Sprite() {
		/*if (surface)
			SDL_FreeSurface(surface);*/
	}
	SDL_Surface* surface;
	SDL_Rect srcRect = { 0,0,0,0 };
	SDL_Rect destRect = { 0,0,0,0 };
};

// a sprite sheet is a collection of other smaller "sprites" that are represented by NxN N being "sections"
// you are freely allowed to manipulate where 
class SpriteSheet : public Renderable {
public:
	SpriteSheet() :Renderable(), srcRect({ 0,0,0,0 }), destRect({ 0,0,0,0 }), tileSize(0) {};
	SpriteSheet(int32_t x, int32_t y, uint8_t tileSize, Texture* tex, std::string path) : Renderable(tex), tileSize(tileSize) {

		// set class members
		this->srcRect = { tileSize * x, tileSize * y, tileSize, tileSize };
		this->destRect = { 0, 0, tileSize, tileSize };
	}

	~SpriteSheet() {

	}

	// the x and y are 0,0 on the top left of the texture
	void load(RenderWindow& window, std::string path, int32_t x, int32_t y, uint8_t tileSize) override;
	void render(RenderWindow& window) override;
	void renderEx(RenderWindow& renderer, SDL_RendererFlip flip);
	void updateSection(uint8_t x, uint8_t y);
	SDL_Rect srcRect = { 0,0,0,0 };
	SDL_Rect destRect = { 0,0,0,0 };
	uint8_t tileSize = 0;
	double angle = 0;
	SDL_Point center = {};
};

// surface textures are always going to render its entire self no matter what
class SurfaceTexture : public Renderable {
public:
	SurfaceTexture() :Renderable(), surface(nullptr), destRect({ 0,0,0,0 }) {};
	SurfaceTexture(RenderWindow& window, std::string_view path) {
		surface = IMG_Load(path.data());
		this->path = path;
	}
	void load(RenderWindow& window, std::string path, int32_t x = 0, int32_t y = 0, uint8_t tileSize = 1) override;
	void render(RenderWindow& renderer) override;

	// call this after youve made modifications using the other utility functions to "save" the changes into the texture
	inline void createTexture(RenderWindow window) {
		if (texture)
			SDL_DestroyTexture(texture);
		texture = SDL_CreateTextureFromSurface(window.getRenderer(), surface);
	}

	inline void setSurfaceAlphaMod(Uint8 a) {

		[[unlikely]] if (SDL_SetSurfaceAlphaMod(surface, a)) {

			std::string error = "setSurfaceAlphaMod failed because: ";

			error += SDL_GetError();

			throw error;
		}
	}

	inline void setSurfaceBlendMode(SDL_BlendMode blend) {

		[[unlikely]] if (SDL_SetSurfaceBlendMode(surface, blend)) {

			std::string error = "SDL_SetSurfaceBlendMode failed because: ";

			error += SDL_GetError();

			throw error;
		}
	}

	inline void setSurfaceColorMod(Uint8 r, Uint8 g, Uint8 b) {

		[[unlikely]] if (SDL_SetSurfaceColorMod(surface, r, g, b)) {

			std::string error = "setSurfaceColorMod failed because: ";

			error += SDL_GetError();

			throw error;
		}
	}

	~SurfaceTexture() {
		if (surface)
			SDL_FreeSurface(surface);
	}
	SDL_Surface* surface;
	SDL_Texture* texture;
	std::string path;
	SDL_Rect destRect;
	uint8_t width;
	uint8_t height;
};

class TextureDictionary {
public:
	static Sprite		  reloadSP(RenderWindow& window, std::string_view path);
	static Sprite		  getSprite(RenderWindow& window, std::string_view path);
	static SurfaceTexture getSurfaceTexture(RenderWindow& window, std::string_view path);
	static SurfaceTexture reloadST(RenderWindow& window, std::string_view path);
	static SpriteSheet	  getSpriteSheet(RenderWindow& window, std::string_view path, uint32_t x, uint32_t y, uint8_t tileSize);
	static SpriteSheet    reloadSS(RenderWindow& window, std::string_view path, uint32_t x, uint32_t y, uint8_t tileSize);

private:
	static Texture* loadTexture(RenderWindow& window, std::string_view p_filePath);
	static inline std::map<std::string, Texture*> textures;
};

#endif