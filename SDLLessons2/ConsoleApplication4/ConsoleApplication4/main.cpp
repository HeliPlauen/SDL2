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
	SDL_Window *window = SDL_CreateWindow(
		"Hello4!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN
	);
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
	const string resPath = getResourcePath("Lesson4");

	// The texture we will use
	SDL_Texture *image = loadTexture(resPath + "image.png", renderer);
	if (!image) {
		cleanup(renderer, window);		
		IMG_Quit();
		SDL_Quit();		
		return 5;
	}

	// Draw image
	// Getting the texture width and height
	int iW, iH;
	SDL_QueryTexture(image, NULL, NULL, &iW, &iH);

	// Get the image start point position
	int x = SCREEN_WIDTH / 2 - iW / 2;
	int y = SCREEN_HEIGHT / 2 - iH / 2;

	// Create Event union
	SDL_Event Events;

	//For tracking if we want to quit
	bool quit = false;

	// The main loop
	while (!quit) {

		while (SDL_PollEvent(&Events)) {

			// If user closes the window
			if (Events.type == SDL_QUIT) {
				cout << "Quit button was pressed!" << endl;
				quit = true;
			}

			// If user presses any key
			if (Events.type == SDL_KEYDOWN) {
				cout << "Any key was pressed!" << endl;
				quit = true;
			}

			// If user klicks the mouse
			if (Events.type == SDL_MOUSEBUTTONDOWN) {
				cout << "Mouse button was clicked!" << endl;
				quit = true;
			}
		}

		// Rendering
		SDL_RenderClear(renderer);

		// Draw the image
		renderTexture(image, renderer, x, y);

		// Show the renovated Window with the new image
		SDL_RenderPresent(renderer);

		SDL_Delay(10);
	}

	// Clean (desroy) everything we created
	cleanup(image, renderer, window);
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


