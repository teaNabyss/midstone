#include "Collision.h"
#include <MMath.h>

Collision::Collision() {}

Collision::~Collision() {}

void Collision::drawAwallBox(Wall& solid) {

}

void Collision::BorderCollision(Entity& obj) {

    if (obj.pos.x - obj.size.x < 0.0f) {
        obj.pos.x = obj.size.x;
    }
    else if (obj.pos.x > 30.0f) {
        obj.pos.x = 30.0f;
    }

    if (obj.pos.y - obj.size.x < 0.0f) {
        obj.pos.y = obj.size.x;
    }
    else if (obj.pos.y > 15.0f) {
        obj.pos.y = 15.0f;
    }
}

bool Collision::CheckCollision(Entity& obj1, Entity& obj2) {

    if (obj1.pos.x > obj2.pos.x + obj2.size.x ||
        obj1.pos.x + obj1.pos.x > obj2.pos.x ||
        obj1.pos.y - obj2.size.y > obj2.pos.y ||
        obj1.pos.y > obj2.pos.y - obj2.size.y)
        return false;

}

void Collision::AABB(Entity& obj1, Entity& obj2) {
    if (CheckCollision(obj1, obj2)) return;

}
