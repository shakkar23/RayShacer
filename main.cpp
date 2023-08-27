// pong.cpp : Defines the entry point for the application.
//
#include "main.hpp"
#include <iostream>

void pressKey(SDL_Scancode scan, moveDirection& player) {
    if (scan == SDL_Scancode::SDL_SCANCODE_W)
        player.Up = true;
    else if (scan == SDL_Scancode::SDL_SCANCODE_S)
        player.Down = true;
    else if (scan == SDL_Scancode::SDL_SCANCODE_A)
        player.Left = true;
    else if (scan == SDL_Scancode::SDL_SCANCODE_D)
        player.Right = true;
    else if(scan == SDL_Scancode::SDL_SCANCODE_SPACE)
		player.Jump = true;
    else if(scan == SDL_Scancode::SDL_SCANCODE_LSHIFT)
		player.Run = true;
}

void unpressKey(SDL_Scancode scan, moveDirection& player) {
    if (scan == SDL_Scancode::SDL_SCANCODE_W)
    {
        player.Up = false;
    }
    else if (scan == SDL_Scancode::SDL_SCANCODE_S)
    {
        player.Down = false;
    }
    else if (scan == SDL_Scancode::SDL_SCANCODE_A)
    {
        player.Left = false;
    }
    else if (scan == SDL_Scancode::SDL_SCANCODE_D)
    {
        player.Right = false;
    }
    else if (scan == SDL_Scancode::SDL_SCANCODE_SPACE)
    {
        player.Jump = false;
    }
    else if (scan == SDL_Scancode::SDL_SCANCODE_ESCAPE) {
        exit(0);
    }
    else if (scan == SDL_Scancode::SDL_SCANCODE_SPACE)
        player.Jump = false;
    else if (scan == SDL_Scancode::SDL_SCANCODE_LSHIFT)
        player.Run = false;
}

std::atomic_int64_t fps = 0;

void fpsThread() {
    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << fps << std::endl;
        if (fps == 0) 
            break;
        else 
            fps = 0;
    }
}


int main(int argc, char* args[]) {

    //std::jthread fps_thread = std::jthread(fpsThread);
    if (SDL_Init(SDL_INIT_VIDEO) != 0)

        ;// std::cout << "HEY.. SDL_Init HAS FAILED. SDL_ERROR: " << SDL_GetError() << std::endl;
    RenderWindow window("RayShacer", 640, 480);
    GameManager.Init(window);
    bool gameRunning = true;
    SDL_Event event;

    bool shouldDisplay = true; // rendering constantly, and not actually displaying causes memory to stack higher and higher until the frames can be shown

    double alpha = 0.0;
    Uint64 last_time = SDL_GetPerformanceCounter();
	
    auto X = 0;
	auto Y = 0;
    moveDirection player;
    moveDirection prevplayer;
    SDL_SetRelativeMouseMode(SDL_TRUE);
    while (gameRunning) {

        while (SDL_PollEvent(&event)) {
            switch (event.type)
            {
            case SDL_QUIT:
				gameRunning = false;
                break;
            case SDL_KEYDOWN:
            case SDL_KEYUP:
                if (event.key.keysym.sym == SDLK_ESCAPE)
                    gameRunning = false;

                if (event.key.state == SDL_PRESSED) {
                    if (event.key.repeat)
                        break;
                    pressKey(event.key.keysym.scancode, player);
                    if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
                        gameRunning = false;
                        break;
                    }
                }
                else {
                    unpressKey(event.key.keysym.scancode, player);
                }
                break;
            case SDL_MOUSEBUTTONDOWN:
                X = event.button.x;
                Y = event.button.y;
                break;
            case SDL_MOUSEMOTION:
				X = event.motion.xrel;
				Y = event.motion.yrel;
				//SDL_Log("%d", X);
				break;
			
            case SDL_FINGERDOWN:
                //SDL_Log("%f", event.tfinger.x);

                window.getWindowSize(X, Y);
                //SDL_Log("%d", X);
                X = event.tfinger.x * (float)X;
                Y = event.tfinger.y * (float)Y;
                break;
            case SDL_WINDOWEVENT:
                switch (event.window.event)
                {
                case SDL_WINDOWEVENT_EXPOSED:
                    shouldDisplay = true;
                    break;
                case SDL_WINDOWEVENT_MINIMIZED:
                    shouldDisplay = false;
                    break;
                case SDL_WINDOWEVENT_FOCUS_GAINED:
                    shouldDisplay = true;
                    break;
                case SDL_WINDOWEVENT_FOCUS_LOST:
                    shouldDisplay = false;
                    break;

                default:
                    break;
                }
					
            default:
                break;
            }

        }

        if (shouldDisplay) { // skip frames that cant be shown due to window not currently accepting frames to display

            const auto now = SDL_GetPerformanceCounter();
            
            alpha += (double)((double)(now - last_time) / SDL_GetPerformanceFrequency() * UPDATES_A_SECOND);
            last_time = now;
            
            while (alpha > 1.0) {
                GameManager.gameLogic(X, Y, player, prevplayer);
				X = 0;
				Y = 0;
                prevplayer = player;
                
                alpha -= 1.0;
            }
            if (gameRunning) {
                GameManager.render(window);
                fps++;
            }
            else {
                gameRunning = false;
            }

        }
        else {
            const auto now = SDL_GetPerformanceCounter();
            alpha = 0;
            last_time = now;
        }
        //window.display();
    }
    window.cleanUp();
    SDL_Quit();
    //std::cin.get();
    return 0;
}
