#pragma once
#include <SDL3/SDL.h>
#include <SDL3/SDL_mixer.h>
#include <SDL3/SDL_image.h>
#include <iostream>
#include "Scene.h"
#include <MMath.h>
#include "Levels.h"
#include "Camera.h"
#include "Player.h"

class NotYetGone : public Scene {
private:
	SDL_Window* window;  //uindou
	SDL_Renderer* renderer; 

	Player* courier; //our courier made out of Player class

	Levels* background; //well, it's a background ಠ_ಠ

	MIX_Mixer* mixer; //some music 
	float master_volume = 0.1f;

	const bool* keyboardState = SDL_GetKeyboardState(nullptr); 

public:
	NotYetGone(SDL_Window* sdlWindow);
	~NotYetGone();
	bool OnCreate() override;
	void OnDestroy() override;
	void HandleEvents(const SDL_Event& event) override;
	void Update(const float time) override;
	void Render() const override;

};