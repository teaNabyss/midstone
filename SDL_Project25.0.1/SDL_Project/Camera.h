#pragma once
#include <SDL3/SDL.h>
#include <SDL3/SDL_image.h>
#include "Wall.h"
#include <MMath.h>

class Camera {
public:
    Vec3 pos;
    float CamWidth;
    float CamHeight;
    int width = 1280;      
    int height = 720;  
    Matrix4 ndc;
    Matrix4 ortho;
    Matrix4 projectionMatrix;

    SDL_Renderer* renderer;

    Camera();
    ~Camera();

    bool OnCreate();  

    void Set(Vec3 pos_);

    void Follow(const Vec3& playerPos);

    bool LoadBackground();

    void SetRenderer(SDL_Renderer* renderer_);

    void Render(Wall& solid);


    Matrix4 GetProjectionMatrix() const {
        return projectionMatrix;
    }
};
