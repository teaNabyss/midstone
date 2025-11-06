#include "Camera.h"

Camera::Camera() {
    ndc = MMath::viewportNDC(width, height);
    ortho = MMath::orthographic(0.0f, 30.0f, 0.0f, 15.0f, -1.0f, 1.0f);
    projectionMatrix = ndc * ortho;
}


void Camera::SetView(int w, int h){
    pos = { 0, 0, 0 };
    width = w;
    height = h;
    }

void Camera::Follow(const Vec3& targetPos) {
    pos.x = targetPos.x - width * 0.5f;
    pos.y = targetPos.y - height * 0.5f;
}

Vec3 Camera::WorldToScreen(const Vec3& worldPos) const {
    return { worldPos.x - pos.x, worldPos.y - pos.y, worldPos.z - pos.z };
}
