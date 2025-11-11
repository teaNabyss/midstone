#include "Levels.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_image.h>
#include <MMath.h>
#include <iostream>
#include "Wall.h"
#include "Camera.h"



Levels::Levels(SDL_Window* sdlWindow_) :
	window(sdlWindow_)
	, renderer(nullptr)
	, xAxis(30.0f)
	, yAxis(15.0f)
{

}

Levels::~Levels() {

}

bool Levels::OnCreate() {
	// Create a project matrix that moves positions from physics/world space 
	// to screen/pixel space
	int w, h;
	SDL_GetWindowSize(window, &w, &h);
	Matrix4 ndc = MMath::viewportNDC(w, h);
	Matrix4 ortho = MMath::orthographic(0.0f, xAxis, 0.0f, yAxis, -1.0f, 1.0f);
	
	camera = new Camera();
	projectionMatrix = camera->GetProjectionMatrix();

	background = new Wall();
	background->pos = Vec3(0.0f, 15.0f, 0.0f);
	background->SetImage("textures/205028.png", renderer);
	std::cout << "Backround created" << std::endl;

	windowBorder = new Wall();
	windowBorder->width = w;
	windowBorder->height = h;
	std::cout << "Window Borders created" << std::endl;


	//Create screen renderer
	renderer = SDL_CreateRenderer(window, NULL);
	if (!renderer) {
		std::cerr << "SDL_Error: " << SDL_GetError() << std::endl;
		return false;
	}
	//Initialize renderer color (black)
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

	return true;
}

void Levels::OnDestroy() {
	// Clean up the renderer
	if (renderer) {
		SDL_DestroyRenderer(renderer);
		renderer = nullptr;
	}
	delete background;
	background = nullptr;


	delete windowBorder;
	windowBorder = nullptr;

	// Delete the objects created on the heap
	// and set to the null pointer just to be safe
}

void Levels::HandleEvents(const SDL_Event& event)
{
	switch (event.type) {
		break;

	default:
		break;
	}
}

void Levels::Update(const float deltaTime) {


}

void Levels::Render() const {
	SDL_RenderClear(renderer);

	// Convert from world coordinates to pixel coordinates using Scott's magical matrix
	Vec3 screenCoords;
	// Set up sprite's position and size
	SDL_FRect square;

	//background
	screenCoords = projectionMatrix * background->pos;
	square.x = screenCoords.x;
	square.y = screenCoords.y;
	square.w = background->GetSurface()->w * 4.0f;
	square.h = background->GetSurface()->h * 4.4f;
	// Display object on the screen
	SDL_RenderTextureRotated(renderer, background->GetTexture(), nullptr, &square, background->angleDeg, nullptr, SDL_FLIP_NONE);

	////windowBorder
	//screenCoords = projectionMatrix * windowBorder->pos;
	//square.x = screenCoords.x;
	//square.y = screenCoords.y;
	//square.w = windowBorder->GetSurface()->w;
	//square.h = windowBorder->GetSurface()->h;
	//SDL_RenderTextureRotated(renderer, windowBorder->GetTexture(), nullptr, &square, windowBorder->angleDeg, nullptr, SDL_FLIP_NONE);

	// Update the screen
	SDL_RenderPresent(renderer);
}