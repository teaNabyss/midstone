#include "Scene0.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_image.h>
#include <MMath.h>
#include <iostream>
#include "Entity.h"
#include "Camera.h"



Scene0::Scene0(SDL_Window* sdlWindow_) :
	window(sdlWindow_)
	, renderer(nullptr)
	, cliff(nullptr)
	, flappy(nullptr)
	, xAxis(30.0f)
	, yAxis(15.0f)
{

}

Scene0::~Scene0(){

}

bool Scene0::OnCreate() {
	// Create a project matrix that moves positions from physics/world space 
	// to screen/pixel space
	int w, h;
	SDL_GetWindowSize(window,&w,&h);

	//Create screen renderer
	renderer = SDL_CreateRenderer(window, NULL);
	if (!renderer) {
		std::cerr << "SDL_Error: " << SDL_GetError() << std::endl;
		return false;
	}
	//Initialize renderer color (black)
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	
	// Create the objects that will be rendered on the screen

	background = new Entity();
	background->pos = Vec3(0.0f, 16.0f, 0.0f);
	background->SetImage("textures/background.png", renderer);
	std::cout << "Backround created" << std::endl;

	//auto wall = new Entity();
	//wall->isStatic = true;
	//wall->pos.set(2.0f, 0.0f, 0.0f);
	//Walls.emplace_back(wall);

	//auto object = new Entity();
	//object->pos.set(0.0f, 10.0f, 0.0f);
	//Objects.emplace_back(object);

	auto platform = new Entity();
	platform->isStatic = true;
	platform->pos.set(14.0f, 5.0f, 0.0f);
	platforms.emplace_back(platform);

	player = new Entity();
	player->pos = Vec3(10.0f, 5.0f, 0.0f);
	player->size = Vec3(2.0f, 3.0f, 0.0f);
	player->SetImage("textures/PurpleMailSprite.png", renderer);
	std::cout << "Player  was created" << std::endl;

	SDL_Init(SDL_INIT_AUDIO);
	MIX_Init();

	mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, nullptr);

	if (!mixer)
	{
		std::cout << "Failed to create mixer: %s\n", SDL_GetError();
		return 0;
	}

	//// Load and play music
	MIX_Audio* Music = MIX_LoadAudio(mixer, "Audio/CrabRave.wav", true);
	MIX_SetMasterGain(mixer, master_volume);
	MIX_PlayAudio(mixer, Music);
	MIX_DestroyAudio(Music);

	camera = new Camera();
	
	return true;
}

void Scene0::OnDestroy() {
	// Clean up the renderer
	if (renderer) {
		SDL_DestroyRenderer(renderer);
		renderer = nullptr;
	}

	//// Turn off audio
	if (mixer)
	{
		MIX_DestroyMixer(mixer);
		MIX_Quit();
	}

	// Delete the objects created on the heap
	// and set to the null pointer just to be safe

	delete player;
	player = nullptr;

}

void Scene0::HandleEvents(const SDL_Event& event)
{
	switch (event.type) {

	default:
		break;
	}
}

void Scene0::Update(const float deltaTime) {

	camera->Follow(player->pos);
}

void Scene0::Render() const {
	SDL_RenderClear(renderer);

	// Convert from world coordinates to pixel coordinates using Scott's magical matrix
	Vec3 screenCoords;
	// Set up sprite's position and size
	SDL_FRect square;

	screenCoords = camera->GetProjectionMatrix() * background->pos;
	square.x = screenCoords.x;
	square.y = screenCoords.y;
	square.w = background->GetSurface()->w * 1.5f;
	square.h = background->GetSurface()->h * 1.5f;
	SDL_RenderTextureRotated(renderer, background->GetTexture(), nullptr, &square, background->angleDeg, nullptr, SDL_FLIP_NONE);

	screenCoords = camera->GetProjectionMatrix() * player->pos;
	// Set up sprite's position and size
	square.x = screenCoords.x;
	square.y = screenCoords.y;
	square.w = player->size.x * camera->GetProjectionMatrix()[0];
	square.h = player->size.y * std::abs(camera->GetProjectionMatrix()[5]);
	square.x -= square.w / 2;
	square.y -= square.h / 2;
	SDL_RenderTextureRotated(renderer, player->GetTexture(), nullptr, &square, player->angleDeg, nullptr, SDL_FLIP_NONE);

	// Update the screen
	SDL_RenderPresent(renderer);
}