#include "Scene3.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_image.h>
#include <MMath.h>
#include <iostream>
#include "Entity.h"
#include "Camera.h"

Scene3::Scene3(SDL_Window* sdlWindow_) :
	window(sdlWindow_)
	, renderer(nullptr)
	, xAxis(30.0f)
	, yAxis(15.0f)
{
	player = nullptr;
	background = nullptr;
	camera = nullptr;
	mixer = nullptr;
	movingLeft = false;
	movingRight = false;
	jumpInput = false;
	jumpStrength = 10.0f;
	gravForce = Vec3(0.0f, -9.8f, 0.0f);
}

Scene3::~Scene3(){

}

bool Scene3::RewindObj(int i) {
	//Stops the object's timer (in the case of recursion, stops a telefraged object's timer IF it has one)
	objects[i]->rewindTimer = 0.0f;
	//Rewinds the object to its original position
	objects[i]->pos = objects[i]->ogPos;
	//Checks if the player is in the rewind location, returning false to kill the player if so
	if (collision.CheckCollision(*player, *objects[i])) {
		return false;
	}
	//Checks to see if any objects are in the rewind location
	for (int j = 0; j < objects.size(); j += 1) {
		//Skips checking collision against itself
		if (j != i && collision.CheckCollision(*objects[j], *objects[i])) {
			//If another object is in the rewind location, rewinds that object as well to reset it to its default position
			if (!RewindObj(j)) {
				return false;
			}
		}
	}
	//Returns true to state the rewind is complete
	return true;
}

