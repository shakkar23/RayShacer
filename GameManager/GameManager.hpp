#pragma once
#include "../Window/RenderWindow.hpp"
#include <thread>
#include <array>
#include <vector>
#include "../Button/Button.hpp"
#include "../World/World.hpp"
#include <SDL_ttf.h>

#include "../Utility/vec2.hpp"
#include "../Utility/vec3.hpp"
#include "../Utility/inputs.hpp"
#include "../Utility/Color.hpp"
#include "../Texture/TextDict.hpp"



// this is the gameManager, it redirects the thread to wherever it needs to go
class gameManager
{
public:
	void Init(RenderWindow& window);
	void gameLogic(int x, int y, moveDirection prev, moveDirection curr);
	void render(RenderWindow& window);

private:
	ShakColor RenderPixel(const vec2 coord)const;

	int winSizeX{};
	int winSizeY{};
	
	float playerX = 4.5;
	float playerY = 4.5;
	float playerZ = 4.5;

	float playerYaw = 0;
	float playerPitch = 0;
	
	World world;

	std::vector<int> horizontal_iterator;
	std::vector<int> vertical_iter;

	Sprite grass;
};
extern gameManager GameManager;
