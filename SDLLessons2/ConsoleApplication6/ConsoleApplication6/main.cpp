#include <iostream>
#include <string>
#include "SDL.h"
#include "SDL_ttf.h"
#include "res_path.h"
#include "cleanup.h"

using namespace std;


// Set the Window dimentions
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

void logSDLError(ostream &os, const string &msg);
void renderTexture(SDL_Texture* tex, SDL_Renderer* ren, SDL_Rect dst, SDL_Rect* clip);
void renderTexture(SDL_Texture* tex, SDL_Renderer* ren, int x, int y, SDL_Rect* clip);
SDL_Texture* renderText(const std::string& message, const std::string& fontFile, SDL_Color color,
	int fontSize, SDL_Renderer* renderer);
int main(int, char **) {

	// Create new subprocess
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		logSDLError(cout, "SDL_Init{}");
		return 1;
	}

	//Also need to init SDL_ttf
	if (TTF_Init() != 0) {
		logSDLError(std::cout, "TTF_Init");
		SDL_Quit();
		return 2;
	}

	// Initialize Window
	SDL_Window *window = SDL_CreateWindow(
		"Hello6!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN
	);
	if (window == nullptr) {
		logSDLError(cout, "SDL_CreateWindow()");
		TTF_Quit();
		SDL_Quit();
		return 3;
	}

	// Initialize render
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == nullptr) {
		logSDLError(cout, "SDL_CreateRenderer()");
		cleanup(window);
		TTF_Quit();
		SDL_Quit();
		return 4;
	}

	// Get path to the files
	const string resPath = getResourcePath("Lesson6");

	//We'll render the string "TTF fonts are cool!" in white
	//Color is in RGB format
	SDL_Color color = { 255, 255, 255, 255 };
	SDL_Texture* image = renderText("TTF fonts are cool!", resPath + "sample.ttf", color, 64, renderer);
	if (image == nullptr) {
		cleanup(image, renderer, window);
		TTF_Quit();
		SDL_Quit();
		return 5;
	}

	// Draw image
	int iW, iH;

	// Get texture dimentions
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

		//Event Polling
		while (SDL_PollEvent(&Events)) {

			// If user closes the window
			if (Events.type == SDL_QUIT) {
				cout << "Quit button was pressed!" << endl;
				quit = true;
			}

			//If any key was pressed
			if (Events.type == SDL_KEYDOWN && Events.key.keysym.sym == SDLK_ESCAPE) {
				cout << "The escape button was pressed!" << endl;
				quit = true;
			}
		}

		// Rendering
		SDL_RenderClear(renderer);

		//We can draw our message as we do any other texture, since it's been
		//rendered to a texture
		renderTexture(image, renderer, x, y, nullptr);

		// Show the renovated Window with the new image
		SDL_RenderPresent(renderer);

		SDL_Delay(10);
	}

	// Clean (desroy) everything we created
	cleanup(image, renderer, window);
	TTF_Quit();
	SDL_Quit();	
	return 0;
}


// The logging errors function
void logSDLError(ostream &os, const string &msg) {
	os << msg << " error: " << SDL_GetError() << endl;
}


// Draw an SDL_Texture to an SDL_Renderer at some destination rect 
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, SDL_Rect dst, SDL_Rect *clip = nullptr) {
	SDL_RenderCopy(ren, tex, clip, &dst);
}


// Draw an SDL_Texture to an SDL_Renderer at position x, y, preserving
void renderTexture(SDL_Texture* tex, SDL_Renderer* ren, int x, int y, SDL_Rect* clip = nullptr) {
	SDL_Rect dst;
	dst.x = x;
	dst.y = y;
	if (clip != nullptr) {
		dst.w = clip->w;
		dst.h = clip->h;
	}
	else {
		SDL_QueryTexture(tex, NULL, NULL, &dst.w, &dst.h);
	}
	renderTexture(tex, ren, dst, clip);
}


// Render the message we want to display to a texture for drawing
SDL_Texture* renderText(const std::string& message, const std::string& fontFile, SDL_Color color,
	int fontSize, SDL_Renderer* renderer)
{
	//Open the font
	TTF_Font* font = TTF_OpenFont(fontFile.c_str(), fontSize);
	if (font == nullptr) {
		logSDLError(std::cout, "TTF_OpenFont");
		return nullptr;
	}
	//We need to first render to a surface as that's what TTF_RenderText returns, then
	//load that surface into a texture
	SDL_Surface* surf = TTF_RenderText_Blended(font, message.c_str(), color);
	if (surf == nullptr) {
		TTF_CloseFont(font);
		logSDLError(std::cout, "TTF_RenderText");
		return nullptr;
	}
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surf);
	if (texture == nullptr) {
		logSDLError(std::cout, "CreateTexture");
	}
	//Clean up the surface and font
	SDL_FreeSurface(surf);
	TTF_CloseFont(font);
	return texture;
}