bool Scene3::OnCreate() {

	playerDeath = false; //  <~~~~~~~~~~~~~~ PLAYER DEATH SETs TO FALSE HERE

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

	//---------------------BACKGROUND----------------------
	for (int i = 0; i <= 5; i += 1) {
		for (int j = 0; j <= 4; j += 1) {
			background = new Entity();
			background->SetImage("textures/background.png", renderer);
			background->pos = Vec3(15.0f + (29.9f * i), 8.0f + (15.0f * j), 0.0f);
			background->size = Vec3(30.0f, 15.0f, 0.0f);
			backgrounds.emplace_back(background);
		}
	}


	//------------------------PLAYER------------------------
	player = new Entity();
	player->SetImage("textures/PurpleMailSprite.png", renderer);
	player->mass = 5.0f;
	player->size = Vec3(2.0f, 3.0f, 0.0f);
	player->pos = player->ogPos = Vec3(4.0f, 16.0f, 0.0f);
	player->isPlayer = true;
	player->isStatic = false;
	player->onGround = true;

	//------------------------BOXES--------------------------
	auto box1 = new Entity();
	box1->SetImage("textures/Crate.png", renderer);
	box1->mass = 2.0f;
	box1->size = Vec3(2.0f, 2.0f, 0.0f);
	box1->pos = box1->ogPos = Vec3(19.0f, 16.0f, 0.0f);
	box1->isStatic = false;
	box1->rewindTimer = 0.0f;
	box1->rewindMaxTimer = 25.0f;
	objects.emplace_back(box1);
	auto box2 = new Entity();
	box2->SetImage("textures/Crate.png", renderer);
	box2->mass = 2.0f;
	box2->size = Vec3(2.0f, 2.0f, 0.0f);
	box2->pos = box2->ogPos = Vec3(22.0f, 2.0f, 0.0f);
	box2->rewindTimer = 0.0f;
	box2->rewindMaxTimer = 23.0f;
	box2->isStatic = false;
	objects.emplace_back(box2);
	auto box3 = new Entity();
	box3->SetImage("textures/Crate.png", renderer);
	box3->mass = 2.0f;
	box3->size = Vec3(2.0f, 2.0f, 0.0f);
	box3->pos = box3->ogPos = Vec3(4.5f, 8.0f, 0.0f);
	box3->isStatic = false;
	objects.emplace_back(box3);
	auto largeBox1 = new Entity();
	largeBox1->SetImage("textures/Crate.png", renderer);
	largeBox1->mass = 2.0f;
	largeBox1->size = Vec3(4.0f, 4.0f, 0.0f);
	largeBox1->pos = largeBox1->ogPos = Vec3(30.0f, 14.0f, 0.0f);
	largeBox1->isStatic = false;
	objects.emplace_back(largeBox1);
	auto largeBox2 = new Entity();
	largeBox2->SetImage("textures/Crate.png", renderer);
	largeBox2->mass = 2.0f;
	largeBox2->size = Vec3(4.0f, 4.0f, 0.0f);
	largeBox2->pos = largeBox2->ogPos = Vec3(34.0f, 14.0f, 0.0f);
	largeBox2->isStatic = false;
	objects.emplace_back(largeBox2);
	auto decoBox1 = new Entity();
	decoBox1->SetImage("textures/Crate.png", renderer);
	decoBox1->mass = 2.0f;
	decoBox1->size = Vec3(2.0f, 2.0f, 0.0f);
	decoBox1->pos = decoBox1->ogPos = Vec3(18.5f, 22.0f, 0.0f);
	decoBox1->isStatic = false;
	decoBox1->rewindTimer = 0.0f;
	decoBox1->rewindMaxTimer = 18.0f;
	decoBox1->autoRewind = true;
	objects.emplace_back(decoBox1);
	auto decoBox2 = new Entity();
	decoBox2->SetImage("textures/Crate.png", renderer);
	decoBox2->mass = 2.0f;
	decoBox2->size = Vec3(2.0f, 2.0f, 0.0f);
	decoBox2->pos = decoBox2->ogPos = Vec3(21.0f, 21.0f, 0.0f);
	decoBox2->isStatic = false;
	decoBox2->rewindTimer = 0.0f;
	decoBox2->rewindMaxTimer = 11.0f;
	decoBox2->autoRewind = true;
	objects.emplace_back(decoBox2);

	//-------------------PLATFORMS----------------
	auto leftBorder = new Entity();
	leftBorder->SetImage("textures/Wall.png", renderer);
	leftBorder->mass = 100.0f;
	leftBorder->size = Vec3(1.0f, 30.0f, 0.0f);
	leftBorder->pos = Vec3(0.0f, 16.0f, 0.0f);
	leftBorder->isStatic = true;
	platforms.emplace_back(leftBorder);
	auto rightBorder = new Entity();
	rightBorder->SetImage("textures/Wall.png", renderer);
	rightBorder->mass = 100.0f;
	rightBorder->size = Vec3(1.0f, 30.0f, 0.0f);
	rightBorder->pos = Vec3(63.5f, 16.0f, 0.0f);
	rightBorder->isStatic = true;
	platforms.emplace_back(rightBorder);
	auto floor = new Entity();
	floor->SetImage("textures/Platform.png", renderer);
	floor->mass = 100.0f;
	floor->size = Vec3(64.0f, 1.0f, 0.0f);
	floor->pos = Vec3(32.0f, 0.5f, 0.0f);
	floor->isStatic = true;
	platforms.emplace_back(floor);
	auto ceiling = new Entity();
	ceiling->SetImage("textures/Platform.png", renderer);
	ceiling->mass = 100.0f;
	ceiling->size = Vec3(60.0f, 1.0f, 0.0f);
	ceiling->pos = Vec3(30.0f, 30.0f, 0.0f);
	ceiling->isStatic = true;
	platforms.emplace_back(ceiling);
	auto shelfFloor = new Entity();
	shelfFloor->SetImage("textures/Platform.png", renderer);
	shelfFloor->mass = 100.0f;
	shelfFloor->size = Vec3(10.0f, 1.0f, 0.0f);
	shelfFloor->pos = Vec3(20.0f, 19.0f, 0.0f);
	shelfFloor->isStatic = true;
	platforms.emplace_back(shelfFloor);
	auto shelfLeftWall = new Entity();
	shelfLeftWall->SetImage("textures/Wall.png", renderer);
	shelfLeftWall->mass = 100.0f;
	shelfLeftWall->size = Vec3(1.0f, 10.0f, 0.0f);
	shelfLeftWall->pos = Vec3(15.0f, 24.5f, 0.0f);
	shelfLeftWall->isStatic = true;
	platforms.emplace_back(shelfLeftWall);
	auto shelfRightWall = new Entity();
	shelfRightWall->SetImage("textures/Wall.png", renderer);
	shelfRightWall->mass = 100.0f;
	shelfRightWall->size = Vec3(1.0f, 10.0f, 0.0f);
	shelfRightWall->pos = Vec3(25.0f, 24.5f, 0.0f);
	shelfRightWall->isStatic = true;
	platforms.emplace_back(shelfRightWall);
	auto plat1 = new Entity();
	plat1->SetImage("textures/Platform.png", renderer);
	plat1->mass = 100.0f;
	plat1->size = Vec3(19.0f, 1.0f, 0.0f);
	plat1->pos = Vec3(10.0f, 15.0f, 0.0f);
	plat1->isStatic = true;
	platforms.emplace_back(plat1);
	auto plat2 = new Entity();
	plat2->SetImage("textures/Platform.png", renderer);
	plat2->mass = 100.0f;
	plat2->size = Vec3(6.0f, 1.0f, 0.0f);
	plat2->pos = Vec3(3.5f, 7.0f, 0.0f);
	plat2->isStatic = true;
	platforms.emplace_back(plat2);
	auto plat3 = new Entity();
	plat3->SetImage("textures/Platform.png", renderer);
	plat3->mass = 100.0f;
	plat3->size = Vec3(5.0f, 1.0f, 0.0f);
	plat3->pos = Vec3(3.0f, 22.0f, 0.0f);
	plat3->isStatic = true;
	platforms.emplace_back(plat3);
	auto plat4 = new Entity();
	plat4->SetImage("textures/Platform.png", renderer);
	plat4->mass = 100.0f;
	plat4->size = Vec3(5.0f, 1.0f, 0.0f);
	plat4->pos = Vec3(58.5f, 7.0f, 0.0f);
	plat4->isStatic = true;
	platforms.emplace_back(plat4);
	auto plat5 = new Entity();
	plat5->SetImage("textures/Platform.png", renderer);
	plat5->mass = 100.0f;
	plat5->size = Vec3(10.0f, 1.0f, 0.0f);
	plat5->pos = Vec3(35.0f, 10.0f, 0.0f);
	plat5->isStatic = true;
	platforms.emplace_back(plat5);

	// End Game Portal
	//-----------------------PORTAL--------------------------
	portal = new Entity();
	portal->SetImage("textures/PortalDoor.png", renderer);
	portal->mass = 0.0f;
	portal->size = Vec3(3.0f, 3.0f, 0.0f);
	portal->pos = Vec3(3.0f, 23.9f, 0.0f);
	portal->isStatic = true;


	//-----------------------AUDIO---------------------------
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

	//------------------------CAMERA-------------------------
	// 
	camera = new Camera;
	camera->cameraRect = {0,0,30,15};
	camera->pos = Vec3(camera->cameraRect.x, camera->cameraRect.y, 0.0f);
	return true;
}

