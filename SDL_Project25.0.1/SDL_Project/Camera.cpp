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

