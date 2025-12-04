#pragma once
#include <SDL3/SDL.h>
#include <SDL3/SDL_image.h>
#include "Wall.h"
#include <MMath.h>

class Camera {
public:
    int width = 1280;
    int height = 720;
    Matrix4 ndc;
    Matrix4 ortho;
    Matrix4 projectionMatrix;
    SDL_FRect cameraRect;
    Vec3 pos;

    Camera();
    ~Camera();
    void Follow(const Vec3& targetPos);
    Vec3 WorldToScreen(const Vec3& worldPos) const;

    Matrix4 GetProjectionMatrix() const {
        return projectionMatrix;
    }
};