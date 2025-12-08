#include "Scene1.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_image.h>
#include <MMath.h>
#include <iostream>
#include "Entity.h"
#include "Camera.h"

Scene1::Scene1(SDL_Window* sdlWindow_) :
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

Scene1::~Scene1() {

}

bool Scene1::RewindObj(int i) {
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

bool Scene1::OnCreate() {

	playerDeath = false; //  <~~~~~~~~~~~~~~ PLAYER DEATH SETs TO FALSE HERE

	// Create a project matrix that moves positions from physics/world space 
	// to screen/pixel space
	int w, h;
	SDL_GetWindowSize(window, &w, &h);

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
	background = new Entity();
	background->SetImage("textures/background.png", renderer);
	background->pos = Vec3(15.0f, 7.1f, 0.0f);
	background->size = Vec3(30.0f, 15.0f, 0.0f);
	backgrounds.emplace_back(background);
	std::cout << "Background created" << std::endl;

	int numOfBackgrounds = 2;
	for (int index = 1; index < numOfBackgrounds; index++) {
		std::cout << "Background Index: " << index << std::endl;
		OtherBackground = new Entity();
		OtherBackground->SetImage("textures/background.png", renderer);
		OtherBackground->pos = Vec3(backgrounds.back()->pos.x + 29.9f, 7.1f, 0.0f);
		OtherBackground->size = Vec3(30.0f, 15.0f, 0.0f);
		backgrounds.emplace_back(OtherBackground);
		std::cout << "Background created" << std::endl;
	}


	//------------------------PLAYER------------------------
	player = new Entity();
	player->SetImage("textures/PurpleMailSprite.png", renderer);
	player->mass = 10.0f;
	player->size = Vec3(2.0f, 3.0f, 0.0f);
	player->pos = player->ogPos = Vec3(2.0f, 2.0f, 0.0f);
	player->isPlayer = true;
	player->isStatic = false;
	player->onGround = true;

	//------------------------BOXES--------------------------
	auto box1 = new Entity();
	box1->SetImage("textures/Crate.png", renderer);
	box1->mass = 2.0f;
	box1->size = Vec3(2.0f, 2.0f, 0.0f);
	box1->pos = box1->ogPos = Vec3(10.0f, 10.0f, 0.0f);
	//AD: Be warned that putting a static entity into the object array applies gravity to a static object, causing collisons to behave irregularly
	box1->isStatic = false;
	objects.emplace_back(box1);
	//Another box, now on a floating platform

	auto DecorBox1 = new Entity();
	DecorBox1->SetImage("textures/Crate.png", renderer);
	DecorBox1->mass = 2.0f;
	DecorBox1->size = Vec3(1.0f, 1.0f, 0.0f);
	DecorBox1->pos = DecorBox1->ogPos = Vec3(18.5f, 14.5f, 0.0f);
	DecorBox1->isStatic = false;
	objects.emplace_back(DecorBox1);
	auto DecorBox2 = new Entity();
	DecorBox2->SetImage("textures/Crate.png", renderer);
	DecorBox2->mass = 2.0f;
	DecorBox2->size = Vec3(1.0f, 1.0f, 0.0f);
	DecorBox2->pos = DecorBox2->ogPos = Vec3(19.5f, 14.5f, 0.0f);
	DecorBox2->isStatic = false;
	objects.emplace_back(DecorBox2);
	auto DecorBox3 = new Entity();
	DecorBox3->SetImage("textures/Crate.png", renderer);
	DecorBox3->mass = 2.0f;
	DecorBox3->size = Vec3(1.0f, 1.0f, 0.0f);
	DecorBox3->pos = DecorBox3->ogPos = Vec3(20.0f, 14.5f, 0.0f);
	DecorBox3->isStatic = false;
	objects.emplace_back(DecorBox3);

	auto DecorBox4 = new Entity();
	DecorBox4->SetImage("textures/crates2.png", renderer);
	DecorBox4->mass = 2.0f;
	DecorBox4->size = Vec3(2.0f, 2.0f, 0.0f);
	DecorBox4->pos = DecorBox4->ogPos = Vec3(1.3f, 11.5f, 0.0f);
	DecorBox4->isStatic = false;
	objects.emplace_back(DecorBox4);
	auto DecorBox5 = new Entity();
	DecorBox5->SetImage("textures/crates2.png", renderer);
	DecorBox5->mass = 2.0f;
	DecorBox5->size = Vec3(2.0f, 2.0f, 0.0f);
	DecorBox5->pos = DecorBox5->ogPos = Vec3(2.3f, 11.5f, 0.0f);
	DecorBox5->isStatic = false;
	objects.emplace_back(DecorBox5);

	//-------------------Level platforms----------------

	//_____FLOOR__WALLS__CEILING_____
	auto floor1 = new Entity();
	floor1->SetImage("textures/Wall.png", renderer);
	floor1->mass = 100.0f;
	floor1->size = Vec3(60.0f, 1.0f, 0.0f);
	floor1->pos = Vec3(30.0f, 0.0f, 0.0f);
	floor1->isStatic = true;
	platforms.emplace_back(floor1);
	auto floor2 = new Entity();
	floor2->SetImage("textures/Wall.png", renderer);
	floor2->mass = 100.0f;
	floor2->size = Vec3(2.0f, 12.0f, 0.0f);
	floor2->pos = Vec3(30.0f, 0.0f, 0.0f);
	floor2->isStatic = true;
	platforms.emplace_back(floor2);
	auto leftWall = new Entity();
	leftWall->SetImage("textures/Wall.png", renderer);
	leftWall->mass = 100.0f;
	leftWall->size = Vec3(1.0f, 15.0f, 0.0f);
	leftWall->pos = Vec3(0.0f, 7.5f, 0.0f);
	leftWall->isStatic = true;
	platforms.emplace_back(leftWall);
	auto rightWall = new Entity();
	rightWall->SetImage("textures/Wall.png", renderer);
	rightWall->mass = 100.0f;
	rightWall->size = Vec3(1.0f, 15.0f, 0.0f);
	rightWall->pos = Vec3(39.0f, 7.5f, 0.0f);
	rightWall->isStatic = true;
	platforms.emplace_back(rightWall);
	auto ceiling = new Entity();
	ceiling->SetImage("textures/Wall.png", renderer);
	ceiling->mass = 100.0f;
	ceiling->size = Vec3(60.0f, 1.0f, 0.0f);
	ceiling->pos = Vec3(30.0f, 15.0f, 0.0f);
	ceiling->isStatic = true;
	platforms.emplace_back(ceiling);

	auto platform1 = new Entity();
	platform1->SetImage("textures/platform.png", renderer);
	platform1->mass = 100.0f;
	platform1->size = Vec3(3.0f, 1.0f, 0.0f);
	platform1->pos = Vec3(5.0f, 5.0f, 0.0f);
	platform1->isStatic = true;
	platforms.emplace_back(platform1);
	auto platform2 = new Entity();
	platform2->SetImage("textures/platform.png", renderer);
	platform2->mass = 100.0f;
	platform2->size = Vec3(4.0f, 1.0f, 0.0f);
	platform2->pos = Vec3(10.0f, 9.0f, 0.0f);
	platform2->isStatic = true;
	platforms.emplace_back(platform2);
	auto platform3 = new Entity();
	platform3->SetImage("textures/platform.png", renderer);
	platform3->mass = 100.0f;
	platform3->size = Vec3(5.0f, 1.0f, 0.0f);
	platform3->pos = Vec3(36.0f, 9.5f, 0.0f);
	platform3->isStatic = true;
	platforms.emplace_back(platform3);
	auto Decor1 = new Entity();
	Decor1->SetImage("textures/platform.png", renderer);
	Decor1->mass = 100.0f;
	Decor1->size = Vec3(4.0f, 1.0f, 0.0f);
	Decor1->pos = Vec3(19.5f, 13.0f, 0.0f);
	Decor1->isStatic = true;
	platforms.emplace_back(Decor1);
	auto Decor2 = new Entity();
	Decor2->SetImage("textures/platform.png", renderer);
	Decor2->mass = 100.0f;
	Decor2->size = Vec3(4.5f, 0.5f, 0.0f);
	Decor2->pos = Vec3(2.7f, 10.0f, 0.0f);
	Decor2->isStatic = true;
	platforms.emplace_back(Decor2);

	//-----------------------PORTAL--------------------------
	portal = new Entity();
	portal->SetImage("textures/PortalDoor.png", renderer);
	portal->mass = 0.0f;
	portal->size = Vec3(3.0f, 3.0f, 0.0f);
	portal->pos = Vec3(36.5f, 11.5f, 0.0f);
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
	camera = new Camera;
	camera->cameraRect = { 0,0,30,15 };
	camera->pos = Vec3(camera->cameraRect.x, camera->cameraRect.y, 0.0f);
	return true;
}

void Scene1::OnDestroy() {
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

	delete camera;
	camera = nullptr;
}

void Scene1::HandleEvents(const SDL_Event& event) {
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

void Scene1::Update(const float deltaTime) {

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
					playerDeath = true;
					std::cout << "Player would die! Implementation to restart the scene is needed!\n";
				}
			}
			//Otherwise, ticks down the timer if it is currently active
			else if (objects[i]->rewindTimer > 0.0f) {
				objects[i]->rewindTimer -= 0.1f;
				//Prints object timer for debug purposes
			//	std::cout << "Box " << i + 1 << ": " << objects[i]->rewindTimer << "\n";
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
				playerDeath = true;
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

	camera->Follow(player->pos);

	//----------------PORTAL----------------------

	if (portal && collision.CheckCollision(*player, *portal)) {
		sceneComplete = true;
	}
}

void Scene1::Render() const {
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