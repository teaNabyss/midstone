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
    //Calculate the edges of each hitbox (assuming x,y is center)
    float obj1Left = obj1.pos.x - (obj1.size.x / 2.0f);
    //AD: Previous code used "obj1Right = obj.pos.x + (obj1.size.x / 2.0f)". My thinking is that using obj1Left as a base saves a miniscule but not zero amount of calculation time
    float obj1Right = obj1Left + obj1.size.x;
    float obj1Top = obj1.pos.y - (obj1.size.y / 2.0f);
    float obj1Bottom = obj1Top + obj1.size.y;

    float obj2Left = obj2.pos.x - (obj2.size.x / 2.0f);
    float obj2Right = obj2Left + obj2.size.x;
    float obj2Top = obj2.pos.y - (obj2.size.y / 2.0f);
    float obj2Bottom = obj2Top + obj2.size.y;

    // Check for overlap (AABB collision)
    return !(obj1Right < obj2Left ||   // obj1 is left of obj2
        obj1Left > obj2Right ||   // obj1 is right of obj2
        obj1Bottom < obj2Top ||   // obj1 is above obj2
        obj1Top > obj2Bottom);    // obj1 is below obj2
}

void Collision::ResolveCollision(Entity& obj1, Entity& obj2) {
    //Safety measure to ensure the function is not called erronously
    if (!CheckCollision(obj1, obj2)) return;
    //Calculates the distance between object centers
    float deltaX = obj2.pos.x - obj1.pos.x;
    float deltaY = obj2.pos.y - obj1.pos.y;
    //Calculates the overlap on each axis using the half dimensions of both objects
    float overlapX = ((obj1.size.x / 2.0f) + (obj2.size.x / 2.0f)) - fabs(deltaX);
    float overlapY = ((obj1.size.y / 2.0f) + (obj2.size.y / 2.0f)) - fabs(deltaY);
    //Resolves collision on the axis with smallest overlap

    //Resolves horizontal collision
    if (overlapX < overlapY) {
        //If obj1 is static, obj2 is moved
        if (obj1.isStatic) {
            //obj2 is to the right of obj1
            if (deltaX > 0) {
                obj2.pos.x += overlapX;
            }
            //obj2 is to the left of obj1
            else {
                obj2.pos.x -= overlapX;
            }
        }
        //If obj2 is static, obj1 is moved
        else if (obj2.isStatic) {
            //obj1 is to the left of obj2
            if (deltaX > 0) {
                obj1.pos.x -= overlapX;
            }
            //obj1 is to the right of obj2
            else {
                obj1.pos.x += overlapX;
            }
        }
        //If neither object is static, both are moved
        else {
            //obj2 is to the right of obj1
            if (deltaX > 0) {
                obj1.pos.x -= overlapX / 2.0f;
                obj2.pos.x += overlapX / 2.0f;
            }
            //obj2 is to the left of obj1
            else {
                obj1.pos.x += overlapX / 2.0f;
                obj2.pos.x -= overlapX / 2.0f;
            }
        }
    }
    //Resolves vertical collision
    else if (overlapX > overlapY){
        //If obj1 is static, obj2 is moved
        if (obj1.isStatic) {
            //Halts obj2's velocity and acceleration
            obj2.vel.y = obj2.acc.y = 0.0f;
            //obj2 is below obj1
            if (deltaY > 0) {
                obj2.pos.y += overlapY;
            }
            //obj2 is above obj1
            else {
                obj2.pos.y -= overlapY;
                //TODO: Communicate with ground check boolean and jump functionality
            }
        }
        //If obj2 is static, obj1 is moved
        else if (obj2.isStatic) {
            //Halts obj1's velocity and acceleration
            obj1.vel.y = obj1.acc.y = 0.0f;
            //obj1 is above obj2
            if (deltaY > 0) {
                obj1.pos.y -= overlapY;
                //TODO: Communicate with ground check boolean and jump functionality
            }
            //obj1 is below obj2
            else {
                obj1.pos.y += overlapY;
            }
        }
        //If neither object is static, both are...actually what *would* happen?
        else {
            //AD: Different states:
            // - If player is above box, player stands on box as though it is static and box remains stationary
            // - If player is below box, player is crushed(?)
            // - If box is above box, above box stands on below box as though it is static and below box remains stationary
        }
    }
    //If horizontal and vertical overlap are identical, resolves corner collision
    else {
        //TODO: Implement corner collision resolution
    }
}