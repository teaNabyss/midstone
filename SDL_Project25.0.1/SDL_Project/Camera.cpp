#include "Camera.h"
#include <SDL3/SDL_image.h>
#include <SDL3/SDL.h>
#include <MMath.h>

Camera::Camera() : renderer(nullptr)
{
    ndc = MMath::viewportNDC(width, height);
    ortho = MMath::orthographic(0.0f, 30.0f, 0.0f, 15.0f, -1.0f, 1.0f);
    projectionMatrix = ndc * ortho;
}

Camera::~Camera() {}

bool Camera::OnCreate() {
	return true;

}

void Camera::Set(Vec3 pos_){
    pos = pos_;
    }

void Camera::Follow(const Vec3& playerPos) {
	pos.x = playerPos.x - (width * 0.5f);
	pos.y = playerPos.y - (height * 0.5f);
}


//bool LoadBackground() {

//}

void Camera::SetRenderer(SDL_Renderer* renderer_) {
	renderer = renderer_;
}

void Camera::Render(Wall& solid)
{

	// Convert from world coordinates to pixel coordinates using Scott's magical matrix
	Vec3 screenCoords;
	// Set up sprite's position and size
	SDL_FRect square;

	screenCoords = projectionMatrix * solid.pos;
	square.x = screenCoords.x;
	square.y = screenCoords.y;
	square.w = solid.size.x * projectionMatrix[0];
	square.h = solid.size.y * projectionMatrix[5];
	// Display object on the screen
	SDL_RenderTextureRotated(renderer, solid.GetTexture(), nullptr, &square, solid.angleDeg, nullptr, SDL_FLIP_NONE);

}

