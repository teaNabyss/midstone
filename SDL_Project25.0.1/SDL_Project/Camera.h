#pragma once
#include <SDL3/SDL.h>
#include <SDL3/SDL_image.h>
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

    void Set(Vec3 pos_);

    void Follow(const Vec3& playerPos);


    bool LoadBackground();

    void Render();


    Matrix4 GetProjectionMatrix() const {
        return projectionMatrix;
    }
};
