#pragma once
#include <SDL3/SDL.h>
#include <SDL3/SDL_image.h>
#include "Wall.h"
#include <MMath.h>

class Camera {
public:
    float CamWidth;
    float CamHeight;
    int width = 1280;      
    int height = 720;  
    Matrix4 ndc;
    Matrix4 ortho;
    Matrix4 projectionMatrix;
    Matrix4 view;
    SDL_Renderer* renderer;

    Camera();
    ~Camera();

    bool OnCreate();  

    void Follow(const Vec3& playerPos);

    void WorldScroll( Vec3& objects);

    Matrix4 GetProjectionMatrix() const {
        return projectionMatrix;
    }
};
