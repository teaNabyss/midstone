#include "Player.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_image.h>
#include <MMath.h>
#include <iostream>
#include "Entity.h"



Player::Player(SDL_Window* sdlWindow_) :
	window(sdlWindow_)
	, renderer(nullptr)
	, player(nullptr)
	, box(nullptr)
	, xAxis(30.0f)
	, yAxis(15.0f)
{
}

Player::~Player() {

}
bool flipHorizontal = false;

bool Player::OnCreate() {
	// Create a project matrix that moves positions from physics/world space 
	// to screen/pixel space
	// !!!!! ^ this code is in camera now ^ !!!!!
	int w, h;
	SDL_GetWindowSize(window, &w, &h); //1280 X 720

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
	player->pos = Vec3(15.0f, 1.0f, 0.0f);
	player->mass = 2.0f;
	player->SetImage("textures/PurpleMailSprite.png", renderer);
	
	playerScale = 4.0f; //used to scale image, and in collision check
	
	// Creates a box for collision code
	box = new Entity();
	box->pos = Vec3(10.0f, 1.0f, 0.0f);
	box->mass = 4.0f;
	box->SetImage("textures/crate.png", renderer);

	// Creates a background to scroll it, just for now
	background = new Entity();
	background->pos = Vec3(0.0f, 15.0f, 0.0f);
	background->SetImage("textures/205028.png", renderer);
	std::cout << "Backround created" << std::endl;

	// "camera"
	camera = new Camera();

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

void Player::HandleEvents(const SDL_Event& event)
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

void Player::Update(const float deltaTime) {
	if (deltaTime < VERY_SMALL) return;

	// Gravity
	Vec3 gravAccel(0.0f, -9.8f, 0.0f);
	Vec3 PlayerGravForce = player->mass * gravAccel;

	// Jump
	float g = 9.8f; //gravity
	float jumpForce = sqrt(2.0f * g * jumpHeight); // how fast will player jump considering height and gravity
	//const float jumpDistance = 4.0f; 

	if (SpaceDown && player->OnGround) {
		player->vel.y = jumpForce;  // pushes up/ jump itsellf
		player->ApplyForce(PlayerGravForce); // apllies gravity
		player->OnGround = false;
	}

	// applies x motion on player
	player->xInput(keyAdown, keyDdown);
	
	// moves background in opposite direction to player with half of player's speed
	float move = (player->speed * 0.5f) * deltaTime;

	if (keyAdown) {
		background->pos.x += move;
	}
	else if (keyDdown) {
		background->pos.x -= move;
	}


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
	box->BorderCollision(3.5f);



	// --- Player-Box collision (AABB) ---

	float playerHalfSize = 1.0f; 
	float boxHalfSizeW = 1.2f;   
	float boxHalfSizeH = 1.0f;

	Vec3 playerMin = player->pos - Vec3(playerHalfSize, playerHalfSize, 0.0f);
	Vec3 playerMax = player->pos + Vec3(playerHalfSize, playerHalfSize, 0.0f);

	Vec3 boxMin = box->pos - Vec3(boxHalfSizeW, boxHalfSizeH, 0.0f);
	Vec3 boxMax = box->pos + Vec3(boxHalfSizeW, boxHalfSizeH, 0.0f);

	bool overlapX = (playerMin.x <= boxMax.x) && (playerMax.x >= boxMin.x);
	bool overlapY = (playerMin.y <= boxMax.y) && (playerMax.y >= boxMin.y);


	if (overlapX && overlapY)
	{
		float overlapLeft = playerMax.x - boxMin.x;
		float overlapRight = boxMax.x - playerMin.x;
		float overlapTop = boxMax.y - playerMin.y;
		float overlapBottom = playerMax.y - boxMin.y;

		float minOverlapX = std::min(overlapLeft, overlapRight);
		float minOverlapY = std::min(overlapTop, overlapBottom);

		float correctionBlend = 0.2f;

		if (minOverlapX < minOverlapY)
		{
			float targetX = player->pos.x;
			if (overlapLeft < overlapRight)
				targetX -= minOverlapX;
			else
				targetX += minOverlapX;

			// blend towards target
			player->pos.x = targetX;

			player->vel.x = 0.0f;
		}
		else
		{
			float targetY = player->pos.y;
			if (overlapBottom < overlapTop)
			{
				targetY = boxMax.y + playerScale;
				player->OnGround = true;
			}
			else
			{
				targetY = boxMin.y - playerScale;
			}
		}
	}

	//Camera follows player here
	camera->Follow(player->pos);
	// still not yet

}

void Player::Render() const {
	SDL_RenderClear(renderer);

	// Convert from world coordinates to pixel coordinates using Scott's magical matrix
	Vec3 screenCoords = camera->GetProjectionMatrix() * background->pos;
	SDL_FRect square;
	square.x = screenCoords.x;
	square.y = screenCoords.y;
	square.w = background->GetSurface()->w * 4.0f;
	square.h = background->GetSurface()->h * 4.4f;
	SDL_RenderTextureRotated(renderer, background->GetTexture(), nullptr, &square, background->angleDeg, nullptr, SDL_FLIP_NONE);

	//Vec3 relativePos = camera->WorldToScreen(player->pos);
	screenCoords = camera->GetProjectionMatrix() * player->pos;
	// Set up sprite's position and size
	square.x = screenCoords.x;
	square.y = screenCoords.y;
	square.w = player->GetSurface()->w / playerScale;
	square.h = player->GetSurface()->h / playerScale;
	square.x -= square.w;
	square.y -= square.h * 0.25f;
	SDL_RenderTextureRotated(renderer, player->GetTexture(), nullptr, &square, player->angleDeg, nullptr, 
		(flipHorizontal) ?SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);

	//crate
	screenCoords = camera->GetProjectionMatrix() * box->pos;
	// Set up sprite's position and size
	square.x = screenCoords.x;
	square.y = screenCoords.y;
	square.w = player->GetSurface()->w / 3.5f;
	square.h = player->GetSurface()->h / 4.5f;
	square.x -= square.w;
	square.y -= square.h * 0.25f;
	SDL_RenderTextureRotated(renderer, box->GetTexture(), nullptr, &square, box->angleDeg, nullptr, SDL_FLIP_NONE);

	// Update the screen
	SDL_RenderPresent(renderer);
}