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
void renderTexture(SDL_Texture *texture, SDL_Renderer *renderer, SDL_Rect destination, SDL_Rect *clip);
void renderTexture(SDL_Texture* texture, SDL_Renderer* renderer, int x, int y, SDL_Rect* clip);
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
		"Hello5!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
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
	const string resPath = getResourcePath("Lesson5");

	// The texture we will use
	SDL_Texture *image = loadTexture(resPath + "image.png", renderer);
	if (!image) {
		cleanup(renderer, window);		
		IMG_Quit();
		SDL_Quit();		
		return 5;
	}

	// Draw image
	int iW = 100, iH = 100;

	// Get the image start point position
	int x = SCREEN_WIDTH / 2 - iW / 2;
	int y = SCREEN_HEIGHT / 2 - iH / 2;

	//Setup the clips for our image
	SDL_Rect clips[4];
	for (int i = 0; i < 4; i++) {
		clips[i].x = i / 2 * iW;
		clips[i].y = i % 2 * iH;
		clips[i].w = iW;
		clips[i].h = iH;
	}

	//Specify a default clip to start with
	int useClip = 0;

	// Create Event union
	SDL_Event Events;

	//For tracking if we want to quit
	bool quit = false;

	// The main loop
	while (!quit) {

		//Event Polling
		while (SDL_PollEvent(&Events)) {

			// If user closes the window
			if (Events.type == SDL_QUIT) {
				cout << "Quit button was pressed!" << endl;
				quit = true;
			}

			// Use number input to select which clip should be drawn
			if (Events.type == SDL_KEYDOWN) {				
				switch (Events.key.keysym.sym) {
				case SDLK_1:
				case SDLK_KP_1:
					useClip = 0;
					cout << "1 was pressed!" << endl;
					break;
				case SDLK_2:
				case SDLK_KP_2:
					useClip = 1;
					cout << "2 was pressed!" << endl;
					break;
				case SDLK_3:
				case SDLK_KP_3:
					useClip = 2;
					cout << "3 was pressed!" << endl;
					break;
				case SDLK_4:
				case SDLK_KP_4:
					useClip = 3;
					cout << "4 was pressed!" << endl;
					break;
				case SDLK_ESCAPE:
					quit = true;
					cout << "Escape was pressed!" << endl;
					break;
				default:
					cout << "Any other key was pressed!" << endl;
					break;
				}
			}

			// If user klicks the mouse
			if (Events.type == SDL_MOUSEBUTTONDOWN) {
				cout << "Mouse button was clicked!" << endl;
			}
		}

		// Rendering
		SDL_RenderClear(renderer);

		// Draw the image
		renderTexture(image, renderer, x, y, &clips[useClip]);

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


// Draw an SDL_Texture to an SDL_Renderer at some destination rect
void renderTexture(SDL_Texture *texture, SDL_Renderer *renderer, SDL_Rect destination, SDL_Rect *clip = nullptr) {
	SDL_RenderCopy(renderer, texture, clip, &destination);
}


// Draw an SDL_Texture to an SDL_Renderer at position x, y, preserving
void renderTexture(SDL_Texture *texture, SDL_Renderer *renderer, int x, int y, SDL_Rect *clip = nullptr) {
	
	// Show SDL_Texture on SDL_Renderer in x\y coordinates using scaling
	SDL_Rect destination;
	destination.x = x;
	destination.y = y;
	if (clip != nullptr) {
		destination.w = clip->w;
		destination.h = clip->h;
	}
	else {
		SDL_QueryTexture(texture, NULL, NULL, &destination.w, &destination.h);
	}
	renderTexture(texture, renderer, destination, clip);
}

