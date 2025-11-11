#ifndef COLLISION_H
#define COLLISION_H
#include <SDL3/SDL.h>
#include <SDL3/SDL_image.h>
#include <MMath.h>
#include "Entity.h"
using namespace MATH;

class Collision {
public:
	bool collisionX;
	bool collisionY;
	float prevPos;
	Collision();
	~Collision();
	void BorderCollision(Entity& obj, Vec3 scale);
	bool CheckCollision(Entity& obj1, Entity& obj2);
	void AABB(Entity& obj1, Entity& obj2);
};

#endif