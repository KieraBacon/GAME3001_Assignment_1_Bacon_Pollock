#pragma once
#ifndef __Game__
#define __Game__

// Core Libraries
#include <iostream>
#include <vector>

#include <SDL.h>
#include <SDL_image.h>

// Game Managers
#include "TextureManager.h"
#include "CollisionManager.h"

// Game Objects
#include "Player.h"
#include "Island.h"
#include "Ocean.h"
#include "Cloud.h"
#include "ship.h"
#include "Target.h"
#include "Obstacle.h"
#include "ObjectData.h"
#include "Title.h"

class Game
{
public:
	bool m_bDebug;

	static Game* Instance()
	{
		if (s_pInstance == nullptr)
		{
			s_pInstance = new Game();
			return s_pInstance;
		}

		return s_pInstance;
	}

	// simply set the running variable to true
	void init() { m_bRunning = true; }

	bool init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);

	// public functions
	void render();
	void update();
	void handleEvents();
	void clean();

	// a function to access the private running variable
	bool running() { return m_bRunning; }

	// setters
	ObjectData SeekState;
	ObjectData ArriveState;
	ObjectData AvoidState;
	ObjectData FleeState;

	void setDesiredState(SteeringState state);					// ADDED

	// getters
	SDL_Renderer* getRenderer();
	glm::vec2 getMousePosition();
	glm::vec2 getTargetPosition();
	SteeringState getDesiredState();							// ADDED
	bool getGameMenu();											// ADDED
	ship* getShip();
	Target* getTarget();
	Obstacle* getObstacle();

private:
	Game();
	~Game();

	SDL_Window* m_pWindow;
	SDL_Renderer* m_pRenderer;

	int m_currentFrame;
	bool m_gameMenu;											// ADDED
	SteeringState m_desiredState;								// ADDED

	bool m_bRunning;

	static Game* s_pInstance;

	// GameObjects
	ship* m_pShip;
	Target* m_pTarget;
	Obstacle* m_pObstacle;
	Title* m_pTitle;

	void createGameObjects();

	// Game Controllers
	glm::vec2 m_mousePosition;
	const int JOYSTICK_DEAD_ZONE = 8000;
	SDL_Joystick* m_pGameController = nullptr;
	glm::vec2 m_joystickPosition;

};

typedef Game TheGame;

#endif /*defined (__Game__)*/

