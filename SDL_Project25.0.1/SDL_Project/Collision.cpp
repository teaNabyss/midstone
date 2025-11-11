#include "Collision.h"
#include <MMath.h>

Collision::Collision() {}

Collision::~Collision() {}

void Collision::BorderCollision(Entity& obj, Vec3 scale) {

    if (obj.pos.x - scale.x / 2 < 0.0f) {
        obj.pos.x = scale.x / 2;
        obj.vel.x *= -1.0f;
    }
    else if (obj.pos.x > 30.0f) {
        obj.pos.x = 30.0f;
        obj.vel.x *= -1.0f;
    }

    if (obj.pos.y - scale.x / 2 < 0.0f) {
        obj.pos.y = scale.x / 2;
        obj.vel.y = 0.0f;    
    }
    else if (obj.pos.y > 15.0f) {
        obj.pos.y = 15.0f;
        obj.vel.y *= -1.0f;
    }
}

bool Collision::CheckCollision(Entity& obj1, Entity& obj2) {

    // collision x-axis?
    collisionX = obj1.pos.x + obj1.size.x >= obj2.pos.x &&
        obj2.pos.x + obj2.size.x >= obj1.pos.x;
    // collision y-axis?
    collisionY = obj1.pos.y + obj1.size.y >= obj2.pos.y &&
        obj2.pos.y + obj2.pos.y >= obj1.pos.y;
    // collision only if on both axes
    return collisionX && collisionY;
}

void Collision::AABB(Entity& obj1, Entity& obj2) {
    // Посчитаем половины размеров
    float halfW1 = obj1.size.x * 0.5f;
    float halfH1 = obj1.size.y * 0.5f;
    float halfW2 = obj2.size.x * 0.5f;
    float halfH2 = obj2.size.y * 0.5f;

    // Центры
    float dx = (obj1.pos.x) - (obj2.pos.x);
    float dy = (obj1.pos.y) - (obj2.pos.y);

    // Перекрытие по осям
    float overlapX = obj1.size.x + halfW2 - fabs(dx);
    float overlapY = obj1.size.x + halfH2 - fabs(dy);

    // Если пересечения нет, нафиг выходим
    if (overlapX <= 0 || overlapY <= 0) return;

    // Определяем направление и двигаем только по самой маленькой оси
    if (overlapX < overlapY) {
        // Столкнулись слева/справа
        if (dx > 0) obj1.pos.x += overlapX;
        else        obj1.pos.x -= overlapX;

        obj1.vel.x = 0;
    }
    else {
        // Столкнулись сверху/снизу
        if (dy > 0) {
            // player сверху box
            obj1.pos.y += overlapY;
            // стоим на ящике
            obj1.vel.y = 0;
        }
        else {
            // player снизу box
            obj1.pos.y -= overlapY;
            obj1.vel.y = 0;
        }
    }

}
