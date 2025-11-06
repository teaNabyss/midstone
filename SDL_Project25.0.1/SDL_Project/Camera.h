#pragma once
#include <SDL3/SDL.h>
#include <SDL3/SDL_image.h>
#include <MMath.h>

class Camera {
public:
    Vec3 pos;       
    int width = 1280;      
    int height = 720;  
    Matrix4 ndc;
    Matrix4 ortho;
    Matrix4 projectionMatrix;

    Camera();

    void SetView(int w, int h);

    void Follow(const Vec3& targetPos);

    Vec3 WorldToScreen(const Vec3& worldPos) const;
};
