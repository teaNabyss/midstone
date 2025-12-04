#include "NotYetGone.h"
#include <iostream>
#include <MMath.h>
#include "Levels.h"
#include "Camera.h"
#include "Player.h"

NotYetGone::NotYetGone(SDL_Window* sdlWindow_) : 
	window(sdlWindow_)
	, courier(nullptr),
	renderer(nullptr)
{}

NotYetGone::~NotYetGone(){}

bool NotYetGone::OnCreate() {

	//										RENDERER 
	//									 (for now here) 
	//Create screen renderer
	renderer = SDL_CreateRenderer(window, NULL);
	if (!renderer) {
		std::cerr << "SDL_Error: " << SDL_GetError() << std::endl;
		return false;
	}
	//Initialize renderer color (black)
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);


	//										COURIER
	courier = new Player(window);
	courier->SetRenderer(renderer);
	std::cout << "Courier was created" << std::endl;

	if (!courier->OnCreate()) {
		std::cerr << "Failed to create Player Courier ＼(｀0´)／ " << std::endl;
		return false;
	}

	//										 LEVEL
	level = new Levels(window);
	level->SetRenderer(renderer);
	if (!level->OnCreate()) {
		std::cerr << "Failed to create Levels Level ＼(｀0´)／ " << std::endl;
		return false;
	}


	//										 MIXER
	SDL_Init(SDL_INIT_AUDIO);
	MIX_Init();
	mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, nullptr);

	if (!mixer)
	{
		std::cout << "Failed to create mixer: %s\n", SDL_GetError();
		return 0;
	}

	//// Load and play music
	MIX_Audio* Music = MIX_LoadAudio(mixer, "Audio/CrabRave.wav", true);
	MIX_SetMasterGain(mixer, master_volume);
	MIX_PlayAudio(mixer, Music);
	MIX_DestroyAudio(Music);

	return true;

}

void NotYetGone::OnDestroy() {
	//// Turn off audio
	if (mixer)
	{
		MIX_DestroyMixer(mixer);
		MIX_Quit();
	}
	// Clean up the renderer
	if (renderer) {
		SDL_DestroyRenderer(renderer);
		renderer = nullptr;
	}

	// Clean up player
	delete courier;
	courier = nullptr;

	// Clean up level
	delete level;
	level = nullptr;

}

void NotYetGone::HandleEvents(const SDL_Event& event) {

	



}

void NotYetGone::Update(const float deltaTime) {
	
	/// make oer courier moove (◑‿◐)
	if (courier) {
		courier->Update(deltaTime);
		courier->HandleInput(keyboardState);
	}


}

void NotYetGone::Render() const {
	SDL_RenderClear(renderer);

	// render Level class
	if (level)
	level->Render();

	// render Courier 
	if (courier) 
		courier->Render();
	
	// Update the screen
	SDL_RenderPresent(renderer);
}