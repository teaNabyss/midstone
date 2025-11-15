#include "Collision.h"
#include <MMath.h>

Collision::Collision() {}

Collision::~Collision() {}

void Collision::drawAwallBox(Wall& solid) {

}

//Entity& obj is player
void Collision::BorderCollision(Entity& obj) {

    if (obj.pos.x - obj.size.x / 2 < 0.0f) {
        obj.pos.x = obj.size.x / 2;
    }
    else if (obj.pos.x + obj.size.x / 2 > 30.0f) {
        obj.pos.x = 30.0f - obj.size.x / 2;
    }

    if (obj.pos.y - obj.size.y / 2 < 0.0f) {
        obj.pos.y = obj.size.y / 2;
    }
    else if (obj.pos.y + obj.size.y / 2 > 15.0f) {
        obj.pos.y = 15.0f - obj.size.y / 2;
    }
}
// obj1 is a player and obj2 is a platform
bool Collision::CheckCollision(Entity& obj1, Entity& obj2) {
    // Calculate half-widths and half-heights
    float obj1HalfW = obj1.size.x / 2.0f;
    float obj1HalfH = obj1.size.y / 2.0f;
    float obj2HalfW = obj2.size.x / 2.0f;
    float obj2HalfH = obj2.size.y / 2.0f;

    // Calculate the edges of each hitbox (assuming x,y is center)
    float obj1Left = obj1.pos.x - obj1HalfW;
    float obj1Right = obj1.pos.x + obj1HalfW;
    float obj1Top = obj1.pos.y - obj1HalfH;
    float obj1Bottom = obj1.pos.y + obj1HalfH;

    float obj2Left = obj2.pos.x - obj2HalfW;
    float obj2Right = obj2.pos.x + obj2HalfW;
    float obj2Top = obj2.pos.y - obj2HalfH;
    float obj2Bottom = obj2.pos.y + obj2HalfH;

    // Check for overlap (AABB collision)
    return !(obj1Right < obj2Left ||   // obj1 is left of obj2
        obj1Left > obj2Right ||   // obj1 is right of obj2
        obj1Bottom < obj2Top ||   // obj1 is above obj2
        obj1Top > obj2Bottom);    // obj1 is below obj2
}

void Collision::AABB(Entity& obj1, Entity& obj2) {
    if (!CheckCollision(obj1, obj2)) return;  // Fixed: should be !CheckCollision

    // Calculate half dimensions
    float obj1HalfW = obj1.size.x / 2.0f;
    float obj1HalfH = obj1.size.y / 2.0f;
    float obj2HalfW = obj2.size.x / 2.0f;
    float obj2HalfH = obj2.size.y / 2.0f;

    // Calculate distance between centers
    float deltaX = obj2.pos.x - obj1.pos.x;
    float deltaY = obj2.pos.y - obj1.pos.y;

    // Calculate overlap on each axis
    float overlapX = (obj1HalfW + obj2HalfW) - fabs(deltaX);
    float overlapY = (obj1HalfH + obj2HalfH) - fabs(deltaY);

    // Resolve collision on the axis with smallest overlap
    if (overlapX < overlapY) {
        // Resolve horizontally
        if (deltaX > 0) {
            // obj2 is to the right of obj1
            obj1.pos.x -= overlapX / 2.0f;
            obj2.pos.x += overlapX / 2.0f;
        }
        else {
            // obj2 is to the left of obj1
            obj1.pos.x += overlapX / 2.0f;
            obj2.pos.x -= overlapX / 2.0f;
        }
    }
    else {
        // Resolve vertically
        if (deltaY > 0) {
            // obj2 is below obj1
            obj1.pos.y -= overlapY / 2.0f;
          //  obj2.pos.y += overlapY / 2.0f;
        }
        else {
            // obj2 is above obj1
            obj1.pos.y += overlapY;
          //  obj2.pos.y -= overlapY / 2.0f;
        }
    }
}