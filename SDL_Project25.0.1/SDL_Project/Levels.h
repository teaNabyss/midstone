#ifndef Levels_H
#define Levels_H
#include <SDL3/SDL.h>
#include <Matrix.h>
#include "Scene.h"
#include "Wall.h"

#include <SDL3/SDL_mixer.h>

using namespace MATH;
class Levels : public Scene {
private:
	SDL_Window *window;
	float xAxis;
	float yAxis;

	Wall* background;
	Wall* windowBorder;

	Matrix4 projectionMatrix;
	SDL_Renderer* renderer;

	
public:
	Levels(SDL_Window* sdlWindow);
	~Levels();
	bool OnCreate() override;
	void OnDestroy() override;
	void HandleEvents(const SDL_Event& event) override;
	void Update(const float time) override;
	void Render() const override;
};

#endif
