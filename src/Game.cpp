#include <ctime>
#include "Game.h"
#include "Util.h"

Game* Game::s_pInstance = 0;

glm::vec2 Game::getTargetPosition()
{
	return m_pTarget->getPosition();
}

SteeringState Game::getDesiredState()
{
	return m_desiredState;
}

bool Game::getGameMenu()
{
	return m_gameMenu;
}

SDL_Renderer * Game::getRenderer()
{
	return m_pRenderer;
}

glm::vec2 Game::getMousePosition()
{
	return m_mousePosition;
}

Game::Game() : m_bDebug(false), m_gameMenu(true)
{
	srand(time(NULL));
}

Game::~Game()
{
}

ship* Game::getShip()
{
	return m_pShip;
}

Target* Game::getTarget()
{
	return m_pTarget;
}

Obstacle* Game::getObstacle()
{
	return m_pObstacle;
}

void Game::createGameObjects()
{
	m_pShip = new ship();
	m_pTarget = new Target();
	m_pObstacle = new Obstacle();
	m_pTitle = new Title();
}

bool Game::init(const char* title, int xpos, int ypos, int height, int width, bool fullscreen)
{
	int flags = 0;

	if (fullscreen)
	{
		flags = SDL_WINDOW_FULLSCREEN;
	}

	// initialize SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) >= 0)
	{
		std::cout << "SDL Init success" << std::endl;

		// if succeeded create our window
		m_pWindow = SDL_CreateWindow(title, xpos, ypos, height, width, flags);

		// if window creation successful create our renderer
		if (m_pWindow != 0)
		{
			std::cout << "window creation success" << std::endl;
			m_pRenderer = SDL_CreateRenderer(m_pWindow, -1, 0);

			if (m_pRenderer != 0) // render init success
			{
				std::cout << "renderer creation success" << std::endl;
				SDL_SetRenderDrawColor(m_pRenderer, 255, 255, 255, 255);
			}
			else
			{
				std::cout << "renderer init failure" << std::endl;
				return false; // render int fail
			}

			//TheTextureManager::Instance()->load("../../Assets/textures/animate-alpha.png", "animate", m_pRenderer);
			createGameObjects();
		}
		else 
		{
			std::cout << "window init failure" << std::endl;
			return false; // window init fail
		}
	}
	else
	{
		std::cout << "SDL init failure" << std::endl;
		return false; //SDL could not intialize
	}

	//Check for joysticks
	if (SDL_NumJoysticks() < 1)
	{
		printf("Warning: No joysticks connected!\n");
	}
	else
	{
		//Load joystick
		m_pGameController = SDL_JoystickOpen(0);
		if (m_pGameController == nullptr)
		{
			printf("Warning: Unable to open game controller! SDL Error: %s\n", SDL_GetError());
		}
	}

	TheSoundManager::Instance()->load("../Assets/audio/Launch.ogg", "LaunchSound", sound_type::SOUND_SFX);

	std::cout << "init success" << std::endl;
	m_bRunning = true; // everything initialized successfully - start the main loop

	return true;
}

