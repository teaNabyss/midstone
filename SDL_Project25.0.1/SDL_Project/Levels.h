#ifndef Levels_H
#define Levels_H
#include <SDL3/SDL.h>
#include <SDL3/SDL_image.h>
#include <Matrix.h>
#include "Wall.h"
#include "Player.h"
#include "Camera.h"


#include <SDL3/SDL_mixer.h>

using namespace MATH;
class Levels {
private:
	SDL_Window *window;
	float xAxis;
	float yAxis;

	Wall* background;
	Wall* windowBorder;

	Matrix4 projectionMatrix;
	SDL_Renderer* renderer;

	Camera* camera;
	
public:
	Levels(SDL_Window* sdlWindow);
	~Levels();
	bool OnCreate();
	void OnDestroy();
	void HandleEvents(const SDL_Event& event);
	void Update(const float time);
	void SetRenderer(SDL_Renderer* renderer_);
	void Render() const;
};

#endif
