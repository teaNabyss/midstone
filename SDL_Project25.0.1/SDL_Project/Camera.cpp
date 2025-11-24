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

void Camera::WorldScroll( Vec3& objectsPos) {
    objectsPos.x -= pos.x * (-0.5f);

}
