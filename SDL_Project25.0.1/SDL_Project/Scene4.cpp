#include "Scene4.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_image.h>
#include <MMath.h>
#include <iostream>
#include "Entity.h"
#include "Camera.h"

Scene4::Scene4(SDL_Window* sdlWindow_) :
	window(sdlWindow_)
	, renderer(nullptr)
	, xAxis(30.0f)
	, yAxis(15.0f)
{

}

Scene4::~Scene4(){

}

bool Scene4::OnCreate() {
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
	
	movingLeft = false;
	movingRight = false;
	jumpInput = false;
	isJumping = false;
	isOnGround = false;

	// Create the objects that will be rendered on the screen
	background = new Entity();
	background->pos = Vec3(0.0f, 16.0f, 0.0f);
	background->SetImage("textures/background.png", renderer);
	std::cout << "Backround created" << std::endl;
	//Create a player entity
	player = new Entity();
	player->SetImage("textures/PurpleMailSprite.png", renderer);
	player->mass = 2.0f;
	player->size = Vec3(2.0f, 3.0f, 0.0f);
	player->pos = Vec3(4.0f, 8.0f, 0.0f);
	player->isStatic = false;
	//Creates a box entity and places it below the player
	auto box1 = new Entity();
	box1->SetImage("textures/Crate.png", renderer);
	box1->mass = 2.0f;
	box1->size = Vec3(2.0f, 2.0f, 0.0f);
	box1->pos = Vec3(4.0f, 2.0f, 0.0f);
	//AD: For the sake of testing, the box is currently static
	box1->isStatic = true;
	platforms.emplace_back(box1);
	//Another box, now on a floating platform
	auto box2 = new Entity();
	box2->SetImage("textures/Crate.png", renderer);
	box2->mass = 2.0f;
	box2->size = Vec3(2.0f, 2.0f, 0.0f);
	box2->pos = Vec3(20.0f, 7.0f, 0.0f);
	box2->isStatic = false;
	objects.emplace_back(box2);
	//Test level platforms
	auto plat1 = new Entity();
	plat1->SetImage("textures/Crate.png", renderer);
	plat1->mass = 100.0f;
	plat1->size = Vec3(30.0f, 1.0f, 0.0f);
	plat1->pos = Vec3(15.0f, 0.5f, 0.0f);
	plat1->isStatic = true;
	platforms.emplace_back(plat1);
	auto plat2 = new Entity();
	plat2->SetImage("textures/Crate.png", renderer);
	plat2->mass = 100.0f;
	plat2->size = Vec3(5.0f, 1.0f, 0.0f);
	plat2->pos = Vec3(20.0f, 5.0f, 0.0f);
	plat2->isStatic = true;
	platforms.emplace_back(plat2);
	auto plat3 = new Entity();
	plat3->SetImage("textures/Crate.png", renderer);
	plat3->mass = 100.0f;
	plat3->size = Vec3(1.0f, 6.0f, 0.0f);
	plat3->pos = Vec3(30.0f, 4.0f, 0.0f);
	plat3->isStatic = true;
	platforms.emplace_back(plat3);

	// End Game Portal

	portal = new Entity();
	portal->SetImage("textures/PurplePortal.png", renderer);
	portal->mass = 0.0f;
	portal->size = Vec3(2.0f, 3.0f, 0.0f);
	portal->pos = Vec3(28.0f, 5.0f, 0.0f);
	portal->isStatic = true;

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

void Scene4::OnDestroy() {
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

	for (int i = 0; i < objects.size(); i += 1) {
		delete objects[i];
		objects[i] = nullptr;
	}

	for (int i = 0; i < platforms.size(); i += 1) {
		delete platforms[i];
		platforms[i] = nullptr;
	}
	delete portal;
	portal = nullptr;
	
}

void Scene4::HandleEvents(const SDL_Event& event) {
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

void Scene4::Update(const float deltaTime) {
	//Movement
	if (movingLeft && !movingRight) {
		player->vel.x = -10.0f;
	}
	else if (movingRight && !movingLeft) {
		player->vel.x = 10.0f;
	}
	else {
		player->vel.x = 0.0f;
	}
	//Applies gravity to the player
	player->ApplyForce(Vec3(0.0f, -9.8f, 0.0f));
	player->Update(deltaTime);
	//TODO: Implement spacial partisioning
	//Applies gravity to the objects
	//AD: I'm not combining this with the collision loop to facilitate using an object's y velocity to tell whether or not 
	for (int i = 0; i < objects.size(); i += 1) {
		objects[i]->ApplyForce(Vec3(0.0f, -9.8f, 0.0f));
		objects[i]->Update(deltaTime);
	}
	//Checks for collision between the player/objects and platforms
	for (int i = 0; i < platforms.size(); i += 1) {
		if (collision.CheckCollision(*player, *platforms[i])) {
			collision.ResolveCollision(*player, *platforms[i]);
		}
		for (int j = 0; j < objects.size(); j += 1) {
			if (collision.CheckCollision(*objects[j], *platforms[i])) {
				collision.ResolveCollision(*objects[j], *platforms[i]);
			}
		}
	}
	//Checks for collision between the player/objects and objects
	for (int i = 0; i < objects.size(); i += 1) {
		if (collision.CheckCollision(*player, *objects[i])) {
			collision.ResolveCollision(*player, *objects[i]);
		}
		for (int j = i + 1; j < objects.size(); j += 1) {
			if (collision.CheckCollision(*objects[j], *platforms[i])) {
				collision.ResolveCollision(*objects[j], *platforms[i]);
			}
		}
	}
	//Camera stuff
	camera->Follow(player->pos);

	if (portal && collision.CheckCollision(*player, *portal)) {
		sceneComplete = true;
	}
}

void Scene4::Render() const {
	SDL_RenderClear(renderer);
	// Convert from world coordinates to pixel coordinates using Scott's magical matrix
	Vec3 screenCoords;
	// Set up sprite's position and size
	SDL_FRect square;
	//Renders player
	screenCoords = camera->GetProjectionMatrix() * player->pos;
	square.x = screenCoords.x;
	square.y = screenCoords.y;
	square.w = player->size.x * camera->GetProjectionMatrix()[0];
	square.h = player->size.y * std::abs(camera->GetProjectionMatrix()[5]);
	square.x -= square.w / 2;
	square.y -= square.h / 2;
	SDL_RenderTextureRotated(renderer, player->GetTexture(), nullptr, &square, player->angleDeg, nullptr, SDL_FLIP_NONE);
	//Renders objects
	for (int i = 0; i < objects.size(); i += 1) {
		screenCoords = camera->GetProjectionMatrix() * objects[i]->pos;
		square.x = screenCoords.x;
		square.y = screenCoords.y;
		square.w = objects[i]->size.x * camera->GetProjectionMatrix()[0];
		square.h = objects[i]->size.y * std::abs(camera->GetProjectionMatrix()[5]);
		square.x -= square.w / 2;
		square.y -= square.h / 2;
		SDL_RenderTextureRotated(renderer, objects[i]->GetTexture(), nullptr, &square, objects[i]->angleDeg, nullptr, SDL_FLIP_NONE);
	}
	//Renders platforms
	for (int i = 0; i < platforms.size(); i += 1) {
		screenCoords = camera->GetProjectionMatrix() * platforms[i]->pos;
		square.x = screenCoords.x;
		square.y = screenCoords.y;
		square.w = platforms[i]->size.x * camera->GetProjectionMatrix()[0];
		square.h = platforms[i]->size.y * std::abs(camera->GetProjectionMatrix()[5]);
		square.x -= square.w / 2;
		square.y -= square.h / 2;
		SDL_RenderTextureRotated(renderer, platforms[i]->GetTexture(), nullptr, &square, platforms[i]->angleDeg, nullptr, SDL_FLIP_NONE);
	}
	if (portal) {
		screenCoords = camera->GetProjectionMatrix() * portal->pos;
		square.x = screenCoords.x;
		square.y = screenCoords.y;
		square.w = portal->size.x * camera->GetProjectionMatrix()[0];
		square.h = portal->size.y * std::abs(camera->GetProjectionMatrix()[5]);
		square.x -= square.w / 2;
		square.y -= square.h / 2;

		SDL_RenderTextureRotated(renderer, portal->GetTexture(), nullptr, &square, portal->angleDeg, nullptr, SDL_FLIP_NONE);
	}
	// Update the screen
	SDL_RenderPresent(renderer);
}