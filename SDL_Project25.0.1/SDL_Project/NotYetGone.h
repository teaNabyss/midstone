#pragma once
#include <SDL3/SDL.h>
#include <SDL3/SDL_mixer.h>
#include <SDL3/SDL_image.h>
#include <iostream>
#include <MMath.h>
#include "Levels.h"
#include "Camera.h"
#include "Player.h"

class NotYetGone : public Scene {
private:
	SDL_Window* window;
	SDL_Renderer* renderer;



	MIX_Mixer* mixer;
	float master_volume = 0.1f;
public:
	NotYetGone(SDL_Window* sdlWindow);
	~NotYetGone();
	bool OnCreate() override;
	void OnDestroy() override;
	void HandleEvents(const SDL_Event& event) override;
	void Update(const float time) override;
	void Render() const override;

};