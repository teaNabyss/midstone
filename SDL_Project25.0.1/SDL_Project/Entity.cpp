#include "Entity.h"

Entity::Entity() : // this is an initializer list
	surface(nullptr)
	, texture(nullptr)
	, angleDeg(0.0f)
	, mass(1.0f)
	, radius(1.0f)
{
}

Entity::~Entity() {
	// This is the destructor for Entity (notice the little squiggle ~)
	// Clean up the surface and texture data
	//SDL_DestroySurface(surface);
	//SDL_DestroyTexture(texture);
	surface = nullptr;
	texture = nullptr;
}
// Sets an image of an entity object
void Entity::SetImage(const char* filename, SDL_Renderer* renderer) {
	// We will use the surface to grab the width and height of the image later on
	surface = IMG_Load(filename);
	if (surface == nullptr) {
		std::cerr << "Did you spell the file right?" << std::endl;
	}
	// We will use the texture to render to screen later on
	texture = SDL_CreateTextureFromSurface(renderer, surface);
}

void Entity::ApplyForce(Vec3 netForce)
{
	acc = netForce / mass;
}

void Entity::Update(float deltaTime)
{
	vel += acc * deltaTime;
	pos += vel * deltaTime + 0.5f * acc * (deltaTime * deltaTime);
}

// Handle collision with window borders
void Entity::BorderCollision(float playerScale) {
	if (pos.x - playerScale / 2 < 0.0f) {
		pos.x = playerScale / 2;
		vel.x *= -1.0f;
	}
	else if (pos.x > 30.0f) {
		pos.x = 30.0f;
		vel.x *= -1.0f;
	}

	if (pos.y - playerScale / 2 < 0.0f) {
		pos.y = playerScale / 2;
	}
	else if (pos.y > 15.0f) {
		pos.y = 15.0f;
		vel.y *= -1.0f;
	}

}
// Handels left and right movement using keyAdown and keyDdown boolean check
void Entity::xInput(bool keyAdown, bool keyDdown) {

	if (keyAdown && !keyDdown)
		vel.x = -speed;
	else if (!keyAdown && keyDdown)
		vel.x = speed;
	else
		vel.x = 0.0f;
}
