#ifndef Player_H
#define Player_H
#include <SDL3/SDL.h>
#include <SDL3/SDL_image.h>

#include <Matrix.h>
#include "Entity.h"
#include "Camera.h"
#include "Collision.h"
#include <SDL3/SDL_mixer.h>

using namespace MATH;
class Player  {
private:
	SDL_Window *window;
 	float xAxis;
	float yAxis;

	SDL_Renderer* renderer;

	Entity* box;
	Entity* background;
	Camera* camera;
	bool running = 0;
	bool keyAdown = false;
	bool keyDdown = false;
	bool SpaceDown = false;
	bool flipHorizontal = false;
	float playerSpeed = 15.0f;
	float jumpHeight = 2.0f;

	Collision collision;


public:
	Entity* player;
	//Player(SDL_Window* sdlWindow); //old one, don't get bothered by it
	Player(SDL_Window* sdlWindow);
	~Player();
	bool OnCreate();
	void OnDestroy();
	//void HandleEvents(const SDL_Event& event); //old handle events if stuf will go wrong >:/
	void HandleInput(const bool* keyboardState); // better say that it's handles input of a player, 
														//the handle events is more of a "NotYetGone" manager function
	void Update(const float time);
	void SetRenderer(SDL_Renderer* renderer_);
	void Render() const;
};

#endif
