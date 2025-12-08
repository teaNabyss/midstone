#include "GameManager.h"
#include "Window.h"
#include "Timer.h"
#include "Scene0.h"
#include "Scene1.h"
#include "Scene2.h"
#include "Scene3.h"
#include "Scene4.h"
#include "Scene5.h"
#include "Scene6.h"
#include "Scene7.h"
#include "Scene8.h"
#include <iostream>

GameManager::GameManager() {
	windowPtr = nullptr;
	timer = nullptr;
	isRunning = true;
	currentScene = nullptr;
	currentLevel = 0;
}


bool GameManager::OnCreate() {
	const int SCREEN_WIDTH = 1280;
	const int SCREEN_HEIGHT = 720;
	windowPtr = new Window(SCREEN_WIDTH, SCREEN_HEIGHT);
	if (windowPtr == nullptr) {
		OnDestroy();
		return false;
	}
	if (windowPtr->OnCreate() == false) {
		OnDestroy();
		return false;
	}

	timer = new Timer();
	if (timer == nullptr) {
		OnDestroy();
		return false;
	}

	currentScene = new Scene2(windowPtr->GetSDL_Window());
	if (currentScene == nullptr) {
		OnDestroy();
		return false;
	}

	if (currentScene->OnCreate() == false) {
		OnDestroy();
		return false;
	}

	return true;
}

/// Here's the whole game
void GameManager::Run() {
	SDL_Event event;
	timer->Start();
	while (isRunning) {
		SDL_PollEvent(&event);
		timer->UpdateFrameTicks();

		currentScene->HandleEvents(event);
		currentScene->Update(timer->GetDeltaTime());

		//PLAYER DEATH
		if (currentScene->PlayerDeath()) {
			currentScene->OnDestroy();
			delete currentScene;
			currentScene = nullptr;
			

			switch (currentLevel) {
			case 0:
				currentScene = new Scene0(windowPtr->GetSDL_Window()); break;
			case 1:
				currentScene = new Scene1(windowPtr->GetSDL_Window()); break;
			case 2:
				currentScene = new Scene2(windowPtr->GetSDL_Window()); break;
			case 3:
				currentScene = new Scene3(windowPtr->GetSDL_Window()); break;
			case 4:
				currentScene = new Scene4(windowPtr->GetSDL_Window()); break;
			case 5:
				currentScene = new Scene5(windowPtr->GetSDL_Window()); break;
			case 6:
				currentScene = new Scene6(windowPtr->GetSDL_Window()); break;
			case 7:
				currentScene = new Scene7(windowPtr->GetSDL_Window()); break;
			case 8:
				currentScene = new Scene8(windowPtr->GetSDL_Window()); break;
			default:
				isRunning = false; 
				break;
			}
			if (!currentScene) break;
			if (!currentScene->OnCreate()) {
				isRunning = false;
				break;
			}
			timer->Start();
			continue;
		}
		//LEVEL COMPLETE
		if (currentScene->IsComplete()) {
			currentScene->OnDestroy();
			delete currentScene;
			currentScene = nullptr;
			currentLevel++;

				switch (currentLevel) {

				case 1:
					currentScene = new Scene1(windowPtr->GetSDL_Window()); break;
				case 2:
					currentScene = new Scene2(windowPtr->GetSDL_Window()); break;
				case 3:
					currentScene = new Scene3(windowPtr->GetSDL_Window()); break;
				case 4:
					currentScene = new Scene4(windowPtr->GetSDL_Window()); break;
				case 5:
					currentScene = new Scene5(windowPtr->GetSDL_Window()); break;
				case 6:
					currentScene = new Scene6(windowPtr->GetSDL_Window()); break;
				case 7:
					currentScene = new Scene7(windowPtr->GetSDL_Window()); break;
				case 8:
					currentScene = new Scene8(windowPtr->GetSDL_Window()); break;
				default:
					isRunning = false;
					break;
				}
				if (!isRunning || !currentScene) break;

			if (!currentScene->OnCreate()) {
				isRunning = false;
				break;
			}
			timer->Start();
			continue;
		}

		currentScene->Render();

		if(event.type == SDL_EVENT_QUIT){
			isRunning = false;
		}

		/// Keeep the event loop running at a proper rate
		SDL_Delay(timer->GetSleepTime(60)); ///60 frames per sec
	}
}

GameManager::~GameManager() {}

void GameManager::OnDestroy(){
	if (currentScene) {
		currentScene->OnDestroy();
		delete currentScene;
	}

	if (timer) delete timer;

	if (windowPtr) {
		windowPtr->OnDestroy();
		delete windowPtr;
	}
}

