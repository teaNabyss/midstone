#ifndef Player_H
#define Player_H
#include <SDL3/SDL.h>
#include <Matrix.h>
#include "Scene.h"
#include "Entity.h"
#include "Camera.h"

#include <SDL3/SDL_mixer.h>

using namespace MATH;
class Player : public Scene {
private:
	SDL_Window *window;
	float xAxis;
	float yAxis;

	SDL_Renderer* renderer;

	Entity* player;
	Entity* box;
	Entity* background;
	Camera* camera;

	bool running = 0;
	bool keyAdown = false;
	bool keyDdown = false;
	bool SpaceDown = false;

	float playerScale; // a scale to make the texture smaller or larger
	float playerSpeed = 15.0f;
	float jumpHeight = 3.0f;


	MIX_Mixer* mixer;
	float master_volume = 0.1f;
public:
	Player(SDL_Window* sdlWindow);
	~Player();
	bool OnCreate() override;
	void OnDestroy() override;
	void HandleEvents(const SDL_Event& event) override;
	void Update(const float time) override;
	void Render() const override;
};

#endif
