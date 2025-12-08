#ifndef SCENE2_H
#define SCENE2_H
#include <SDL3/SDL.h>
#include <Matrix.h>
#include "Scene.h"
#include "Entity.h"
#include "Camera.h"
#include "Collision.h"
#include <SDL3/SDL_mixer.h>
#include <vector>

using namespace MATH;
class Scene2 : public Scene {
private:
	SDL_Window* window;
	float xAxis;
	float yAxis;

	Matrix4 projectionMatrix;
	SDL_Renderer* renderer;

	Entity* player;
	Entity* portal;
	Entity* background;
	Entity* OtherBackground;
	Collision collision;
	Vec3 gravForce;

	//movement booleans
	bool movingLeft;
	bool movingRight;
	bool jumpInput;
	bool sceneComplete = false;
	bool playerDeath = false;
	float jumpStrength;

	//Non-Static objects
	std::vector<Entity*> objects;
	//Static platforms/level geometry
	std::vector<Entity*> platforms;
	std::vector<Entity*> backgrounds;

	Camera* camera;
	bool flipHorizontal = false;
	//SDL_FRect cameraSquare; //should've been FRect -> float
	MIX_Mixer* mixer;
	float master_volume = 0.25f;
public:
	Scene2(SDL_Window* sdlWindow);
	~Scene2();
	bool IsComplete() const override { return sceneComplete; }
	bool PlayerDeath() const override { return playerDeath; }
	//Rewinds an object
	bool RewindObj(int i);
	bool OnCreate() override;
	void OnDestroy() override;
	void HandleEvents(const SDL_Event& event) override;
	void Update(const float time) override;
	void Render() const override;
};

#endif