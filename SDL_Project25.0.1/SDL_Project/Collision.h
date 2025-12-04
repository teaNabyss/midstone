#ifndef COLLISION_H
#define COLLISION_H
#include <SDL3/SDL.h>
#include <SDL3/SDL_image.h>
#include <MMath.h>
#include "Entity.h"
#include "Wall.h"
using namespace MATH;

class Collision {
public:
	
	Collision();
	~Collision();
	void BorderCollision(Entity& obj);
	bool CheckCollision(Entity& obj1, Entity& obj2);
	bool ResolveCollision(Entity& obj1, Entity& obj2);
	void drawAwallBox(Wall& solid);
	//void drawAplayerBox();


};

#endif