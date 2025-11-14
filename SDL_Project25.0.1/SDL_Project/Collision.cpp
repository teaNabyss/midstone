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

    if (obj1.pos.x > obj2.pos.x + obj2.size.x ||
        obj1.pos.x + obj1.size.x < obj2.pos.x ||
        obj1.pos.y - obj1.size.y > obj2.pos.y ||
        obj1.pos.y > obj2.pos.y - obj2.size.y)
        return false;

}

void Collision::AABB(Entity& obj1, Entity& obj2) {
    if (CheckCollision(obj1, obj2)) return;

}
