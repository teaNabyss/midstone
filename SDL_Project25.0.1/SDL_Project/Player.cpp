#include "Player.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_image.h>
#include <MMath.h>
#include <iostream>
#include "Entity.h"
#include "Collision.h"


//Player::Player(SDL_Window* sdlWindow_) :
//	window(sdlWindow_)
//	, renderer(nullptr)
//	, player(nullptr)
//	, box(nullptr)
//	, xAxis(30.0f)
//	, yAxis(15.0f)
//{
//}

Player::Player(SDL_Window* sdlWindow_) :
	window(sdlWindow_),
	player(nullptr),
	camera(nullptr)
{
}
Player::~Player() {

}

bool Player::OnCreate() {
	// Create a project matrix that moves positions from physics/world space 
	// to screen/pixel space
	// !!!!! ^ this code is in camera now ^ !!!!!
	int w, h;
	SDL_GetWindowSize(window, &w, &h); //1280 X 720

	//Create screen renderer
	//renderer = SDL_CreateRenderer(window, NULL);
	//if (!renderer) {
	//	std::cerr << "SDL_Error: " << SDL_GetError() << std::endl;
	//	return false;
	//}
	////Initialize renderer color (black)
	//SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);


	// Creates a player that will be rendered on the screen
	player = new Entity();
	player->pos = Vec3(2.0f, 0.0f, 0.0f);
	player->mass = 2.0f;
	player->size = Vec3(2.0f, 3.0f, 0.0f);
	player->SetImage("textures/PurpleMailSprite.png", renderer);
		
	// Creates a box for collision code
	//box = new Entity();
	//box->pos = Vec3(10.0f, 0.0f, 0.0f);
	//box->size = Vec3(2.0f, 2.0f, 0.0f);
	//box->SetImage("textures/crate.png", renderer);

	//// Creates a background to scroll it, just for now
	//background = new Entity();
	//background->pos = Vec3(0.0f, 16.0f, 0.0f);
	//background->SetImage("textures/background.png", renderer);
	//std::cout << "Backround created" << std::endl;

	// "camera"
	camera = new Camera();
//	camera->Set(player->pos);

	return true;
}

void Player::OnDestroy() {
	// Clean up the renderer
	if (renderer) {
		SDL_DestroyRenderer(renderer);
		renderer = nullptr;
	}

	

	// Delete the objects created on the heap
	// and set to the null pointer just to be safe

	delete player;
	player = nullptr;

	delete box;
	box = nullptr;

	delete background;
	background = nullptr;

	delete camera;
	camera = nullptr;

}

void Player::HandleInput(const bool* keyboardState) //we need to recieve the current state of a whole keyboard
													// and not just specific keys (too much work)
{													// and i dont want to move booleans into update, feels messy ( •_•)
	// Check A key
	if (keyboardState[SDL_SCANCODE_A]) {
		keyAdown = true;
		flipHorizontal = false;
	}
	else {
		keyAdown = false;
	}

	// Check D key
	if (keyboardState[SDL_SCANCODE_D]) {
		keyDdown = true;
		flipHorizontal = true;
	}
	else {
		keyDdown = false;
	}

	// Check Space
	if (keyboardState[SDL_SCANCODE_SPACE]) {
		SpaceDown = true;
	}
	else {
		SpaceDown = false;
	}
}

void Player::Update(const float deltaTime) {
	if (deltaTime < VERY_SMALL) return;

	// Gravity
	Vec3 gravAccel(0.0f, -9.8f, 0.0f);
	Vec3 PlayerGravForce = player->mass * gravAccel;

	// applies x motion on player
		//player->xInput(keyAdown, keyDdown);
	// Jump
	float g = 9.8f; //gravity
	float jumpForce = sqrt(2.0f * g * jumpHeight); // how fast will player jump considering height and gravity
	//const float jumpDistance = 4.0f; 

	//if (SpaceDown && player->OnGround) {
	// 	player->vel.y = jumpForce;  // pushes up/ jump itsellf
	//	player->OnGround = false;
	//}
		player->ApplyForce(PlayerGravForce); // apllies gravity

	
	// moves background in opposite direction to player with half of player's speed
	//float move = (player->speed * 0.5f) * deltaTime;

	//if (keyAdown) {
	//	background->pos.x += move;
	//}
	//else if (keyDdown) {
	//	background->pos.x -= move;
	//}


	// Physics update
	player->Update(deltaTime);

	// Ground collision check to prevent jump in the air
	
	if (player->pos.y - player->size.y / 2 <= 0.0f) {
		player->pos.y = player->size.y / 2;  // go back on the ground
		//player->vel.y = 0.0f;
		//player->OnGround = true;
	}
	else {
		//player->OnGround = false;
	}

	// Collision with borders
	collision.BorderCollision(*player);	
	//collision.BorderCollision(*box);

	//if (collision.CheckCollision(*player, *box)) {
	//	collision.AABB(*player, *box);
	//}

	//Camera follows player here
	//camera->Follow(player->pos);
	// still not yet

}

void Player::SetRenderer(SDL_Renderer* renderer_) {
	renderer = renderer_;
}
void Player::Render() const {
//	SDL_RenderClear(renderer);

	//// Convert from world coordinates to pixel coordinates using Scott's magical matrix
	//Vec3 screenCoords = camera->GetProjectionMatrix() * background->pos;
	//SDL_FRect square;
	//square.x = screenCoords.x;
	//square.y = screenCoords.y;
	//square.w = background->GetSurface()->w * 1.5f;
	//square.h = background->GetSurface()->h * 1.5f;
	//SDL_RenderTextureRotated(renderer, background->GetTexture(), nullptr, &square, background->angleDeg, nullptr, SDL_FLIP_NONE);

	//Vec3 relativePos = camera->WorldToScreen(player->pos);
	Vec3 screenCoords = camera->GetProjectionMatrix() * player->pos;
	SDL_FRect square;
	// Set up sprite's position and size
	square.x = screenCoords.x;
	square.y = screenCoords.y;
	square.w = player->size.x * camera->GetProjectionMatrix()[0];
	square.h = player->size.y * std::abs(camera->GetProjectionMatrix()[5]);
	square.x -= square.w / 2;
	square.y -= square.h / 2;
	SDL_RenderTextureRotated(renderer, player->GetTexture(), nullptr, &square, player->angleDeg, nullptr,
		(flipHorizontal) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);

	////crate
	//screenCoords = camera->GetProjectionMatrix() * box->pos;
	//// Set up sprite's position and size
	//square.x = screenCoords.x;
	//square.y = screenCoords.y;
	//square.w = box->size.x * camera->GetProjectionMatrix()[0];
	//square.h = box->size.y * std::abs(camera->GetProjectionMatrix()[5]);
	//square.x -= square.w / 2;
	//square.y -= square.h / 2;
	//SDL_RenderTextureRotated(renderer, box->GetTexture(), nullptr, &square, box->angleDeg, nullptr, SDL_FLIP_NONE);

	// Update the screen
//	SDL_RenderPresent(renderer);
}