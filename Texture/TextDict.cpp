//not my code
#include "TextDict.hpp"
#include <string_view>

#include "../Window/RenderWindow.hpp"

void SurfaceTexture::load(RenderWindow& window, std::string path, int32_t x, int32_t y, uint8_t tileSize) {

	if (surface)
		SDL_FreeSurface(this->surface);

	if (texture)
		SDL_DestroyTexture(texture);
	(*this) = TextureDictionary::getSurfaceTexture(window, path);
}

void SurfaceTexture::render(RenderWindow& window) {
#ifdef WIN32
	[[unlikely]]
#endif
	// this should never fail on PC
	if (window.render({ 0,0,0,0 }, this->destRect, this->texture))
		return;

	(*this) = TextureDictionary::reloadST(window, this->path);
	window.render({ 0,0,0,0 }, this->destRect, this->texture);
}




void Sprite::load(RenderWindow& window, std::string path, [[maybe_unused]] int32_t x, [[maybe_unused]] int32_t y, [[maybe_unused]] uint8_t tileSize) {
	(*this) = TextureDictionary::getSprite(window, path);
}

void Sprite::render(RenderWindow& window) {
	// this reloading is only neccesary when on android as when you rotate the screen the texture is for some reason not tied to the renderer anymore
#ifdef WIN32
	[[unlikely]]
#endif
	if (window.render(this->srcRect, this->destRect, this->texture->texture))
		return;

	(*this) = TextureDictionary::reloadSP(window, this->texture->path);
	window.render(this->srcRect, this->destRect, this->texture->texture);

}



void SpriteSheet::load(RenderWindow& window, std::string path, int32_t x, int32_t y, uint8_t tileSize) {
	(*this) = TextureDictionary::getSpriteSheet(window, path, x, y, tileSize);
}

void SpriteSheet::render(RenderWindow& window) {
	// this reloading is only neccesary when on android as when you rotate the screen the texture is for some reason not tied to the renderer anymore
#ifdef WIN32
	[[unlikely]]
#endif
	if (window.render(this->srcRect, this->destRect, this->texture->texture))
		return;

	(*this) = TextureDictionary::reloadSS(window, this->texture->path, srcRect.x / tileSize, srcRect.y / tileSize, this->tileSize);
	window.render(this->srcRect, this->destRect, this->texture->texture);
}

void SpriteSheet::renderEx(RenderWindow& window, SDL_RendererFlip flip) {
#ifdef WIN32
	[[unlikely]]
#endif
	if (window.renderEx(this->srcRect, this->destRect, this->texture->texture, this->angle, &this->center, flip))
		return;

	(*this) = TextureDictionary::reloadSS(window, this->texture->path, srcRect.x / tileSize, srcRect.y / tileSize, this->tileSize);
	window.renderEx(this->srcRect, this->destRect, this->texture->texture, this->angle, &this->center, flip);
}

void SpriteSheet::updateSection(uint8_t x, uint8_t y) {
	this->srcRect = { tileSize * x, tileSize * y, tileSize, tileSize };
}




Texture* TextureDictionary::loadTexture(RenderWindow& window, std::string_view p_filePath) {
	return new Texture(window, p_filePath);
}




SurfaceTexture TextureDictionary::getSurfaceTexture(RenderWindow& window, std::string_view path) {
	// no search for if its in the dictionary, as surface textures are user independent
	return SurfaceTexture(window, path.data());
}

SurfaceTexture TextureDictionary::reloadST(RenderWindow& window, std::string_view path) {
	return getSurfaceTexture(window, path);
}




SpriteSheet TextureDictionary::getSpriteSheet(RenderWindow& window, std::string_view path, uint32_t x, uint32_t y, uint8_t tileSize) {

	if (textures.find(path.data()) == textures.end())
	{
		// element not found 
		// this is the first time we encounter this path, generate a new Texture*
		textures[path.data()] = loadTexture(window, path.data());
	}
	return SpriteSheet(x, y, tileSize, textures[path.data()], path.data());
}

SpriteSheet TextureDictionary::reloadSS(RenderWindow& window, std::string_view path, uint32_t x, uint32_t y, uint8_t tileSize) {
	(*textures.at(path.data())) = Texture(window, path);
	return getSpriteSheet(window, path, x, y, tileSize);
}


Sprite TextureDictionary::getSprite(RenderWindow& window, std::string_view path) {

	if (textures.find(path.data()) == textures.end())
	{
		// element not found 
		// this is the first time we encounter this path, generate a new Texture*
		textures[path.data()] = loadTexture(window, path.data());
	}
	//(*textures.at(path.data())) = Texture(window, path);
	return Sprite(window, path.data(), textures[path.data()]);
}

Sprite TextureDictionary::reloadSP(RenderWindow& window, std::string_view path) {
	textures.erase(path.data());
	return getSprite(window, path);
}