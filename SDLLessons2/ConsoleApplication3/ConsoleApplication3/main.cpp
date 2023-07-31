#include <iostream>
#include <string>
#include "SDL.h"
#include "SDL_image.h"
#include "res_path.h"
#include "cleanup.h"

using namespace std;


// Set the Window dimentions
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

// We'll be scaling our tiles to be 40x40
const int TILE_SIZE = 40;

void logSDLError(ostream &os, const string &msg);
SDL_Texture *loadTexture(const string &file, SDL_Renderer *renderer);
void renderTexture(SDL_Texture *texture, SDL_Renderer *renderer, int x, int y, int w, int h);
void renderTexture(SDL_Texture *texture, SDL_Renderer *renderer, int x, int y);
int main(int argc, char *argv[]) {

	// Create new subprocess
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		logSDLError(cout, "SDL_Init{}");
		return 1;
	}

	// Initialize image subsystem
	if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG) {
		logSDLError(cout, "IMG_Init()");
		SDL_Quit();
		return 2;
	}

	// Initialize Window
	SDL_Window *window = SDL_CreateWindow("Hello3!", 100, 100, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (window == nullptr) {
		logSDLError(cout, "SDL_CreateWindow()");
		IMG_Quit();
		SDL_Quit();
		return 3;
	}

	// Initialize render
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == nullptr) {
		logSDLError(cout, "SDL_CreateRenderer()");
		cleanup(window);
		IMG_Quit();
		SDL_Quit();
		return 4;
	}

	// Get path to the files
	const string resPath = getResourcePath("Lesson3");

	// The textures we will use
	SDL_Texture *background = loadTexture(resPath + "background.png", renderer);
	SDL_Texture *image = loadTexture(resPath + "image.png", renderer);
	if (!background || !image) {
		cout << endl;

		// Clean (desroy) everything we created
		//SDL_DestroyTexture(background);
		//SDL_DestroyTexture(image);
		//SDL_DestroyRenderer(renderer);
		//SDL_DestroyWindow(window);
		 
		// Clean (desroy) everything we created
		if (background) {
			cout << "Image was not downloaded!!!" << endl;
			cleanup(background, renderer, window);
		}
		else if (image) {
			cout << "Background was not downloaded!!!" << endl;
			cleanup(image, renderer, window);
		}
		else {
			cout << "NOTHING was downloaded!!!" << endl;
			cleanup(renderer, window);
		}
		IMG_Quit();
		SDL_Quit();		
		return 5;
	}

	// A sleepy rendering loop, wait for 5 seconds and render and present the screen each time
	for (int i = 0; i < 5; ++i) {

		// Rendering
		SDL_RenderClear(renderer);

		// Number of tiles
		int xTiles = SCREEN_WIDTH / TILE_SIZE;
		int yTiles = SCREEN_HEIGHT / TILE_SIZE;

		// Filling background with tiles
		for (int i = 0; i < xTiles * yTiles; ++i)
		{
			int x = i % xTiles;
			int y = i / xTiles;
			renderTexture(background, renderer, x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE);
		}

		// Draw image
		// Getting the texture width and height
		int iW, iH;
		SDL_QueryTexture(image, NULL, NULL, &iW, &iH);

		// Get the image start point position
		int x = SCREEN_WIDTH / 2 - iW / 2;
		int y = SCREEN_HEIGHT / 2 - iH / 2;
		renderTexture(image, renderer, x, y);

		// Show the renovated Window with the new image
		SDL_RenderPresent(renderer);

		std::cout << "Success!!!" << std::endl;

		SDL_Delay(1000);
	}

	// Clean (desroy) everything we created
	cleanup(background, image, renderer, window);

	// The possible cleaning way is 
	//SDL_DestroyTexture(background);
	//SDL_DestroyTexture(image);
	//SDL_DestroyRenderer(renderer);
	//SDL_DestroyWindow(window);

	IMG_Quit();
	SDL_Quit();	
	return 0;
}


// The logging errors function
void logSDLError(ostream &os, const string &msg) {
	os << msg << " error: " << SDL_GetError() << endl;
}


// The loading image function into the texture 
SDL_Texture *loadTexture(const string &file, SDL_Renderer *renderer) {

	// Load image into the texture
	SDL_Texture *texture = IMG_LoadTexture(renderer, file.c_str());
	if (!texture) {
		cout << IMG_GetError();
	}
	return texture;
}


// The function showing SDL_Texture on SDL_Renderer with scaling 
void renderTexture(SDL_Texture *texture, SDL_Renderer *renderer, int x, int y, int w, int h) {

	// Show SDL_Texture on SDL_Renderer in x\y coordinates using scaling
	SDL_Rect destination;
	destination.x = x;
	destination.y = y;
	destination.w = w;
	destination.h = h;
	SDL_RenderCopy(renderer, texture, NULL, &destination);
}


// The function showing SDL_Texture on SDL_Renderer with NO scaling 
void renderTexture(SDL_Texture *texture, SDL_Renderer *renderer, int x, int y) {
	int w, h;

	// Show SDL_Texture on SDL_Renderer in x\y coordinates with NO scaling
	// Getting w and h from texture
	SDL_QueryTexture(texture, NULL, NULL, &w, &h);
	renderTexture(texture, renderer, x, y, w, h);
}


