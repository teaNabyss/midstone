#ifndef ENTITY_H
#define ENTITY_H

#include <Vector.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_image.h>
using namespace MATH;

class Entity {
private:
	// Keep these private as we should only build them in the setImage method
	// I do listen to Scott sometimes...
	SDL_Surface* surface; // Used to get the width and height of the image
	SDL_Texture* texture; // Used to render the image

public:
	float angleDeg;
	Vec3 pos;
	Vec3 vel;
	Vec3 acc;
	Vec3 size;
	float mass;
	bool isPlayer;
	bool isStatic;
	bool onGround;
	float rewindTimer;
	float rewindMaxTimer;
	Vec3 ogPos;
	bool autoRewind;
	// Let's pretend all entities are circles
	float radius;

	Entity();
	~Entity();
	void SetImage(const char* filename, SDL_Renderer* renderer);

	void ApplyForce(Vec3 netForce);
	void Update(float deltaTime);
	//AD: Future idea: utilize the isPlayer boolean to localize player movement to the entity file rather than in each individual scene

	//AD: Are we really going to use any of these?
	//void MoveLeft(float deltaTime);
	//void MoveRight(float deltaTime);
	//void MoveUP(float deltaTime);
	//Vec3 BorderCollision(float playerScale);
	//void xInput(bool keyAdown, bool keyDdown);


	// Need getters for private member variables. 
	SDL_Surface* GetSurface() const { return surface; }
	SDL_Texture* GetTexture() const { return texture; }
};

#endif