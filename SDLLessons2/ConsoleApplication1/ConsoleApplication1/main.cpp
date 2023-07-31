#include <iostream>
#include "SDL.h"

using namespace std;


int main(int argc, char* argv[]) {

	// Create new subprocess
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		cout << "SDL_Init() Error: " << SDL_GetError() << endl;
		return 1;
	}

	// Create new Window
	SDL_Window *window = SDL_CreateWindow("Hello!", 100, 100, 640, 640, SDL_WINDOW_SHOWN);
	if (window == nullptr) {
		cout << "SDL_CreateWindow() Error: " << SDL_GetError() << endl;
		return 1;
	}

	// Create render to draw on the Window
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == nullptr) {
		cout << "SDL_Renderer() Error: " << SDL_GetError() << endl;
		return 1;
	}

	// Download into the memory picture C:\YPROGRAMMING\SDLLessons2\ConsoleApplication1\ConsoleApplication1\hello.bmp
	SDL_Surface *bmp = SDL_LoadBMP("hello.bmp");
	if (bmp == nullptr) {
		cout << "SDL_LoadBMP() Error: " << SDL_GetError() << endl;
		return 1;
	}

	// Download picture into the renderer
	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, bmp);

	// We do not need it any more
	SDL_FreeSurface(bmp);

	if (texture == nullptr) {
		cout << "SDL_CreateTextureFromSurface() Error: " << SDL_GetError() << endl;
		return 1;
	}

	// Clear renderer
	SDL_RenderClear(renderer);

	// Render the texture
	SDL_RenderCopy(renderer, texture, NULL, NULL);

	// Show the renovated Window with the new image
	SDL_RenderPresent(renderer);	

	std::cout << "Success!!!" << std::endl;

	SDL_Delay(5000);

	// Clean (desroy) everything we created
	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}