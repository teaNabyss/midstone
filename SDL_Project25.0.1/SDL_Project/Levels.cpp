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

										/////////////////////////  Background  ///////////////////////////
	background = new Wall();
	background->pos = Vec3(0.0f, 16.0f, 0.0f);
	background->size = Vec3(30.0f, 15.0f , 0.0f );
	background->SetImage("textures/background.png", renderer);
	std::cout << "Backround created" << std::endl;



														// ************************ //
	camera = new Camera();
	camera->SetRenderer(renderer);

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

void Levels::SetRenderer(SDL_Renderer* renderer_) {
	renderer = renderer_;
}

void Levels::Render() const {
	//SDL_RenderClear(renderer);

	// Convert from world coordinates to pixel coordinates using Scott's magical matrix
	//Vec3 screenCoords;
	//// Set up sprite's position and size
	SDL_FRect square;

	//background
	Vec3 screenCoords = projectionMatrix * background->pos;
	square.x = screenCoords.x;
	square.y = screenCoords.y;
	square.w = background->size.x * camera->GetProjectionMatrix()[0];
	square.h = background->size.y * std::abs(camera->GetProjectionMatrix()[5]);
	// Display object on the screen
	SDL_RenderTextureRotated(renderer, background->GetTexture(), nullptr, &square, background->angleDeg, nullptr, SDL_FLIP_NONE);


	//if (background)
	//camera->Render(*background);

 
	//// Update the screen
	//SDL_RenderPresent(renderer);
}