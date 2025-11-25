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

	movingLeft = false;
	movingRight = false;
	jumpInput = false;
	isJumping = false;
	isOnGround = false;


	//auto object = new Entity();
	//object->pos.set(0.0f, 10.0f, 0.0f);
	//Objects.emplace_back(object);

	//auto platform = new Entity();
	//platform->isStatic = true;
	//platform->pos.set(2.0f, 0.0f, 0.0f);
	//platforms.emplace_back(platform);

	//Create a player entity
	player = new Entity();
	player->SetImage("textures/PurpleMailSprite.png", renderer);
	player->mass = 2.0f;
	//TODO: AD: Should we automate size setting via image surface size?
	player->size = Vec3(2.0f, 3.0f, 0.0f);
	player->pos = Vec3(3.0f, 8.0f, 0.0f);
	player->isStatic = false;

	//Creates a box entity and places it below the player
	auto box = new Entity();
	box->SetImage("textures/Crate.png", renderer);
	box->mass = 2.0f;
	box->size = Vec3(2.0f, 2.0f, 0.0f);
	box->pos = Vec3(4.0f, 2.0f, 0.0f);
	//AD: For the sake of testing, the box is currently static
	box->isStatic = true;
	platforms.emplace_back(box);




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

	for (int i = 0; i < platforms.size(); i += 1) {
		delete platforms[i];
		platforms[i] = nullptr;
	}
	
}

void Scene0::HandleEvents(const SDL_Event& event)
{
	switch (event.type) {
		case SDL_EVENT_KEY_DOWN:
			if (event.key.scancode == SDL_SCANCODE_A || event.key.scancode == SDL_SCANCODE_LEFT) {
				movingLeft = true;
			}
			if (event.key.scancode == SDL_SCANCODE_D || event.key.scancode == SDL_SCANCODE_RIGHT) {
				movingRight = true;
			}
			if (event.key.scancode == SDL_SCANCODE_SPACE) {
				jumpInput = true;
			}
			break;
		case SDL_EVENT_KEY_UP:
			if (event.key.scancode == SDL_SCANCODE_A || event.key.scancode == SDL_SCANCODE_LEFT) {
				movingLeft = false;
			}
			if (event.key.scancode == SDL_SCANCODE_D || event.key.scancode == SDL_SCANCODE_RIGHT) {
				movingRight = false;
			}
			if (event.key.scancode == SDL_SCANCODE_SPACE) {
				jumpInput = false;
			}
			break;
	}
}

void Scene0::Update(const float deltaTime) {
	//Movement
	if (movingLeft && !movingRight) {
		player->vel.x = -50.0f;
	}
	else if (movingRight && !movingLeft) {
		player->vel.x = 50.0f;
	}
	else {
		player->vel.x = 0.0f;
	}
	//Applies gravity to the player
	player->ApplyForce(Vec3(0.0f, 9.8f, 0.0f));
	player->Update(deltaTime);
	//Checks for collision between the player and platforms
	//TODO: Implement spacial partisioning
	for (int i = 0; i < platforms.size(); i += 1) {
		if (collision.CheckCollision(*player, *platforms[i])) {
			collision.ResolveCollision(*player, *platforms[i]);
		}
	}

	camera->Follow(player->pos);
}

void Scene0::Render() const {
	SDL_RenderClear(renderer);

	// Convert from world coordinates to pixel coordinates using Scott's magical matrix
	Vec3 screenCoords;
	// Set up sprite's position and size
	SDL_FRect square;

	//TODO: Renders for both player and platforms are inaccurate and purely for rudimentary testing purposes only. Needs proper implementation
	//Renders player
	screenCoords = projectionMatrix * player->pos;
	square.x = screenCoords.x;
	square.y = screenCoords.y;
	square.w = player->GetSurface()->w / 2.0f;
	square.h = player->GetSurface()->h / 2.0f;
	SDL_RenderTextureRotated(renderer, player->GetTexture(), nullptr, &square, player->angleDeg, nullptr, SDL_FLIP_NONE);
	//Renders platforms
	for (int i = 0; i < platforms.size(); i += 1) {
		screenCoords = projectionMatrix * platforms[i]->pos;
		square.x = screenCoords.x;
		square.y = screenCoords.y;
		square.w = platforms[i]->GetSurface()->w * platforms[i]->size.x;
		square.h = platforms[i]->GetSurface()->h * platforms[i]->size.y;
		SDL_RenderTextureRotated(renderer, platforms[i]->GetTexture(), nullptr, &square, platforms[i]->angleDeg, nullptr, SDL_FLIP_NONE);
	}
	
	// Update the screen
	SDL_RenderPresent(renderer);
}