void Scene3::OnDestroy() {
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

	delete background;
	background = nullptr;

	// Destroy it
	delete camera;
	camera = nullptr;
}

void Scene3::HandleEvents(const SDL_Event& event) {
	switch (event.type) {
	case SDL_EVENT_KEY_DOWN:
		if (event.key.scancode == SDL_SCANCODE_A || event.key.scancode == SDL_SCANCODE_LEFT) {
			movingLeft = true;
			flipHorizontal = false;
		}
		if (event.key.scancode == SDL_SCANCODE_D || event.key.scancode == SDL_SCANCODE_RIGHT) {
			movingRight = true;
			flipHorizontal = true;
		}
		if (event.key.scancode == SDL_SCANCODE_SPACE) {
			jumpInput = true;
		}
		if (event.key.scancode == SDL_SCANCODE_R) {
			playerDeath = true;
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
		if (event.key.scancode == SDL_SCANCODE_R) {
			playerDeath = false;
		}
		break;
	}
}

void Scene3::Update(const float deltaTime) {
	//-----------------MOVEMENT-----------------

	if (movingLeft && !movingRight) {
		player->vel.x = -10.0f;
	}
	else if (movingRight && !movingLeft) {
		player->vel.x = 10.0f;
	}
	else {
		player->vel.x = 0.0f;
	}

	//---------------GROUND-STATES--------------

	//Validates the grounded states of objects by checking their y velocity. Rewind timer logic is also housed in this loop
	for (int i = 0; i < objects.size(); i += 1) {
		//Checks if an object can be rewinded
		if (objects[i]->rewindMaxTimer != -1.0f) {
			//Checks the state of the timer
			//Stops the timer and rewinds the object when the timer goes below 0
			if (objects[i]->rewindTimer < 0.0f) {
				//If a player is teleported on in the process, kill the player
				if (!RewindObj(i)) {
					//TODO: Read below
					std::cout << "Player would die! Implementation to restart the scene is needed!\n";
				}
			}
			//Otherwise, ticks down the timer if it is currently active
			else if (objects[i]->rewindTimer > 0.0f) {
				objects[i]->rewindTimer -= 0.1f;
				//Prints object timer for debug purposes
				std::cout << "Box " << i + 1 << ": " << objects[i]->rewindTimer << "\n";
			}
			//Starts auto rewind timers for any objects set to do so
			if (objects[i]->autoRewind && objects[i]->rewindTimer == 0.0f) {
				objects[i]->rewindTimer = objects[i]->rewindMaxTimer;
			}
		}
		if (objects[i]->vel.y < 0.0f) {
			objects[i]->onGround = false;
		}
	}
	//Validates the grounded state of the player by checking their y velocity
	if (player->vel.y < 0.0f) {
		player->onGround = false;
	}
	//If the player is grounded, jump input is checked and executed if true
	if (player->onGround && jumpInput) {
		player->vel.y += jumpStrength;
		player->onGround = false;
	}

	//-----------------FORCES-------------------

	//TODO: Implement spacial partisioning
	//Applies gravity to the objects
	//AD: I'm not combining this with the collision loop to facilitate using an object's y velocity to tell whether or not it is grounded
	//Checks for collision between the player/objects and platforms
	//Applies gravity to the player
	player->ApplyForce(gravForce * player->mass);
	player->Update(deltaTime);

	//Applies gravity to the objects
	//AD: I'm not combining this with the collision loop to facilitate using an object's y velocity to tell whether or not it is grounded
	for (int i = 0; i < objects.size(); i += 1) {
		objects[i]->ApplyForce(gravForce * objects[i]->mass);
		objects[i]->Update(deltaTime);
	}

	//--------------COLLISSIONS------------------

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
			//Kills the player if collision resolution returns false
			if (!collision.ResolveCollision(*player, *objects[i])) {
				//TODO: Read below
				std::cout << "Player would die! Implementation to restart the scene is needed!\n";
			}
			//If the object can rewind and it's timer hasn't started, starts its timer
			if (objects[i]->rewindMaxTimer != -1.0f && objects[i]->rewindTimer == 0.0f) {
				objects[i]->rewindTimer = objects[i]->rewindMaxTimer;
			}
		}
		for (int j = i + 1; j < objects.size(); j += 1) {
			if (collision.CheckCollision(*objects[j], *objects[i])) {
				collision.ResolveCollision(*objects[j], *objects[i]);
				//If the object can rewind and it's timer hasn't started, starts its timer
				if (objects[i]->rewindMaxTimer != -1.0f && objects[i]->rewindTimer == 0.0f) {
					objects[i]->rewindTimer = objects[i]->rewindMaxTimer;
				}
			}
		}
	}

	//Prints player grounded state for debug purposes
	//std::cout << player->onGround << "\n";

	//----------------CAMEARA---------------------
	//Camera now follows you
	camera->Follow(player->pos);

	//----------------PORTAL----------------------

	if (portal && collision.CheckCollision(*player, *portal)) {
		sceneComplete = true;
	}
}

