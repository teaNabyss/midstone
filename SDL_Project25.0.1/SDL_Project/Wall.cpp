#include "Wall.h"

Wall::Wall() : // this is an initializer list
	surface(nullptr)
	, texture(nullptr)
	, angleDeg(0.0f)
{
}
Wall::~Wall() {
	// This is the destructor for Entity (notice the little squiggle ~)
	// Clean up the surface and texture data
	//SDL_DestroySurface(surface);
	//SDL_DestroyTexture(texture);
	surface = nullptr;
	texture = nullptr;
}
// Sets an image of an entity object
void Wall::SetImage(const char* filename, SDL_Renderer* renderer) {
	// We will use the surface to grab the width and height of the image later on
	surface = IMG_Load(filename);
	if (surface == nullptr) {
		std::cerr << "Did you spell the file right?" << std::endl;
	}
	// We will use the texture to render to screen later on
	texture = SDL_CreateTextureFromSurface(renderer, surface);
}

//Empty, but could be used later
void Wall::ApplyForce(Vec3 netForce)
{

}
//Empty, but can be used later
void Wall::Update(float deltaTime)
{

}

void Wall::borderCollision() {

}

