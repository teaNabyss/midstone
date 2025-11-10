#include "NotYetGone.h"
#include <iostream>
#include <MMath.h>
#include "Levels.h"
#include "Camera.h"
#include "Player.h"

NotYetGone::NotYetGone(SDL_Window* sdlWindow_) : 
	window(sdlWindow_){}

NotYetGone::~NotYetGone(){}

bool NotYetGone::OnCreate() {





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


}

void NotYetGone::HandleEvents(const SDL_Event& event) {



}


void NotYetGone::Update(const float deltaTime) {


}

void NotYetGone::Render() const {

}