void Scene3::Render() const {
	SDL_RenderClear(renderer);
	SDL_FRect square;
	Vec3 screenCoords;
	// Renders objects

	//------------------------BACKGROUND-------------------------

	for (int i = 0; i < backgrounds.size(); i += 1) {
		screenCoords = camera->WorldToScreen(backgrounds[i]->pos);
		square.x = screenCoords.x;
		square.y = screenCoords.y;
		square.w = backgrounds[i]->size.x * camera->GetProjectionMatrix()[0];
		square.h = backgrounds[i]->size.y * std::abs(camera->GetProjectionMatrix()[5]);
		square.x -= square.w / 2;
		square.y -= square.h / 2;
		SDL_RenderTextureRotated(renderer, backgrounds[i]->GetTexture(), nullptr, &square, backgrounds[i]->angleDeg, nullptr, SDL_FLIP_NONE);
	}

	//---------------------------BOXES---------------------------

	for (int i = 0; i < objects.size(); i += 1) {
		screenCoords = camera->WorldToScreen(objects[i]->pos);
		square.x = screenCoords.x;
		square.y = screenCoords.y;
		square.w = objects[i]->size.x * camera->GetProjectionMatrix()[0];
		square.h = objects[i]->size.y * std::abs(camera->GetProjectionMatrix()[5]);
		square.x -= square.w / 2;
		square.y -= square.h / 2;
		SDL_RenderTextureRotated(renderer, objects[i]->GetTexture(), nullptr, &square, objects[i]->angleDeg, nullptr, SDL_FLIP_NONE);
	}

	//-------------------------PLATFORMS---------------------------

	for (int i = 0; i < platforms.size(); i += 1) {
		screenCoords = camera->WorldToScreen(platforms[i]->pos);
		square.x = screenCoords.x;
		square.y = screenCoords.y;
		square.w = platforms[i]->size.x * camera->GetProjectionMatrix()[0];
		square.h = platforms[i]->size.y * std::abs(camera->GetProjectionMatrix()[5]);
		square.x -= square.w / 2;
		square.y -= square.h / 2;
		SDL_RenderTextureRotated(renderer, platforms[i]->GetTexture(), nullptr, &square, platforms[i]->angleDeg, nullptr, SDL_FLIP_NONE);
	}

	//---------------------------PORTAL----------------------------

	if (portal) {
		screenCoords = camera->WorldToScreen(portal->pos);
		square.x = screenCoords.x;
		square.y = screenCoords.y;
		square.w = portal->size.x * camera->GetProjectionMatrix()[0];
		square.h = portal->size.y * std::abs(camera->GetProjectionMatrix()[5]);
		square.x -= square.w / 2;
		square.y -= square.h / 2;

		SDL_RenderTextureRotated(renderer, portal->GetTexture(), nullptr, &square, portal->angleDeg, nullptr, SDL_FLIP_NONE);
	}

	//---------------------------PLAYER-----------------------------

	screenCoords = camera->WorldToScreen(player->pos);
	square.x = screenCoords.x;
	square.y = screenCoords.y;
	square.w = player->size.x * camera->GetProjectionMatrix()[0];
	square.h = player->size.y * std::abs(camera->GetProjectionMatrix()[5]);
	square.x -= square.w / 2;
	square.y -= square.h / 2;
	SDL_RenderTextureRotated(renderer, player->GetTexture(), nullptr, &square, player->angleDeg, nullptr,
		(flipHorizontal) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);

	// Update the screen
	SDL_RenderPresent(renderer);
} 