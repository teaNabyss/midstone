#include "Camera.h"

Camera::Camera() {
    ndc = MMath::viewportNDC(width, height);
    ortho = MMath::orthographic(0.0f, 30.0f, 0.0f, 15.0f, -1.0f, 1.0f);
    projectionMatrix = ndc * ortho;
    pos = Vec3(width / 2, height / 2, 0.0f);
    CamWidth = width;
    CamHeight = height;
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


