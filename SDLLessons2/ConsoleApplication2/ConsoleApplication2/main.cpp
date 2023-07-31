#include <iostream>
#include <string>
#include "SDL.h"
#include "res_path.h"

using namespace std;


// Set the Window dimentions
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

// Create new Window
SDL_Window* window = nullptr;

// Create render to draw on the Window
SDL_Renderer* renderer = nullptr;

SDL_Texture* LoadImage(string file);
void ApplySurface(int x, int y, SDL_Texture* tex, SDL_Renderer* rend);
int main(int argc, char* argv[]) {

	// Create new subprocess
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		cout << "SDL_Init() Error: " << SDL_GetError() << endl;
		return 1;
	}

	// Initialize Window
	window = SDL_CreateWindow(
		"Hello2!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
		SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN
	);
	if (window == nullptr) {
		cout << "SDL_CreateWindow() Error: " << SDL_GetError() << endl;
		return 2;
	}

	// Initialize render
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == nullptr) {
		cout << "SDL_Renderer() Error: " << SDL_GetError() << endl;
		return 3;
	}

	// Get path to the files
	const string resPath = getResourcePath("Lesson2");

	// Download two images
	SDL_Texture* background = nullptr, * image = nullptr;
	background = LoadImage(resPath + "background.bmp");
	image = LoadImage(resPath + "image.bmp");
	if (background == nullptr || image == nullptr) {
		return 4;
	}

	// Clear the screen
	SDL_RenderClear(renderer);

	// Draw background
	// Getting the texture width and height
	int bW, bH;
	SDL_QueryTexture(background, NULL, NULL, &bW, &bH);

	// Set the picture corners (for four times!!!)
	ApplySurface(0, 0, background, renderer);
	ApplySurface(bW, 0, background, renderer);
	ApplySurface(0, bH, background, renderer);
	ApplySurface(bW, bH, background, renderer);

	// Draw image
	// Getting the texture width and height
	int iW, iH;
	SDL_QueryTexture(image, NULL, NULL, &iW, &iH);

	// Get the image start point position
	int x = SCREEN_WIDTH / 2 - iW / 2;
	int y = SCREEN_HEIGHT / 2 - iH / 2;

	// Set the picture corners
	ApplySurface(x, y, image, renderer);	

	// Show the renovated Window with the new image
	SDL_RenderPresent(renderer);

	std::cout << "Success!!!" << std::endl;

	SDL_Delay(5000);

	// Clean (desroy) everything we created
	SDL_DestroyTexture(background);
	SDL_DestroyTexture(image);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}


// The function of loading an image into the renderer
SDL_Texture* LoadImage(string file) {	
	SDL_Surface *loadedImage = nullptr;	
	SDL_Texture *texture = nullptr;

	// Download the picture into the memory
	loadedImage = SDL_LoadBMP(file.c_str());
	if (loadedImage) {

		// Download the picture into the renderer
		texture = SDL_CreateTextureFromSurface(renderer, loadedImage);
		SDL_FreeSurface(loadedImage);
	}
	else {
		cout << "Error: " << SDL_GetError() << endl;
	}
	return texture;
}


// The picture position setting function on screen
void ApplySurface(int x, int y, SDL_Texture* texture, SDL_Renderer* renderer) {

	// Set the image position
	SDL_Rect position;
	position.x = x;
	position.y = y;
	SDL_QueryTexture(texture, NULL, NULL, &position.w, &position.h);
	SDL_RenderCopy(renderer, texture, NULL, &position);
}