void Game::render()
{
	SDL_RenderClear(m_pRenderer); // clear the renderer to the draw colour
	if (getGameMenu() == true)
	{
		m_pTitle->draw();
	}
	else
	{
		m_pTarget->draw();
		m_pShip->draw();
		m_pObstacle->draw();
	}
	if (m_bDebug)
	{
		Util::DrawRect(m_pObstacle->getPosition() - glm::vec2(m_pObstacle->getWidth() * 0.5, m_pObstacle->getHeight() * 0.5), m_pObstacle->getWidth(), m_pObstacle->getHeight());
		Util::DrawCircle(m_pShip->getPosition(), m_pShip->getHeight() * 0.5f);
		Util::DrawCircle(m_pTarget->getPosition(), m_pTarget->getWidth() * 0.5f);
		Util::DrawCircle(m_pTarget->getPosition(), m_pShip->getArrivalTarget(), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
		Util::DrawCircle(m_pTarget->getPosition(), m_pShip->getArrivalRadius(), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
		Util::DrawLine(m_pShip->getPosition(), m_pShip->getPosition() + Util::rotateDirectionVec(m_pShip->getCurrentDirection(), m_pShip->getFeelerAngle()) * m_pShip->getFeelerLength(), glm::vec4(0.5f, 1.0f, 0.0f, 1.0f));
		Util::DrawLine(m_pShip->getPosition(), m_pShip->getPosition() + m_pShip->getCurrentDirection() * m_pShip->getFeelerLength(), glm::vec4(0.5f, 1.0f, 0.0f, 1.0f));
		Util::DrawLine(m_pShip->getPosition(), m_pShip->getPosition() + Util::rotateDirectionVec(m_pShip->getCurrentDirection(), m_pShip->getFeelerAngle() * -1.0f) * m_pShip->getFeelerLength(), glm::vec4(0.5f, 1.0f, 0.0f, 1.0f));
	}
	SDL_RenderPresent(m_pRenderer); // draw to the screen
}

void Game::update()
{
	m_pShip->update();
	m_pTarget->update();
	m_pObstacle->update();
}

void Game::clean()
{
	std::cout << "cleaning game" << std::endl;
	SDL_JoystickClose(m_pGameController);
	m_pGameController = nullptr;
	SDL_DestroyRenderer(m_pRenderer);
	SDL_DestroyWindow(m_pWindow);
	SDL_Quit();
}

void Game::handleEvents()
{
	SDL_Event event;
	if (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			m_bRunning = false;
			break;
		case SDL_MOUSEMOTION:
			m_mousePosition.x = event.motion.x;
			m_mousePosition.y = event.motion.y;
			break;
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym) {
				case SDLK_ESCAPE:
					m_bRunning = false;
					break;
				case SDLK_w:
					m_pTarget->setVelocity(glm::vec2(m_pTarget->getVelocity().x, -1.0f));
					m_pShip->setTargetPosition(m_pTarget->getPosition());
					break;
				case SDLK_s:
					m_pTarget->setVelocity(glm::vec2(m_pTarget->getVelocity().x, 1.0f));
					m_pShip->setTargetPosition(m_pTarget->getPosition());
					break;
				case SDLK_a:
					m_pTarget->setVelocity(glm::vec2(-1.0f, m_pTarget->getVelocity().y));
					m_pShip->setTargetPosition(m_pTarget->getPosition());
					break;
				case SDLK_d:
					m_pTarget->setVelocity(glm::vec2(1.0f, m_pTarget->getVelocity().y));
					m_pShip->setTargetPosition(m_pTarget->getPosition());
					break;
				case SDLK_0:
					m_pShip->setTargetPosition(m_pTarget->getPosition());
					setDesiredState(SteeringState::IDLE);
					break;
				case SDLK_1:
					// reset positions
					SeekState.shipPosition = glm::vec2(100, 300);
					SeekState.targetPosition = glm::vec2(20 + rand() % 760, 20 + rand() % 560);
					SeekState.obstaclePosition = glm::vec2(-500, -500);
					m_pShip->setPosition(SeekState.shipPosition);
					m_pTarget->setPosition(SeekState.targetPosition);
					m_pObstacle->setPosition(SeekState.obstaclePosition);

					// set target and state
					setDesiredState(SteeringState::SEEK);
					m_pShip->setTargetPosition(m_pTarget->getPosition());
					TheSoundManager::Instance()->playSound("LaunchSound", 0);
					break;
				case SDLK_2:
					// reset positions
					ArriveState.shipPosition = glm::vec2(100, 300);
					ArriveState.targetPosition = glm::vec2(20 + rand() % 760, 20 + rand() % 560);
					ArriveState.obstaclePosition = glm::vec2(-500, -500);
					m_pShip->setPosition(ArriveState.shipPosition);
					m_pTarget->setPosition(ArriveState.targetPosition);
					m_pObstacle->setPosition(ArriveState.obstaclePosition);

					// set target and state
					m_pShip->setTargetPosition(m_pTarget->getPosition());
					setDesiredState(SteeringState::ARRIVE);
					TheSoundManager::Instance()->playSound("LaunchSound", 0);
					break;
				case SDLK_3:
					// reset positions
					AvoidState.shipPosition = glm::vec2(100, 300);
					AvoidState.targetPosition = glm::vec2(700, 250);
					AvoidState.obstaclePosition = glm::vec2(375 + rand() % 51, 225 + rand() % 151);
					m_pShip->setPosition(AvoidState.shipPosition);
					m_pTarget->setPosition(AvoidState.targetPosition);
					m_pObstacle->setPosition(AvoidState.obstaclePosition);

					// set target and state
					m_pShip->setTargetPosition(m_pTarget->getPosition());
					setDesiredState(SteeringState::AVOID);
					TheSoundManager::Instance()->playSound("LaunchSound", 0);
					break;
				case SDLK_4:
					// reset positions
					FleeState.shipPosition = glm::vec2(300 + rand() % 101, 250 + rand() % 101);
					FleeState.targetPosition = glm::vec2(400 + rand() % 101, 250 + rand() % 101);
					FleeState.obstaclePosition = glm::vec2(-500, -500);
					m_pShip->setPosition(FleeState.shipPosition);
					m_pTarget->setPosition(FleeState.targetPosition);
					m_pObstacle->setPosition(FleeState.obstaclePosition);

					// set target and state
					m_pShip->setTargetPosition(m_pTarget->getPosition());
					setDesiredState(SteeringState::FLEE);
					TheSoundManager::Instance()->playSound("LaunchSound", 0);
					break;
				case SDLK_RIGHT:
					m_pShip->turnRight(false);
					break;
				case SDLK_LEFT:
					m_pShip->turnLeft(false);
					break;
				case SDLK_UP:
					m_pShip->setVelocity(m_pShip->getCurrentDirection() * m_pShip->getMaxSpeed());
					m_pShip->move();
					break;
				case SDLK_BACKQUOTE:
					m_bDebug = !m_bDebug;
					break;
				case SDLK_TAB:
					m_gameMenu = true;
					break;
			}
			break;
		case SDL_KEYUP:
			switch (event.key.keysym.sym) {
				case SDLK_w:
					if (m_pTarget->getVelocity().y < 0.0f) {
						m_pTarget->setVelocity(glm::vec2(m_pTarget->getVelocity().x, 0.0f));
					}
					break;
				case SDLK_s:
					if (m_pTarget->getVelocity().y > 0.0f) {
						m_pTarget->setVelocity(glm::vec2(m_pTarget->getVelocity().x, 0.0f));
					}
					break;
				case SDLK_a:
					if (m_pTarget->getVelocity().x < 0.0f) {
						m_pTarget->setVelocity(glm::vec2(0.0f, m_pTarget->getVelocity().y));
					}
					break;
				case SDLK_d:
					if (m_pTarget->getVelocity().x > 0.0f) {
						m_pTarget->setVelocity(glm::vec2(0.0f, m_pTarget->getVelocity().y));
					}
					break;
				case SDLK_RIGHT:
					break;
				case SDLK_LEFT:
					break;
				case SDLK_UP:
					break;
			}
			break;
		case SDL_JOYAXISMOTION:
			switch(event.jaxis.which) {
				case 0:
					switch(event.jaxis.axis) {
						case 0:
							// Left of dead zone
							if (event.jaxis.value < -JOYSTICK_DEAD_ZONE)
							{
								m_joystickPosition.x = event.jaxis.value / 32768;
							}
							// Right of dead zone
							else if (event.jaxis.value > JOYSTICK_DEAD_ZONE)
							{
								m_joystickPosition.x = event.jaxis.value / 32767;
							}
							// Inside dead zone
							else
							{
								m_joystickPosition.x = 0.0f;
							}
							m_pTarget->setVelocity(glm::vec2(m_joystickPosition.x, m_pTarget->getVelocity().y));
							m_pShip->setTargetPosition(m_pTarget->getPosition());
							break;
						case 1:
							// Below dead zone
							if (event.jaxis.value < -JOYSTICK_DEAD_ZONE)
							{
								m_joystickPosition.y = event.jaxis.value / 32768;
							}
							// Above dead zone
							else if (event.jaxis.value > JOYSTICK_DEAD_ZONE)
							{
								m_joystickPosition.y = event.jaxis.value / 32767;
							}
							// Inside dead zone
							else
							{
								m_joystickPosition.y = 0.0f;
							}
							m_pTarget->setVelocity(glm::vec2(m_pTarget->getVelocity().x, m_joystickPosition.y));
							m_pShip->setTargetPosition(m_pTarget->getPosition());
							break;
						default:
							break;
					}
				default:
					break;
			}
		default:
			break;
		}
	}
}

void Game::setDesiredState(SteeringState state)
{
	m_gameMenu = false;
	m_pShip->setSteeringState(state, 0);
	if (state == ARRIVE || state == AVOID)
	{
		m_pShip->setSteeringState(SEEK);
	}
	else
	{
		m_pShip->setSteeringState(state);
	}
}
