#include "Scene2.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_image.h>
#include <MMath.h>
#include <iostream>
#include "Entity.h"



Scene2::Scene2(SDL_Window* sdlWindow_) :
	window(sdlWindow_)
	, renderer(nullptr)
	, player(nullptr)
	, xAxis(30.0f)
	, yAxis(15.0f)
{
	//True outcome
}

Scene2::~Scene2() {

}
bool flipHorizontal = false;
bool Scene2::OnCreate() {
	// Create a project matrix that moves positions from physics/world space 
	// to screen/pixel space
	int w, h;
	SDL_GetWindowSize(window, &w, &h);
	Matrix4 ndc = MMath::viewportNDC(w, h);
	Matrix4 ortho = MMath::orthographic(0.0f, xAxis, 0.0f, yAxis, -1.0f, 1.0f);
	projectionMatrix = ndc * ortho;

	//Create screen renderer
	renderer = SDL_CreateRenderer(window, NULL);
	if (!renderer) {
		std::cerr << "SDL_Error: " << SDL_GetError() << std::endl;
		return false;
	}
	//Initialize renderer color (black)
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);


	// Creates a player that will be rendered on the screen
	player = new Entity();
	player->pos = Vec3(2.0f, 1.0f, 0.0f);
	player->mass = 2.0f;
	player->SetImage("textures/PurpleMailSprite.png", renderer);

	playerScale = 4.0f; //used to scale image, and in collision check\

	SDL_Init(SDL_INIT_AUDIO);
	MIX_Init();

	//TEST
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


	return true;
}

void Scene2::OnDestroy() {
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

void Scene2::HandleEvents(const SDL_Event& event)
{	
	switch (event.type) {
	case SDL_EVENT_KEY_DOWN:
		if (event.key.key == SDLK_A) {
			keyAdown = true;
			flipHorizontal = false;
		}
		if (event.key.key == SDLK_D) {
			keyDdown = true;
			flipHorizontal = true;
		}
		if (event.key.key == SDLK_SPACE) {
			SpaceDown = true;
		}

		break;
	case SDL_EVENT_KEY_UP:
		if (event.key.key == SDLK_A) {
			keyAdown = false;

		}
		if (event.key.key == SDLK_D) {
			keyDdown = false;

		}
		if (event.key.key == SDLK_SPACE) {
			SpaceDown = false;
		}

		break;

	default:
		break;
	}
}

void Scene2::Update(const float deltaTime) {
	if (deltaTime < VERY_SMALL) return;
	

	// Gravity
	Vec3 gravAccel(0.0f, -9.8f, 0.0f);
	Vec3 gravForce = player->mass * gravAccel;
	player->ApplyForce(gravForce);

	// Jump
	float g = 9.8f; //gravity
	float jumpForce = sqrt(2.0f * g * jumpHeight); // how fast will player jump considering height and gravity
	//const float jumpDistance = 4.0f; 

	if (SpaceDown && player->OnGround) {
		player->vel.y = jumpForce;  // pushes up/ jump itsellf
		player->ApplyForce(gravForce); // apllies gravity
		player->OnGround = false;

	}

	// applies x motion on player
	player->xInput(keyAdown, keyDdown);

	// Physics update
	player->Update(deltaTime);

	// Ground collision check to prevent jump in the air
	if (player->pos.y - playerScale / 2 <= 0.0f) {
		player->pos.y = playerScale / 2;  // go back on the ground
		player->vel.y = 0.0f;
		player->OnGround = true;
	}
	else {
		player->OnGround = false;
	}

	// Collision with borders
	player->BorderCollision(playerScale);
}

void Scene2::Render() const {
	SDL_RenderClear(renderer);

	// Convert from world coordinates to pixel coordinates using Scott's magical matrix
	Vec3 screenCoords = projectionMatrix * player->pos;
	// Set up sprite's position and size
	SDL_FRect square;
	square.x = screenCoords.x;
	square.y = screenCoords.y;
	square.w = player->GetSurface()->w / playerScale;
	square.h = player->GetSurface()->h / playerScale;
	square.x -= square.w;
	square.y -= square.h * 0.25f;
	SDL_RenderTextureRotated(renderer, player->GetTexture(), nullptr, &square, player->angleDeg, nullptr, 
		(flipHorizontal) ?SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);

	// Update the screen
	SDL_RenderPresent(renderer);
}