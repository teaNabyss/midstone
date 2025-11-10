#ifndef WALL_H
#define WALL_H

#include <Vector.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_image.h>

using namespace MATH;

class Wall {
private:
	// Keep these private as we should only build them in the setImage method
	// I do listen to Scott sometimes...
	SDL_Surface* surface; // Used to get the width and height of the image
	SDL_Texture* texture; // Used to render the image

public:
	float angleDeg;
	float width;
	float height;
	Vec3 pos;

	float facingDir = 1;
	float radius;

	Wall();
	~Wall();
	void SetImage(const char* filename, SDL_Renderer* renderer);

	void ApplyForce(Vec3 netForce);
	void Update(float deltaTime);
	void borderCollision();

	// Need getters for private member variables. 
	SDL_Surface* GetSurface() const { return surface; }
	SDL_Texture* GetTexture() const { return texture; }
};

#endif
