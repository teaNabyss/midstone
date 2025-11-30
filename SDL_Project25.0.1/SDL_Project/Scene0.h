#ifndef SCENE0_H
#define SCENE0_H
#include <SDL3/SDL.h>
#include <Matrix.h>
#include "Scene.h"
#include "Entity.h"
#include "Camera.h"
#include "Collision.h"
#include <SDL3/SDL_mixer.h>
#include <vector>

using namespace MATH;
class Scene0 : public Scene {
private:
	SDL_Window *window;
	float xAxis;
	float yAxis;

	Matrix4 projectionMatrix;
	SDL_Renderer* renderer;

	Entity* player;
	Collision collision;
	Entity* background;

	//movement booleans
	bool movingLeft;
	bool movingRight;
	bool jumpInput;
	bool isJumping;
	bool isOnGround;

	//Non-Static objects
	std::vector<Entity*> objects;
	//Static platforms/level geometry
	std::vector<Entity*> platforms;

	Camera* camera;
	SDL_Rect cameraSquare;
	MIX_Mixer* mixer;
	float master_volume = 0.25f;
public:
	Scene0(SDL_Window* sdlWindow);
	~Scene0();
	bool OnCreate() override;
	void OnDestroy() override;
	void HandleEvents(const SDL_Event& event) override;
	void Update(const float time) override;
	void Render() const override;
};

#endif